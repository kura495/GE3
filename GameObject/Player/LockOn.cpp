#include "LockOn.h"


void LockOn::Initalize()
{

	Texture_ = TextureManager::GetInstance()->LoadTexture("resources/reticle.png");
	lockOnMark_ = std::make_unique<Sprite>();
	lockOnMark_->Initialize({-1.0f,1.0f,0.0f,0.0f},{-1.0f,-1.0f,0.0f,0.0f},{1.0f,1.0f,0.0f,0.0f},{1.0f,-1.0f,0.0f,0.0f});
	world_.Initialize();

}

void LockOn::Update(const std::list<Enemy*>& enemies,const ViewProjection& viewProjection)
{
	return;
	enemies;
	viewProjection;
}

void LockOn::Draw()
{
	if (target_) {
		lockOnMark_->Draw(world_, Texture_);
	}
}
