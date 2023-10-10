#include "Enemy.h"

Enemy::Enemy(){}
Enemy::~Enemy(){}

void Enemy::Initialize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);


}

void Enemy::Update()
{
	BaseCharacter::Update();

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	

}
