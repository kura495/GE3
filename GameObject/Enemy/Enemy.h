﻿#pragma once
#include "GameObject/BaseCharacter/BaseCharacter.h"

class Enemy : public BaseCharacter,public BoxCollider
{
public:
	Enemy();
	~Enemy();
 
	void Initialize(const std::vector<Model*>& models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProjection)override;
	void OnCollision(const Collider* collider)override;
	void SetPos(Vector3 pos) { worldTransform_.translation_ = pos; }
	Vector3 GetPos() const {
	return	{ worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1]
			, worldTransform_.matWorld_.m[3][2]};
	}

private:
	void SetParent(const WorldTransform* parent);
	void SoulRotationGimmick();
	//各パーツのローカル座標
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformSoul_;

	bool IsAlive = true;
	const int kRespownTime = 120;
	int RespownTimeCount = 0;

	//回転の媒介変数
	float rotate_t = 0.0f;
};
