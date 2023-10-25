#include "MovePlane.h"

MovePlane::MovePlane(){}
MovePlane::~MovePlane(){}

void MovePlane::Initalize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);

	BoxCollider::SetcollisionMask(~kCollitionAttributeFloor);
	BoxCollider::SetcollitionAttribute(kCollitionAttributeFloor);
	BoxCollider::SetParent(worldTransform_);
	BoxCollider::SetSize({ 10.0f,0.0f,10.0f });
}

void MovePlane::Update()
{
	BaseCharacter::Update();
	BoxCollider::Update(worldTransform_);
}

void MovePlane::Draw(const ViewProjection& viewProjection)
{
	BaseCharacter::Draw(viewProjection);
}

void MovePlane::BoxOnCollision(uint32_t collisionAttribute)
{
	if (collisionAttribute == kCollitionAttributePlayer) {
		player_.SetParent(&worldTransform_);
	}
	return;
}

void MovePlane::LinearMoveGimmick()
{	
	T += kspeedOfT;
	if (T >= 1.0f) {
		linearMoveFlag = false;
	}
	else if (T <= 0.0f) {
		linearMoveFlag = false;
	}

	if (linearMoveFlag == true){
		worldTransform_.translation_ = VectorLerp(StartPoint,EndPoint,T);
	}
	else if (linearMoveFlag == false) {
		AnimeFlame++;
		if (AnimeFlame >= 60.0f) {
			linearMoveFlag = true;
			AnimeFlame = 0.0f;
		}
	}

}
