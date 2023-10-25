#include "BoxCollider.h"

void BoxCollider::Initalize()
{
	world_.Initialize();
}

void BoxCollider::Update(const WorldTransform* world)
{
	//小さいポイントの設定
	aabb_.min.x = world->GetTranslateFromMatWorld().x - range_.x / 2;
	aabb_.min.y = world->GetTranslateFromMatWorld().y - range_.y / 2;
	aabb_.min.z = world->GetTranslateFromMatWorld().z - range_.z / 2;
	//大きいポイントの設定
	aabb_.max.x = world->GetTranslateFromMatWorld().x + range_.x / 2;
	aabb_.max.y = world->GetTranslateFromMatWorld().y + range_.y / 2;
	aabb_.max.z = world->GetTranslateFromMatWorld().z + range_.z / 2;
}

