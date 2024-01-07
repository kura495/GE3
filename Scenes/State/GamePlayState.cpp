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
	modelFighterBody_.reset(Model::CreateModelFromObj("resources/SeaHorse", "SeaHorse.obj"));
	modelFighterArrow_.reset(Model::CreateModelFromObj("resources/Line", "Line.obj"));
	std::vector<Model*> playerModels = {
		modelFighterBody_.get(),modelFighterArrow_.get()
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
	
	model_Sango_.reset(Model::CreateModelFromObj("resources/sango", "sango.obj"));
	std::vector<Model*> Sango_Models = {
		model_Sango_.get() };
	sango_ = std::make_unique<Sango>();
	sango_->Initalize(Sango_Models,{10.0f,2.0f,25.0f});

#pragma region Sprite
	inst1_texture = textureManager_->LoadTexture("resources/inst.png");
	inst1_world_.Initialize();
	inst1_world_.translation_.y = 600.0f;
	inst1_world_.UpdateMatrix();
	inst1 = std::make_unique<Sprite>();
	inst1->Initialize({ 0.0f,0.0f,0.0f,1.0f }, { 0.0f,104.0f,0.0f,1.0f }, { 1280.0f,0.0f,0.0f,1.0f }, { 1280.0f,104.0f,0.0f,1.0f });

	inst2_texture = textureManager_->LoadTexture("resources/inst2.png");
	inst2_world_.Initialize();
	inst2_world_.translation_.y = 540.0f;
	inst2_world_.UpdateMatrix();
	inst2 = std::make_unique<Sprite>();
	inst2->Initialize({ 0.0f,0.0f,0.0f,1.0f }, { 0.0f,160.0f,0.0f,1.0f }, { 470.0f,0.0f,0.0f,1.0f }, { 470.0f,160.0f,0.0f,1.0f });
	
	inst3_texture = textureManager_->LoadTexture("resources/inst3.png");
	inst3_world_.Initialize();
	inst3_world_.translation_.x = 650.0f;
	inst3_world_.translation_.y = 250.0f;
	inst3_world_.UpdateMatrix();
	inst3 = std::make_unique<Sprite>();
	inst3->Initialize({ 0.0f,0.0f,0.0f,1.0f }, { 0.0f,80.0f,0.0f,1.0f }, { 250.0f,0.0f,0.0f,1.0f }, { 250.0f,80.0f,0.0f,1.0f });
#pragma endregion スプライト

	RotationSoundHundle = audio->LoadAudio("resources/Rotation.wav");
	GrapJumpSoundHundle = audio->LoadAudio("resources/GrapJump.wav");
	std::vector<uint32_t>sound{ RotationSoundHundle,GrapJumpSoundHundle };
	player->SetSound(sound);

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
	sango_->Update();
	
	collisionManager_->AddBoxCollider(player.get());
	
	for (uint32_t Volume_i = 0; Volume_i < 8; Volume_i++) {
		collisionManager_->AddBoxCollider(plane_[Volume_i].get());
	}
	collisionManager_->AddBoxCollider(plane_Move_.get());
	collisionManager_->AddBoxCollider(wall_.get());
	collisionManager_->AddBoxCollider(sango_.get());
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();
}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	wall_->Draw(viewProjection_);
	sango_->Draw(viewProjection_);
	player->Draw(viewProjection_);
	Skydome_->Draw(viewProjection_);
	for (uint32_t Volume_i = 0; Volume_i < 8; Volume_i++) {
		plane_[Volume_i]->Draw(viewProjection_);
	}
	plane_Move_->Draw(viewProjection_);



	//3Dモデル描画ここまで	

	//Sprite描画ここから
	if (player->GetBehavior() != Behavior::kGrap) {
		inst1->Draw(inst1_world_, inst1_texture);
	}
	else {
		inst2->Draw(inst2_world_, inst2_texture);
	}
	if (player->GetCanGrap() == true && player->GetBehavior() != Behavior::kGrap) {
		inst3->Draw(inst3_world_, inst3_texture);
	}


	//Sprite描画ここまで
	
	//描画ここまで
}
