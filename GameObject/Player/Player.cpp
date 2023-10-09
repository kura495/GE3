#include "Player.h"

void Player::Initialize()
{
	input = Input::GetInstance();
	model = Model::CreateModelFromObj("resources","cube.obj");
	
	worldTransform_.Initialize();

	const char* groupName = "Player";

	//GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->AddItem(groupName,"speed",speed);
}

void Player::Update()
{
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			//移動量
			Vector3 move{
				(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
				(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
			//正規化をして斜めの移動量を正しくする
			move.x = Normalize(move).x * speed;
			move.y = Normalize(move).y * speed;
			move.z = Normalize(move).z * speed;
			//カメラの正面方向に移動するようにする
			//回転行列を作る
			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);
			//移動ベクトルをカメラの角度だけ回転
			move = TransformNormal(move, rotateMatrix);
			//移動
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);
			//プレイヤーの向きを移動方向に合わせる
			//playerのY軸周り角度(θy)
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);

		}
	}
	
	ImGui();
	worldTransform_.UpdateMatrix();
	ApplyGlobalVariables();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	model->Draw(worldTransform_, viewProjection);
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
	model->ImGui("Player");
}
