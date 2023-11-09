#include "Enemy.h"

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
	worldTransform_.translation_ = { 10.0f, 0.0f, 20.0f
	};
	worldTransform_.UpdateMatrix();
	BoxCollider::SetcollisionMask(~kCollitionAttributeEnemy);
	BoxCollider::SetcollitionAttribute(kCollitionAttributeEnemy);

	BoxCollider::SetSize({ 2.0f,2.0f,2.0f });

}

void Enemy::Update()
{
	if (IsAlive == false) {
		BoxCollider::SetSize({ 0.0f,0.0f,0.0f });
		RespownTimeCount++;
		if (RespownTimeCount == kRespownTime) {
			RespownTimeCount = 0;
			BoxCollider::SetSize({ 2.0f,2.0f,2.0f });
			IsAlive = true;
		}
	}
	//IsAlive = true;
	// 速さ
	const float kSpeed = 0.01f;
	Vector3 velocity{ 0.0f, 0.0f, kSpeed };

	// 移動ベクトルをカメラの角度だけ回転
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 移動量
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
	// 自機のY軸周り角度(θy)
	worldTransform_.rotation_.y -= 0.03f;
	
	SoulRotationGimmick();

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

void Enemy::OnCollision(uint32_t collisionAttribute)
{
	//プレイヤーと当たった時は何もしない
	if (collisionAttribute == kCollitionAttributeWeapon) {
		IsAlive = false;
	}
	return;
}

void Enemy::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformSoul_.parent_ = parent;
}

void Enemy::SoulRotationGimmick()
{
	// 速さ
	const float kSpeed = 1.5f;
	Vector3 velocity{ 0.0f, 0.0f, kSpeed };

	// 移動ベクトルをカメラの角度だけ回転
	velocity = TransformNormal(velocity, worldTransformBody_.matWorld_);
	worldTransformSoul_.translation_ = Add(worldTransformBody_.translation_, velocity);
	worldTransformSoul_.rotation_.y += 0.03f;
}
