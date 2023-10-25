#include "Plane.h"

Plane::Plane(){}
Plane::~Plane(){}

void Plane::Initalize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);

	BoxCollider::SetcollisionMask(~kCollitionAttributeFloor);
	BoxCollider::SetcollitionAttribute(kCollitionAttributeFloor);
	BoxCollider::SetParent(worldTransform_);
	BoxCollider::SetSize({ 10.0f,0.0f,10.0f });
}

void Plane::Update()
{
	BaseCharacter::Update();
	BoxCollider::Update(&worldTransform_);
}

void Plane::Draw(const ViewProjection& viewProjection)
{
	BaseCharacter::Draw(viewProjection);
}

void Plane::BoxOnCollision(uint32_t collisionAttribute)
{
	return;
	collisionAttribute;
}

