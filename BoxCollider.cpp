#include "BoxCollider.h"

BoxCollider::BoxCollider(){}
BoxCollider::~BoxCollider(){}

void BoxCollider::Update()
{
	//�������|�C���g�̐ݒ�
	aabb_.min.x = world_.translation_.x - range_.x / 2;
	aabb_.min.y = world_.translation_.y - range_.y / 2;
	aabb_.min.z = world_.translation_.z - range_.z / 2;
	//�傫���|�C���g�̐ݒ�
	aabb_.min.x = world_.translation_.x + range_.x / 2;
	aabb_.min.y = world_.translation_.y + range_.y / 2;
	aabb_.min.z = world_.translation_.z + range_.z / 2;
}

