#pragma once
#include "GameObject/BaseCharacter/BaseCharacter.h"

class Enemy : public BaseCharacter,public BoxCollider
{
public:
	Enemy();
	~Enemy();
 
	void Initialize(const std::vector<Model*>& models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProjection)override;
	void OnCollision(uint32_t collisionAttribute)override;

private:
	void SetParent(const WorldTransform* parent);
	void SoulRotationGimmick();
	//各パーツのローカル座標
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformSoul_;

	bool IsAlive = true;
	const int kRespownTime = 120;
	int RespownTimeCount = 0;
};
