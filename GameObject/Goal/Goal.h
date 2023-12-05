#pragma once

#include "GameObject/BaseCharacter/BaseCharacter.h"

class Goal : public BaseCharacter, public BoxCollider
{
public:
	Goal();
	~Goal();

	void Initalize(const std::vector<Model*>& models);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void OnCollision(const Collider* collider)override;

private:

	

};

