#include "Enemy.h"

Enemy::Enemy(){}
Enemy::~Enemy(){}

void Enemy::Initialize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);
	BoxCollider::Initalize();
	worldTransformBody_.Initialize();
	worldTransformSoul_.Initialize();
	SetParent(&worldTransformBody_);
	worldTransformBody_.parent_ = &worldTransform_;

	BoxCollider::SetcollisionMask(~kCollitionAttributeEnemy);
	BoxCollider::SetcollitionAttribute(kCollitionAttributeEnemy);
	BoxCollider::SetParent(worldTransform_);
	BoxCollider::SetSize({ 1.0f,1.0f,1.0f });

}

void Enemy::Update()
{
	// 速さ
	const float kSpeed = 0.1f;
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
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformSoul_, viewProjection);
}

void Enemy::BoxOnCollision(uint32_t collisionAttribute)
{
	//プレイヤーと当たった時は何もしない
	if (collisionAttribute == kCollitionAttributePlayer) {
		return;
	}
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
