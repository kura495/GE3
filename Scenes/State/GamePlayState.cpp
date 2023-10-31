#include "Scenes/State/GamePlayState.h"

void GamePlayState::Initialize()
{
	//基本機能生成
	camera_ = new Camera();
	camera_->Initialize(1280, 720);
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();

	DirectX_ = DirectXCommon::GetInstance();
	//
	//3Dオブジェクト生成
	player = new Player();
	player->Initialize();

	particle = new Particle();
	particle->Initalize(2);
	//

	//
	//リソースを作る
	//テクスチャ
	Texture = textureManager_->LoadTexture("resources/uvChecker.png");
	//サウンド

	//
	viewProjection_.Initialize();
	worldTransform_.Initialize();
}

void GamePlayState::Update()
{
#ifdef _DEBUG
if (input->IspushKey(DIK_LALT)) {
		camera_->DebugCamera(true);
}
else {
	camera_->DebugCamera(false);
}
#endif // _DEBUG
	GlobalVariables::GetInstance()->Update();

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("transform", &viewProjection_.translation_.x, 10.0f, -10.0f);
	ImGui::SliderFloat3("rotation", &viewProjection_.rotation_.x, 10.0f, -10.0f);
	ImGui::End();
	light_->ImGui("Light");
	viewProjection_.UpdateMatrix();
	
	player->Update();
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	player->Draw(viewProjection_);
	particle->Draw(viewProjection_);




	//3Dモデル描画ここまで	
	//描画ここまで
}
