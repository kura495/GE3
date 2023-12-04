#pragma once

#include "Base/ViewProjection.h"
#include "Base/Sprite.h"
#include "GameObject/Enemy/Enemy.h"
#include "Base/TextureManager.h"
#include "Base/Input/Input.h"

class LockOn
{
public:
	void Initalize();
	void Update(const std::list<Enemy*>& enemies, const ViewProjection& viewProjection);
	void Draw();

	Vector3 GetTargetPosition()const;
	bool ExistTarget()const { return target_ ? true : false; }

private:
	void search(const std::list<Enemy*>& enemies, const ViewProjection& viewProjection);
	Vector3 WorldToScreen(Vector3& position, const ViewProjection& viewProjection);
	bool ChackOnLockOnRenge(const ViewProjection& viewProjection);

	Input* input = nullptr;
	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	std::unique_ptr<Sprite> lockOnMark_;
	const Enemy* target_ = nullptr;
	WorldTransform world_;
	int Texture_;
	float kDegreeToRadian = (float)std::numbers::pi / 180;
	float minDistance_ = 10.0f;
	float maxDistance_ = 30.0f;
	float angleRange_ = 20.0f * kDegreeToRadian;
};