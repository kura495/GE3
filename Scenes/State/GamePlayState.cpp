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

	collisionManager_ = std::make_unique<CollisionManager>();
	//
	//3Dオブジェクト生成

#pragma region player
	player = std::make_unique<Player>();
	modelFighterBody_.reset(Model::CreateModelFromObj("resources/float_Body", "float_Body.obj"));
	modelFighterHead_.reset(Model::CreateModelFromObj("resources/float_Head", "float_Head.obj"));
	modelFighterL_arm_.reset(Model::CreateModelFromObj("resources/float_L_arm", "float_L_arm.obj"));
	modelFighterR_arm_.reset(Model::CreateModelFromObj("resources/float_R_arm", "float_R_arm.obj"));
	modelFighterWeapon.reset(Model::CreateModelFromObj("resources/weapon", "weapon.obj"));
	std::vector<Model*> playerModels = {
		modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),modelFighterR_arm_.get(),modelFighterWeapon.get()
	};
	player->Initialize(playerModels);
#pragma endregion

	Skydome_ = std::make_unique<Skydome>();
	Skydome_->Initalize();

#pragma region Planes

	model_plane_.reset(Model::CreateModelFromObj("resources/Plane", "Plane.obj"));
	std::vector<Model*> PlaneModels = {
		model_plane_.get() };
	for (uint32_t Volume_i = 0; Volume_i < 8; Volume_i++) {
		plane_[Volume_i] = std::make_unique<Plane>();
	}

	plane_[0]->Initalize(PlaneModels, {0.0f,0.0f,0.0f});
	plane_[1]->Initalize(PlaneModels, {0.0f, 0.0f, 30.0f});
	plane_[2]->Initalize(PlaneModels, {10.0f, 0.0f, 20.0f});
	plane_[3]->Initalize(PlaneModels, {10.0f, 0.0f, 10.0f});
	plane_[4]->Initalize(PlaneModels, {10.0f, 0.0f, 0.0f});
	plane_[5]->Initalize(PlaneModels, {20.0f, 0.0f, 20.0f});
	plane_[6]->Initalize(PlaneModels, {20.0f, 0.0f, 10.0f});
	plane_[7]->Initalize(PlaneModels, {20.0f, 0.0f, 0.0f});

	model_plane_Move_.reset(Model::CreateModelFromObj("resources/Plane", "MovePlane.obj"));
	std::vector<Model*> Plane_Move_Models = {
		model_plane_Move_.get() };
	plane_Move_ = std::make_unique<MovePlane>();
	plane_Move_->Initalize(Plane_Move_Models);
	plane_Move_->SetPlayer(player.get());

#pragma endregion

	model_wall_	.reset(Model::CreateModelFromObj("resources/Wall", "Wall.obj"));
	std::vector<Model*> wall_Models = {
		model_wall_.get() };
	wall_ = std::make_unique<Wall>();
	wall_->Initalize(wall_Models,{10.0f,5.0f,25.0f});

	viewProjection_.Initialize();
	worldTransform_.Initialize();
	followCamera = std::make_unique<FollowCamera>();
	followCamera->Initalize();
	followCamera->SetTarget(&player->GetWorldTransform());
	
	player->SetViewProjection(&followCamera->GetViewProjection());

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

	player->Update();
	
	
	Skydome_->Update();
	for (uint32_t Volume_i = 0; Volume_i < 8; Volume_i++) {
		plane_[Volume_i]->Update();
	}
	plane_Move_->Update();

	followCamera->Update();
	viewProjection_ = followCamera->GetViewProjection();


	viewProjection_.UpdateMatrix();
	
	wall_->Update();
	
	collisionManager_->AddBoxCollider(player.get());
	
	for (uint32_t Volume_i = 0; Volume_i < 8; Volume_i++) {
		collisionManager_->AddBoxCollider(plane_[Volume_i].get());		
	}
	collisionManager_->AddBoxCollider(plane_Move_.get());
	collisionManager_->AddBoxCollider(wall_.get());
	collisionManager_->AddBoxCollider(player->GetWeapon());
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();

}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	player->Draw(viewProjection_);
	Skydome_->Draw(viewProjection_);
	for (uint32_t Volume_i = 0; Volume_i < 8; Volume_i++) {
		plane_[Volume_i]->Draw(viewProjection_);
	}
	plane_Move_->Draw(viewProjection_);
	wall_->Draw(viewProjection_);

	//3Dモデル描画ここまで	

	//Sprite描画ここから


	//Sprite描画ここまで
	
	//描画ここまで
}
