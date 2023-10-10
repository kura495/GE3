#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Transform.h"
#include "Input.h"
#include "VectorCalc.h"

enum modelNumber {
	kModelIndexBody,
	kModelIndexHead,
	kModelIndexL_arm,
	kModelIndexR_arm,
};
class Player
{
public:
	void Initialize(std::vector<Model*> models);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	const WorldTransform& GetWorldTransform() {
		return worldTransform_;
	}

private:
	void SetParent(const WorldTransform* parent);
	void ApplyGlobalVariables();
	void ImGui();
	//kamataEngine
	Input* input = nullptr;
	std::vector<Model*> models_;
	WorldTransform worldTransform_;
	//各パーツのローカル座標
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	GlobalVariables* globalVariables = nullptr;
	
	float speed = 0.3f;
	XINPUT_STATE joyState;
};

