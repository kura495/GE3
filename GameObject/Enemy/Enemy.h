#pragma once
#include "GameObject/BaseCharacter/BaseCharacter.h"
#include "VectorCalc.h"

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
	WorldTransform worldTransformSoul_;
};
