#pragma once
#include "GameObject/BaseCharacter/BaseCharacter.h"

class Enemy : public BaseCharacter
{
public:
	Enemy();
	~Enemy();

	void Initialize(const std::vector<Model*>& models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProjection)override;

private:
	void SetParent(const WorldTransform* parent);
	//各パーツのローカル座標
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
};
