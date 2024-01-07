#include "Player.h"
#include "GameObject/Player/LockOn.h"


const std::array<ConstAttack, Player::ComboNum>Player::kConstAttacks_ = {
	{
	{10,0,20,0,0.0f,0.0f,0.15f},
	{20,10,15,0,0.2f,0.0f,0.0f},
	{20,10,15,30,0.2f,0.0f,0.0f},
	}
};

void Player::Initialize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);


	BoxCollider::Initialize();
	input = Input::GetInstance();

	WorldTransformInitalize();

	BoxCollider::SetcollisionMask(~kCollitionAttributePlayer);
	BoxCollider::SetcollitionAttribute(kCollitionAttributePlayer);
	BoxCollider::SetParent(worldTransform_);
	BoxCollider::SetSize({3.0f,3.0f,1.0f});

	models_[kModelIndexBody]->SetLightMode(Lighting::harfLambert);

	const char* groupName = "Player";
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	moveQuaternion_ = IdentityQuaternion();
}

void Player::Update()
{	
	//パッドの状態をゲット
	input->GetJoystickState(0,joyState);

	if (behaviorRequest_) {
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (behavior_)
		{
		case Behavior::kRoot:
		default:
			BehaviorRootInit();
			break;
		case Behavior::kGrap:
			GrapInit();
			break;
		case Behavior::kJump:
			BehaviorJumpInit();
			break;
		}		
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kGrap:
		GrapUpdate();
		break;
	case Behavior::kJump:
		BehaviorJumpUpdate();
		break;
	}

	if (worldTransform_.translation_.y <= -20.0f) {
		//地面から落ちたらリスタートする
		IsAlive = false;
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		worldTransform_.UpdateMatrix();
		behaviorRequest_ = Behavior::kRoot;
	}

	worldTransform_.quaternion = Normalize(worldTransform_.quaternion);

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformArrow_.UpdateMatrix();

	//つかめない
	canGrap = false;

	BoxCollider::Update(&worldTransform_);
	//前フレームのゲームパッドの状態を保存
	joyStatePre = joyState;
}
void Player::Draw(const ViewProjection& viewProjection)
{
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	if (behavior_ == Behavior::kGrap) {
		models_[kModelArrow]->Draw(worldTransformArrow_, viewProjection);
	}
}
void Player::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributeEnemy) {
		//敵に当たったらリスタートする
		//ペアレントの解除
		DeleteParent();
		IsAlive = false;
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		worldTransform_.UpdateMatrix();
		behaviorRequest_ = Behavior::kRoot;
	}
	else if (collider->GetcollitionAttribute() == kCollitionAttributeFloor) {
		IsOnGraund = true;
	}
	else if (collider->GetcollitionAttribute() == kCollitionAttributeMoveFloor) {
		IsOnGraund = true;
	}
	else if (collider->GetcollitionAttribute() == kCollitionAttributeSango) {
		canGrap = true;
		grapPoint = collider->GetColliderWorld().GetTranslateFromMatWorld();
	}
	else {
		return;
	}
	
}
void Player::SetParent(const WorldTransform* parent)
{
	// 親子関係を結ぶ
	if (!worldTransform_.parent_) {
		worldTransform_.parent_ = parent;
		Vector3 Pos = Subtract(worldTransform_.GetTranslateFromMatWorld(), parent->GetTranslateFromMatWorld());
		worldTransform_.translation_ = Pos;
		worldTransform_.UpdateMatrix();
	}
}
void Player::WorldTransformInitalize()
{
	worldTransformBody_.Initialize();
	worldTransformArrow_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;
}
void Player::Move()
{
		//移動量
		if (joyState.Gamepad.sThumbLX != 0 && joyState.Gamepad.sThumbLY != 0) {
			move = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
			//正規化をして斜めの移動量を正しくする
			move = Normalize(move);
			move.x =move.x * speed;
			move.y =move.y * speed;
			move.z =move.z * speed;
			//カメラの正面方向に移動するようにする
			//回転行列を作る
			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
			//移動ベクトルをカメラの角度だけ回転
			move = TransformNormal(move, rotateMatrix);
			//移動
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);
			//プレイヤーの向きを移動方向に合わせる
			move = Normalize(move);
			move.y = 0.0f;
			Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, move));
			float dot = Dot({ 0.0f,0.0f,1.0f }, move);
			moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

		}
		else if (lockOn_ && lockOn_->ExistTarget()) {
			//ロックオン座標
			Vector3 lockOnPosition = lockOn_->GetTargetPosition();
			lockOnPosition.y = 0;
			//追従対象からロックオン対象へのベクトル
			Vector3 sub = lockOnPosition - worldTransform_.GetTranslateFromMatWorld();

			//プレイヤーの現在の向き
			sub = Normalize(sub);

			Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, sub));
			float dot = Dot({0.0f,0.0f,1.0f}, sub);

			//行きたい方向のQuaternionの作成
			moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
		}

		worldTransform_.quaternion = Slerp(worldTransform_.quaternion, moveQuaternion_, 0.3f);

		worldTransform_.UpdateMatrix();
}
void Player::BehaviorRootInit()
{
	InitializeFloatingGimmick();
	DownForce = 0.05f;

}
void Player::BehaviorRootUpdate()
{

	UpdateFloatingGimmick();
	Move();
	PullDown();
	//RTで攻撃
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//前のフレームでは押していない
		if (!joyStatePre.Gamepad.wButtons && XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			if (canGrap) {
				behaviorRequest_ = Behavior::kGrap;
			}
		}
	}
	//Bでジャンプ
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		behaviorRequest_ = Behavior::kJump;
	}

}
void Player::BehaviorJumpInit()
{
	worldTransformBody_.translation_.y = 0;

	//ジャンプ初速
	const float kJumpFirstSpeed = 0.5f;

	move.y = kJumpFirstSpeed;

}
void Player::BehaviorJumpUpdate()
{
	//移動
	worldTransform_.translation_ += move;
	//重力加速度
	const float kGravity = 0.05f;
	//加速度ベクトル
	Vector3 accelerationVector = { 0.0f,-kGravity,0.0f };
	//加速する
	move += accelerationVector;

	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0.0f;
		//ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}
}
void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;
}
void Player::UpdateFloatingGimmick() {
	// 浮遊移動のサイクル<frame>
	const uint16_t T = (uint16_t)floatcycle_;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * (float)std::numbers::pi / T;
	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = (float)std::fmod(floatingParameter_, 2.0f * std::numbers::pi);
	// 浮遊の振幅<m>
	const float floatingAmplitude = floatingAmplitude_;
	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude;

}
void Player::PullDown()
{
	if (IsOnGraund) {
		worldTransform_.translation_.y = 0.0f;
		IsOnGraund = false;
		return;
	}
	else {
		
		//重力加速度
		const float kGravity = 0.98f;
		//加速度ベクトル
		Vector3 accelerationVector = { 0.0f,-kGravity,0.0f };
		//移動
		worldTransform_.translation_.y += accelerationVector.y;

	}
}
void Player::GrapInit()
{

	worldTransform_.translation_ = grapPoint;
	worldTransform_.translation_.z -= 2.0f;
	worldTransform_.UpdateMatrix();
	rotateQua = IdentityQuaternion();
	moveQuaternion_ = IdentityQuaternion();
	worldTransform_.quaternion = IdentityQuaternion();
	worldTransformArrow_.translation_ = grapPoint;
	beginVecQua = IdentityQuaternion();
	endVecQua = IdentityQuaternion();
	lerpQua = IdentityQuaternion();
	 IsOnGraund = false;
	angleParam = 0.0f;
	moveVector = {0.0f};
	grapJump = false;
	grapJumpAnime = 0;
	angle = 1.0f;
	GrapBehaviorRequest_ = GrapBehavior::kRight;

}
void Player::GrapUpdate()
{
	if (joyState.Gamepad.sThumbLX != 0) {
		if (joyState.Gamepad.sThumbLX > 0 && GrapBehavior_!= GrapBehavior::kRight) {
			GrapBehaviorRequest_ = GrapBehavior::kRight;
		}
		else if (joyState.Gamepad.sThumbLX < 0 && GrapBehavior_ != GrapBehavior::kLeft) {
			GrapBehaviorRequest_ = GrapBehavior::kLeft;
		}
	}

	if (GrapBehaviorRequest_) {
		//ふるまいの変更
		GrapBehavior_ = GrapBehaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (GrapBehavior_)
		{
		case GrapBehavior::kLeft:
			GrapJumpLeftInitalize();
			break;
		case GrapBehavior::kRight:
		default:
			GrapJumpRightInitalize();
			break;
		}
		GrapBehaviorRequest_ = std::nullopt;
	}
	switch (GrapBehavior_)
	{
	case GrapBehavior::kLeft:
		GrapJumpLeftUpdate();
		break;
	case GrapBehavior::kRight:
	default:
		GrapJumpRightUpdate();
		break;
	}
	worldTransform_.quaternion = Multiply(worldTransform_.quaternion, rotateQua);
}
void Player::GrapJumpLeftInitalize()
{
	rotateQua = IdentityQuaternion();
	moveQuaternion_ = IdentityQuaternion();
	worldTransform_.quaternion = IdentityQuaternion();
	beginVecQua = IdentityQuaternion();
	endVecQua = IdentityQuaternion();
	lerpQua = IdentityQuaternion();
	angleParam = 0.0f;
	grapJumpAnime = 0;
	angle = 1.0f;
	Vector3 cross = Normalize(Cross({ 1.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f }));
	beginVecQua = MakeRotateAxisAngleQuaternion(cross, std::acos(-1.0f));
	worldTransformArrow_.quaternion = beginVecQua;
	beginVecQua = Normalize(beginVecQua);
	endVecQua = MakeRotateAxisAngleQuaternion(cross, std::acos(0.0f));
	endVecQua = Normalize(endVecQua);
}
void Player::GrapJumpLeftUpdate()
{
	grapJumpVec = { 1.0f,0.0f,0.0f };
	//////回転行列を作る
	lerpQua = Normalize(lerpQua);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(lerpQua);
	//移動ベクトルをカメラの角度だけ回転
	grapJumpVec = TransformNormal(grapJumpVec, rotateMatrix);
	grapJumpVec = Normalize(grapJumpVec);
	Vector3 cross = Normalize(Cross({ -1.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f }));
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//前のフレームでは押していない
		if (!joyStatePre.Gamepad.wButtons && XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::kRoot;
		}
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {

		if (angle > 0.9f) {
			angle -= 0.001f;
		}
		else {
			angle = 0.9f;
		}
		rotateQua = MakeRotateAxisAngleQuaternion(cross, std::acos(angle));
		rotateQua = Normalize(rotateQua);

		if (angleParam < 1.0f) {
			angleParam += 0.005f;
		}
		else if (angleParam > 1.0f) {
			angleParam = 1.0f;
		}
		lerpQua = Slerp(beginVecQua, endVecQua, angleParam);
	
		worldTransformArrow_.quaternion = lerpQua;

	}
	else if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		if (joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			grapJump = true;
			moveVector = grapJumpVec;
		}

	}
	if (grapJump == true && grapJumpAnime <= 50) {
		moveVector.y -= 0.03f;
		worldTransform_.translation_.x += moveVector.x;
		worldTransform_.translation_.y += moveVector.y;
		worldTransform_.translation_.z += moveVector.z;
		grapJumpAnime++;
	}
	else if (grapJumpAnime >= 10) {
		grapJump = false;
		behaviorRequest_ = Behavior::kRoot;
	}
}
void Player::GrapJumpRightInitalize()
{
	rotateQua = IdentityQuaternion();
	moveQuaternion_ = IdentityQuaternion();
	worldTransform_.quaternion = IdentityQuaternion();
	beginVecQua = IdentityQuaternion();
	endVecQua = IdentityQuaternion();
	lerpQua = IdentityQuaternion();
	worldTransformArrow_.quaternion = IdentityQuaternion();
	angleParam = 0.0f;
	grapJumpAnime = 0;
	angle = 1.0f;
	Vector3 cross = Normalize(Cross({ 1.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f }));
	beginVecQua = MakeRotateAxisAngleQuaternion(cross, std::acos(1.0f));
	beginVecQua = Normalize(beginVecQua);
	endVecQua = MakeRotateAxisAngleQuaternion(cross, std::acos(0.0f));
	endVecQua = Normalize(endVecQua);

}
void Player::GrapJumpRightUpdate()
{
	grapJumpVec = { 1.0f,0.0f,0.0f };
	//////回転行列を作る
	lerpQua = Normalize(lerpQua);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(lerpQua);
	//移動ベクトルをカメラの角度だけ回転
	grapJumpVec = TransformNormal(grapJumpVec, rotateMatrix);
	grapJumpVec = Normalize(grapJumpVec);
	Vector3 cross = Normalize(Cross({ 1.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f }));
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//前のフレームでは押していない
		if (!joyStatePre.Gamepad.wButtons && XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::kRoot;
		}
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {

		if (angle > 0.9f) {
			angle -= 0.001f;
		}
		else {
			angle = 0.9f;
		}
		rotateQua = MakeRotateAxisAngleQuaternion(cross, std::acos(angle));
		rotateQua = Normalize(rotateQua);


		if (angleParam < 1.0f) {
			angleParam += 0.005f;
		}
		else if (angleParam > 1.0f) {
			angleParam = 1.0f;
		}
		lerpQua = Slerp(beginVecQua, endVecQua, angleParam);

		worldTransformArrow_.quaternion = lerpQua;

	}
	else if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		if (joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			grapJump = true;

			moveVector = grapJumpVec;
		}

	}
	if (grapJump == true && grapJumpAnime <= 50) {
		moveVector.y -= 0.03f;
		worldTransform_.translation_.x += moveVector.x;
		worldTransform_.translation_.y += moveVector.y;
		worldTransform_.translation_.z += moveVector.z;
		grapJumpAnime++;
	}
	else if (grapJumpAnime >= 10) {
		grapJump = false;
		behaviorRequest_ = Behavior::kRoot;
	}
}
