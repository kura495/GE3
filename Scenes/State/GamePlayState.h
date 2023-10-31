#pragma once
#include "IgameState.h"
#include "MyEngine.h"
#include "ModelData.h"
#include "Audio.h"
#include "Input.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Light.h"
#include "Sphere.h"
#include "TextureManager.h"
#include "Model.h"
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "GameObject/Camera/Camera.h"
#include "GameObject/Player/Player.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Math_Structs.h"
#include "Base/Particle.h"

class GamePlayState :public GameState
{
public:

	void Initialize();
	void Update();
	void Draw();

private:
	//基本機能ズ
	MyEngine* myEngine = nullptr;
	Camera* camera_ = nullptr;
	TextureManager* textureManager_ = nullptr;	
	Input* input = nullptr;
	Audio* audio=nullptr;
	Light* light_ = nullptr;
	DirectXCommon* DirectX_ = nullptr;
	GlobalVariables* globalVariables = nullptr;
	Model* model = nullptr;
	//
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//3Dオブジェクトたち
	Player* player = nullptr;
	Particle* particle = nullptr;
	//2Dオブジェクトたち
	Sprite* sprite;
	//
	//テクスチャハンドルたち
	int Texture;
	//

	
};