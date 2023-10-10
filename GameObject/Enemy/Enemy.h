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

};
