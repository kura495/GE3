#pragma once

#include "Base/Model.h"
#include "Base/WorldTransform.h"
#include "Base/ViewProjection.h"

#include "Base/Math/VectorCalc.h"

#include "GameObject/BaseCharacter/BaseCharacter.h"
#include "GameObject/Player/Player.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Utility/CollisionConfig.h"

class MovePlane : public BaseCharacter, public BoxCollider
{
public:
	MovePlane();
	~MovePlane();

	void Initalize(const std::vector<Model*>& models);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void BoxOnCollision(uint32_t collisionAttribute)override;

	void LinearMoveGimmick();

	void SetPlayer(const Player& player) {
		player_ = player;
	}

private:
	Player player_;
	float T = 0.0f;
	float kspeedOfT = 0.1f;
	bool linearMoveFlag = true;
	float AnimeFlame = 0.0f;
	Vector3 StartPoint = {
		0.0f,0.0f,10.0f
	};
	
	Vector3 EndPoint = {
		0.0f,0.0f,20.0f
	};

};


