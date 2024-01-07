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
	std::vector<Model*> PlayerModel = { models[kModelIndexBody],models[kModelIndexHead],models[kModelIndexL_arm],models[kModelIndexR_arm]
	};
	BaseCharacter::Initialize(PlayerModel);


	BoxCollider::Initialize();
	input = Input::GetInstance();

	WorldTransformInitalize();

	BoxCollider::SetcollisionMask(~kCollitionAttributePlayer);
	BoxCollider::SetcollitionAttribute(kCollitionAttributePlayer);
	BoxCollider::SetParent(worldTransform_);
	BoxCollider::SetSize({3.0f,3.0f,1.0f});

	models_[kModelIndexBody]->SetLightMode(Lighting::harfLambert);
	models_[kModelIndexHead]->SetLightMode(Lighting::harfLambert);
	models_[kModelIndexL_arm]->SetLightMode(Lighting::harfLambert);
	models_[kModelIndexR_arm]->SetLightMode(Lighting::harfLambert);

	const char* groupName = "Player";
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	moveQuaternion_ = IdentityQuaternion();
}

void Player::Update()
{	
	//jsonファイルの内容を適応
	ApplyGlobalVariables();
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
	}

	if (worldTransform_.translation_.y <= -20.0f) {
		//地面から落ちたらリスタートする
		IsAlive = false;
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		worldTransform_.UpdateMatrix();
	}

	

	worldTransform_.quaternion = Normalize(worldTransform_.quaternion);

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	//つかめない
	canGrap = false;

	BoxCollider::Update(&worldTransform_);
	//前フレームのゲームパッドの状態を保存
	joyStatePre = joyState;
}
void Player::Draw(const ViewProjection& viewProjection)
{
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
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
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	//腕の位置調整
	worldTransformL_arm_.translation_.y = 1.4f;
	worldTransformR_arm_.translation_.y = 1.4f;
	//武器の位置調整

	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
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
	worldTransformL_arm_.quaternion = IdentityQuaternion();
	worldTransformR_arm_.quaternion = IdentityQuaternion();
	worldTransformR_arm_.translation_.z = 0;
	worldTransformR_arm_.UpdateMatrix();
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
	worldTransform_.UpdateMatrix();
	rotateQua = IdentityQuaternion();
	moveQuaternion_ = IdentityQuaternion();
	worldTransform_.quaternion = IdentityQuaternion();
	Vector3 cross = Normalize(Cross({ 1.0f,0.0f,0.0f }, { 0.0f,1.0f,0.0f }));
	beginVecQua = IdentityQuaternion();
	beginVecQua = MakeRotateAxisAngleQuaternion(cross, std::acos(1.0f));
	beginVecQua = Normalize(beginVecQua);

	endVecQua = IdentityQuaternion();
	 endVecQua = MakeRotateAxisAngleQuaternion(cross, std::acos(0.0f));
	 endVecQua = Normalize(endVecQua);
	 lerpQua = IdentityQuaternion();
	 IsOnGraund = false;
	angleParam = 0.0f;
	grapJumpAnime = 0;

}
void Player::GrapUpdate()
{
	grapJumpVec = { 1.0f,0.0f,0.0f };
	//////回転行列を作る
	lerpQua = Normalize(lerpQua);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(lerpQua);
	//移動ベクトルをカメラの角度だけ回転
	grapJumpVec = TransformNormal(grapJumpVec, rotateMatrix);
	grapJumpVec = Normalize(grapJumpVec);
	Vector3 cross = Normalize(Cross({ 1.0f,0.0f,0.0f }, {0.0f,1.0f,0.0f}));
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//前のフレームでは押していない
		if (!joyStatePre.Gamepad.wButtons && XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				behaviorRequest_ = Behavior::kRoot;
		}
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		rotateQua = MakeRotateAxisAngleQuaternion(cross, std::acos(0.9f));
		rotateQua = Normalize(rotateQua);
		worldTransform_.quaternion = Multiply(worldTransform_.quaternion, rotateQua);
		if (angleParam < 1.0f) {
			angleParam += 0.02f;
		}
		else if (angleParam > 1.0f) {
			angleParam = 1.0f;
		}
		lerpQua = Slerp(beginVecQua,endVecQua, angleParam);


	}
	else if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
		if (joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			grapJump = true;
			moveVector = grapJumpVec*2;
		}

	}
	if (grapJump == true && grapJumpAnime <= 10) {
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
