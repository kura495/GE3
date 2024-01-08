#include"Scenes/State/GameTitleState.h"

void GameTitleState::Initialize()
{
	input = Input::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	textureHundle = textureManager_->LoadTexture("resources/title.png");
	texture_world_.Initialize();
	texture_world_.translation_.y = 150.0f;
	texture_world_.translation_.x = 230.0f;
	texture_world_.UpdateMatrix();
	texture = std::make_unique<Sprite>();
	texture->Initialize({ 0.0f,0.0f,0.0f,1.0f }, { 0.0f,420.0f,0.0f,1.0f }, { 820.0f,0.0f,0.0f,1.0f }, { 820.0f,420.0f,0.0f,1.0f });

	textureHundle2 = textureManager_->LoadTexture("resources/PressAButton.png");
	texture2_world_.Initialize();
	texture2_world_.translation_.y = 570.0f;
	texture2_world_.translation_.x = 386.0f;
	texture2_world_.UpdateMatrix();
	texture2 = std::make_unique<Sprite>();
	texture2->Initialize({ 0.0f,0.0f,0.0f,1.0f }, { 0.0f,72.0f,0.0f,1.0f }, { 508.0f,0.0f,0.0f,1.0f }, { 508.0f,72.0f,0.0f,1.0f });
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
	texture2->Draw(texture2_world_,textureHundle2);
}
