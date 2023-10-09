#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Transform.h"
#include "Input.h"
#include "VectorCalc.h"

class Player
{
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	const WorldTransform& GetWorldTransform() {
		return worldTransform_;
	}

private:
	void ApplyGlobalVariables();
	void ImGui();
	//kamataEngine
	Input* input = nullptr;
	Model* model = nullptr;
	WorldTransform worldTransform_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	GlobalVariables* globalVariables = nullptr;
	
	float speed = 0.3f;
	XINPUT_STATE joyState;
};

