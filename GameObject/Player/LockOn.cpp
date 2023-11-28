#include "LockOn.h"


void LockOn::Initalize()
{
	input = Input::GetInstance();
	Texture_ = TextureManager::GetInstance()->LoadTexture("resources/reticle.png");
	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_->Initialize({-1.0f,1.0f,0.0f,0.0f},{-1.0f,-1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f,0.0f},{1.0f,-1.0f,0.0f,0.0f});
	world_.Initialize();

}

void LockOn::Update(const std::list<Enemy*>& enemies,const ViewProjection& viewProjection)
{
	input->GetJoystickState(0,joyState);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {

	}
	return;
	enemies;
	viewProjection;
}

void LockOn::Draw()
{
	if (target_) {
		lockOnMark_->Draw(world_, Texture_);
	}
}

void LockOn::search(const std::list<Enemy*>& enemies, const ViewProjection& viewProjection)
{
	//目標
	std::list<std::pair<float, const Enemy*>> targets;
	//全ての敵に対して順にロックオン判定

}
