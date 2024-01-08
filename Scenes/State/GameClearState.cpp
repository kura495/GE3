#include"Scenes/State/GameClearState.h"

void GameClearState::Initialize()
{
	input = Input::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	textureHundle = textureManager_->LoadTexture("resources/inst.png");
	texture_world_.Initialize();
	texture_world_.translation_.y = 600.0f;
	texture_world_.UpdateMatrix();
	texture = std::make_unique<Sprite>();
	texture->Initialize({ 0.0f,0.0f,0.0f,1.0f }, { 0.0f,104.0f,0.0f,1.0f }, { 1280.0f,0.0f,0.0f,1.0f }, { 1280.0f,104.0f,0.0f,1.0f });
}

void GameClearState::Update()
{
	input->GetJoystickState(0, joyState);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		StateNo = 1;
	}
}

void GameClearState::Draw()
{
	texture->Draw(texture_world_, textureHundle);
}

