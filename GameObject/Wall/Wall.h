#pragma once

#include "Base/Model.h"
#include "Base/WorldTransform.h"
#include "Base/ViewProjection.h"

#include "GameObject/BaseCharacter/BaseCharacter.h"
#include "GameObject/Player/Player.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Utility/CollisionConfig.h"

class Wall : public BaseCharacter, public BoxCollider
{
public:
	Wall();
	~Wall();

	void Initalize(const std::vector<Model*>& models, Vector3 position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void OnCollision(const Collider* collider)override;

private:

};