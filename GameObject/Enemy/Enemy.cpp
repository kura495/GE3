#include "Enemy.h"

Enemy::Enemy(){}
Enemy::~Enemy(){}

void Enemy::Initialize(const std::vector<Model*>& models)
{
	BaseCharacter::Initialize(models);
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	SetParent(&worldTransformBody_);
	worldTransformBody_.parent_ = &worldTransform_;

}

void Enemy::Update()
{
	// 速さ
	const float kSpeed = 0.3f;
	Vector3 velocity{ 0.0f, 0.0f, kSpeed };

	// 移動ベクトルをカメラの角度だけ回転
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 移動量
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
	// 自機のY軸周り角度(θy)
	worldTransform_.rotation_.y += 0.03f;

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

void Enemy::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransformHead_.parent_ = parent;
	worldTransformL_arm_.parent_ = parent;
	worldTransformR_arm_.parent_ = parent;
}