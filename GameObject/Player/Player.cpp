#include "Player.h"

void Player::Initialize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);
	BoxCollider::Initalize();
	input = Input::GetInstance();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformBody_.parent_ = &worldTransform_;
	const char* groupName = "Player";
	BoxCollider::SetcollisionMask(~kCollitionAttributePlayer);
	BoxCollider::SetcollitionAttribute(kCollitionAttributePlayer);
	BoxCollider::SetParent(worldTransform_);
	BoxCollider::SetSize({1.0f,1.0f,1.0f});
	//GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->AddItem(groupName,"speed",speed);
}

void Player::Update()
{	
	ImGui();
	ApplyGlobalVariables();
	UpdateFloatingGimmick();

	Move();

	PullDown();

	if (worldTransform_.translation_.y <= -10.0f) {
		//地面から落ちたらリスタートする
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		worldTransform_.UpdateMatrix();
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	BoxCollider::Update(&worldTransform_);
	
	
}

void Player::Draw(const ViewProjection& viewProjection)
{
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

void Player::BoxOnCollision(uint32_t collisionAttribute)
{
	if (collisionAttribute == kCollitionAttributeEnemy) {
		ImGui::Begin("Player");
		ImGui::Text("Hit!!!!!");
		ImGui::End();
		//敵に当たったらリスタートする
		worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
		worldTransform_.UpdateMatrix();
	}
	else if (collisionAttribute == kCollitionAttributeFloor) {
		ImGui::Begin("Player");
		ImGui::Text("PlaneHit!!!!!");
		ImGui::End();
		IsOnGraund = true;
	}
	else if (collisionAttribute == kCollitionAttributeMoveFloor) {
		ImGui::Begin("Player");
		ImGui::Text("MovePlaneHit!!!!!");
		ImGui::End();
		IsOnGraund = true;
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

void Player::Move()
{
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//移動量
		if (joyState.Gamepad.sThumbLX == 0 && joyState.Gamepad.sThumbLX == 0 && joyState.Gamepad.sThumbLY == 0 && joyState.Gamepad.sThumbLY == 0) {
			return;
		}
		Vector3 move{
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		//正規化をして斜めの移動量を正しくする
		move.x = Normalize(move).x * speed;
		move.y = Normalize(move).y * speed;
		move.z = Normalize(move).z * speed;
		//カメラの正面方向に移動するようにする
		//回転行列を作る
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, rotateMatrix);
		//移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		//プレイヤーの向きを移動方向に合わせる
		//playerのY軸周り角度(θy)
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}
}

void Player::ApplyGlobalVariables()
{
	const char* groupName = "Player";
	speed = GlobalVariables::GetInstance()->GetfloatValue(groupName, "speed");
}

void Player::ImGui()
{
	ImGui::Begin("Player");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x,-10, 10);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x,-10, 10);
	ImGui::SliderFloat3("scale", &worldTransform_.scale_.x,-10, 10);
	ImGui::End();
	//model->ImGui("Player");
}

void Player::InitializeFloatingGimmick() {
	floatingParameter_ = 0.0f;
}

void Player::UpdateFloatingGimmick() {
	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head", &worldTransformHead_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("ArmL", &worldTransformL_arm_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderFloat3("ArmR", &worldTransformR_arm_.translation_.x, 0.0f, 10.0f);
	ImGui::SliderInt("cycle", &floatcycle_, 1, 120);
	ImGui::SliderFloat("Amplitude", &floatingAmplitude_, 0.0f, 10.0f);
	ImGui::End();
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
		IsOnGraund = false;
		return;
	}
	else {
		worldTransform_.translation_.y -= DownForce;
	}
}
