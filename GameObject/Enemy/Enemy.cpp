#include "Enemy.h"
#include "GameObject/Player/Player.h"
Enemy::Enemy(){}
Enemy::~Enemy(){}

void Enemy::Initialize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);
	BoxCollider::Initialize();
	worldTransformBody_.Initialize();
	worldTransformSoul_.Initialize();
	SetParent(&worldTransformBody_);
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransform_.UpdateMatrix();
	BoxCollider::SetcollisionMask(~kCollitionAttributeEnemy);
	BoxCollider::SetcollitionAttribute(kCollitionAttributeEnemy);

	BoxCollider::SetSize({ 1.0f,1.0f,1.0f });

}

void Enemy::Update()
{
	if (IsAlive == false) {
		BoxCollider::SetSize({ 0.0f,0.0f,0.0f });
	}

	if (IsHit) {
		if (t > 0.0f) {
			t -= 0.1f;
		}
		else if (t <= 0.0f) {
			IsAlive = false;
		}
		models_[kModelIndexBody]->SetColor({1.0f,1.0f,1.0f,t});
		models_[kModelIndexHead]->SetColor({1.0f,1.0f,1.0f,t});
	}
	
	// 自機のY軸周り角度(θy)
	Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, { 1.0f,0.0f,0.0f }));
	

	Quaternion ArmMovequaternion = MakeRotateAxisAngleQuaternion(cross,rotate_t);
	ArmMovequaternion = Normalize(ArmMovequaternion);
	
	worldTransform_.quaternion = ArmMovequaternion;
	
	SoulRotationGimmick();
	
	rotate_t += 0.01f;
	if (rotate_t >= 2.0f * (float)std::numbers::pi) {
		rotate_t = 0.0f;
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformSoul_.UpdateMatrix();
	BoxCollider::Update(&worldTransform_);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	if (IsAlive) {
		models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
		models_[kModelIndexHead]->Draw(worldTransformSoul_, viewProjection);
	}
}

void Enemy::OnCollision(const Collider* collider)
{
	//武器に当たった時
	if (collider->GetcollitionAttribute() == kCollitionAttributeWeapon) {
		worldTransform_.translation_.y += 0.2f;
		IsHit = true;
	}

	return;
}

void Enemy::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformSoul_.parent_ = parent;
}

void Enemy::SoulRotationGimmick()
{
	Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, { 1.0f,0.0f,0.0f }));

	Quaternion ArmMovequaternion = MakeRotateAxisAngleQuaternion(cross, rotate_t * 2.0f);
	ArmMovequaternion = Normalize(ArmMovequaternion);

	worldTransformSoul_.quaternion = ArmMovequaternion;
}
