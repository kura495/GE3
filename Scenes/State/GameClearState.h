#pragma once
#include"IgameState.h"
#include "Base/Input/Input.h"
#include "Base/TextureManager.h"
#include "Base/Sprite.h"
#include "Base/WorldTransform.h"

class GameClearState :public GameState
{
public:
	void Initialize();
	void Update();
	void Draw();
private:
	TextureManager* textureManager_;

	std::unique_ptr<Sprite>texture;
	uint32_t textureHundle;
	WorldTransform texture_world_;

	std::unique_ptr<Sprite>texture2;
	uint32_t textureHundle2;
	WorldTransform texture2_world_;

	Input* input;
	XINPUT_STATE joyState;
};
