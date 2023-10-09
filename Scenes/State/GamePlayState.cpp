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
	player = std::make_unique<Player>();
	player->Initialize();
	player->SetViewProjection(viewProjection_);
	Skydome_ = std::make_unique<Skydome>();
	Skydome_->Initalize();
	//
	//2Dオブジェクト作成
	sprite = new Sprite();
	sprite->Initialize(LeftTop[0], LeftBottom[0], RightTop[1], RightBottom[1]);
	worldTransform_Sprite.Initialize();
	//
	//リソースを作る
	//テクスチャ
	Texture = textureManager_->LoadTexture("resources/uvChecker.png");
	//サウンド
	mokugyo = audio->LoadAudio("resources/mokugyo.wav");
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
	Skydome_->Update();
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	player->Draw(viewProjection_);
	Skydome_->Draw(viewProjection_);
	//3Dモデル描画ここまで	

	//1. ビット演算を取り回しの良いUtilityクラスにする
	//2. 三項演算子をつかって、構造がわかりやすいようにする
	//piplineStatus->GetIsMesh2() ? mesh2->Draw() : ;
	//piplineStatus->GetIsSprite() ? sprite->Draw() : ;
	//piplineStatus->SetAllFlags();
	//void PipelineStatu::SetAllFlags() {
	//	bit = 0x111111111111.....;
	//}
	//void PipelineStatus::ResetAllFlags() {
		//Ethna
	//	bit = 0;
	//}

	//Sprite描画ここから


	//Sprite描画ここまで
	
	//描画ここまで
}
