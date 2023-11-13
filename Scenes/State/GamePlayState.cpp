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

#pragma region enemy
	enemy_ = std::make_unique<Enemy>();
	modelEnemyBody_.reset(Model::CreateModelFromObj("resources/Enemy", "Enemy_Body.obj"));
	modelEnemy_Soul_.reset(Model::CreateModelFromObj("resources/Enemy", "Enemy_Soul.obj"));
	std::vector<Model*> EnemyModels = {
		modelEnemyBody_.get(),modelEnemy_Soul_.get() };
	enemy_->Initialize(EnemyModels);
#pragma endregion

	Skydome_ = std::make_unique<Skydome>();
	Skydome_->Initalize();

#pragma region Planes

	model_plane_.reset(Model::CreateModelFromObj("resources/Plane", "Plane.obj"));
	std::vector<Model*> PlaneModels = {
		model_plane_.get() };
	plane_ = std::make_unique<Plane>();
	plane_->Initalize(PlaneModels, { 0.0f,0.0f,0.0f });

	plane_2 = std::make_unique<Plane>();
	plane_2->Initalize(PlaneModels, {0.0f, 0.0f, 30.0f});
	plane_3 = std::make_unique<Plane>();
	plane_3->Initalize(PlaneModels, {10.0f, 0.0f, 20.0f});

	model_plane_Move_.reset(Model::CreateModelFromObj("resources/Plane", "MovePlane.obj"));
	std::vector<Model*> Plane_Move_Models = {
		model_plane_Move_.get() };
	plane_Move_ = std::make_unique<MovePlane>();
	plane_Move_->Initalize(Plane_Move_Models);
	plane_Move_->SetPlayer(player.get());

#pragma endregion

	model_goal_.reset(Model::CreateModelFromObj("resources/Cube", "Cube.obj"));
	std::vector<Model*> model_goal_Models = {
		model_goal_.get() };
	goal = std::make_unique<Goal>();
	goal->Initalize(model_goal_Models);


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
	enemy_->Update();
	Skydome_->Update();
	plane_->Update();
	plane_2->Update();
	plane_3->Update();
	plane_Move_->Update();
	goal->Update();

	GlobalVariables::GetInstance()->Update();
	followCamera->Update();
	viewProjection_ = followCamera->GetViewProjection();


	viewProjection_.UpdateMatrix();
	
	
	
	collisionManager_->AddCollider(player.get());
	collisionManager_->AddCollider(enemy_.get());
	collisionManager_->AddCollider(plane_.get());
	collisionManager_->AddCollider(plane_2.get());
	collisionManager_->AddCollider(plane_3.get());
	collisionManager_->AddCollider(plane_Move_.get());
	collisionManager_->AddCollider(goal.get());
	collisionManager_->AddCollider(player->GetWeapon());
	collisionManager_->CheckAllCollisions();
	collisionManager_->ClearCollider();

}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから
	player->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	Skydome_->Draw(viewProjection_);
	plane_->Draw(viewProjection_);
	plane_2->Draw(viewProjection_);
	plane_3->Draw(viewProjection_);
	plane_Move_->Draw(viewProjection_);
	goal->Draw(viewProjection_);
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
