#include "Sango.h"

Sango::Sango() {}
Sango::~Sango() {}

void Sango::Initalize(const std::vector<Model*>& models, Vector3 position)
{
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	BoxCollider::SetcollisionMask(~kCollitionAttributeSango);
	BoxCollider::SetcollitionAttribute(kCollitionAttributeSango);
	BoxCollider::SetParent(worldTransform_);
	BoxCollider::SetSize({ 10.0f,10.0f,10.0f });
}

void Sango::Update()
{
	BaseCharacter::Update();
	BoxCollider::Update(&worldTransform_);
}

void Sango::Draw(const ViewProjection& viewProjection)
{
	BaseCharacter::Draw(viewProjection);
}

void Sango::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributePlayer) {

		return;
	}

}