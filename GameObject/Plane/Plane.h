#pragma once

#include "Base/Model.h"
#include "Base/WorldTransform.h"
#include "Base/ViewProjection.h"

#include "GameObject/BaseCharacter/BaseCharacter.h"
#include "GameObject/Player/Player.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Utility/CollisionConfig.h"

class Plane : public BaseCharacter,public BoxCollider
{
public:
	Plane();
	~Plane();

	void Initalize(const std::vector<Model*>& models);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void BoxOnCollision(uint32_t collisionAttribute)override;

private:

};

