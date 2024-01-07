#include "Goal.h"
Goal::Goal(){}
Goal::~Goal(){}

void Goal::Initalize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = { 20.0f,5.0f,10.0f };
	BoxCollider::Initialize();
	BoxCollider::SetSize({1.0f,1.0f,1.0f});
	BoxCollider::SetcollitionAttribute(kCollitionAttributeGoal);
	BoxCollider::SetcollisionMask(~kCollitionAttributeGoal);
	BoxCollider::SetParent(worldTransform_);
}

void Goal::Update()
{
	BaseCharacter::Update();
	BoxCollider::Update(&worldTransform_);
}

void Goal::Draw(const ViewProjection& viewProjection)
{
	BaseCharacter::Draw(viewProjection);
}

void Goal::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute()) {
		return;
	}
}

