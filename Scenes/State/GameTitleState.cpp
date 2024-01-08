#include"Scenes/State/GameTitleState.h"

void GameTitleState::Initialize()
{
	input = Input::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	textureHundle = textureManager_->LoadTexture("resources/inst.png");
	texture_world_.Initialize();
	texture_world_.translation_.y = 600.0f;
	texture_world_.UpdateMatrix();
	texture = std::make_unique<Sprite>();
	texture->Initialize({ 0.0f,0.0f,0.0f,1.0f }, { 0.0f,420.0f,0.0f,1.0f }, { 1280.0f,0.0f,0.0f,1.0f }, { 820.0f,420.0f,0.0f,1.0f });
	time = 0;
	IsCanPush = false;
}

void GameTitleState::Update()
{
	time++;
	if (time > 60) {
		IsCanPush = true;
	}

	input->GetJoystickState(0,joyState);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		if (IsCanPush) {
			StateNo = 1;
		}
	}
}

void GameTitleState::Draw()
{
	texture->Draw(texture_world_,textureHundle);
}
