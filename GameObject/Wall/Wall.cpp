#include "Wall.h"

Wall::Wall() {}
Wall::~Wall() {}

void Wall::Initalize(const std::vector<Model*>& models, Vector3 position)
{
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	BoxCollider::SetcollisionMask(~kCollitionAttributeFloor);
	BoxCollider::SetcollitionAttribute(kCollitionAttributeFloor);
	BoxCollider::SetParent(worldTransform_);
	BoxCollider::SetSize({ 10.0f,10.0f,0.0f });
}

void Wall::Update()
{
	BaseCharacter::Update();
	BoxCollider::Update(&worldTransform_);
}

void Wall::Draw(const ViewProjection& viewProjection)
{
	BaseCharacter::Draw(viewProjection);
}

void Wall::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributePlayer) {
		return;
	}

}