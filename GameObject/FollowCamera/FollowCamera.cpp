#include "FollowCamera.h"
#include "GameObject/Player/LockOn.h"

void FollowCamera::Initalize() { 
	viewProjection_.Initialize();

	const char* groupName = "Camera";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->AddItem(groupName, "interpolationPatameter", workInter.interParameter_);
}

void FollowCamera::Update() {

	ApplyGlobalVariables();
	if (target_) {
		Vector3 pos = target_->translation_;
		//もしペアレントを結んでいるなら
		if (target_->parent_) {
			pos = Add(target_->translation_, target_->parent_->translation_);
		}
		//追従座標の補間
		workInter.interTarget_ = VectorLerp(workInter.interTarget_, pos , workInter.interParameter_);

		Vector3 offset = OffsetCalc();
		//オフセット分と追従座標の補間分ずらす
		viewProjection_.translation_ = workInter.interTarget_ + offset;
	}

	//スティックでのカメラ回転
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float kRadian = 0.02f;

		rotate_.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kRadian;
		rotate_.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRadian;
		if (rotate_.y > 1.0f) {
			rotate_.y = 1.0f;
		}
		else if (rotate_.y < -1.0f) {
			rotate_.y = -1.0f;
		}


		parameter_t = 1.0f;
	}
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, rotate_.x, parameter_t);
	viewProjection_.rotation_.x = LerpShortAngle(viewProjection_.rotation_.x, rotate_.y, parameter_t);
	viewProjection_.UpdateMatrix();
}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}

void FollowCamera::ApplyGlobalVariables()
{
	const char* groupName = "Camera";
	workInter.interParameter_ = GlobalVariables::GetInstance()->GetfloatValue(groupName, "interpolationPatameter");
}

void FollowCamera::Reset()
{
	//追従対象がいれば
	if (target_) {
		//追従座標・角度の初期化
		workInter.interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}
	workInter.targetAngleY_ = viewProjection_.rotation_.y;

	//追従大賞からのオフセット
	Vector3 offset = OffsetCalc();
	viewProjection_.translation_ = workInter.interTarget_ + offset;
}

Vector3 FollowCamera::OffsetCalc()
{
	Vector3 offset = { 0.0f, 2.0f, -15.0f };
	//回転行列の合成
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

	// オフセットをカメラの回転に合わせて回転
	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}
