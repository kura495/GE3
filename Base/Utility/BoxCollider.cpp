#include "BoxCollider.h"

void BoxCollider::Initalize()
{
	world_.Initialize();
}

void BoxCollider::Update(const WorldTransform& world)
{
	//小さいポイントの設定
	aabb_.min.x = world.translation_.x - range_.x / 2;
	aabb_.min.y = world.translation_.y - range_.y / 2;
	aabb_.min.z = world.translation_.z - range_.z / 2;
	//大きいポイントの設定
	aabb_.max.x = world.translation_.x + range_.x / 2;
	aabb_.max.y = world.translation_.y + range_.y / 2;
	aabb_.max.z = world.translation_.z + range_.z / 2;
}

