#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Transform.h"
#include "Input.h"
#include "VectorCalc.h"
#include "GameObject/BaseCharacter/BaseCharacter.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Utility/CollisionConfig.h"

class Player : public BaseCharacter , public BoxCollider
{
public:
	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	const WorldTransform& GetWorldTransform() {
		return worldTransform_;
	}

	void BoxOnCollision(uint32_t collisionAttribute)override;
	
	void SetParent(const WorldTransform* parent) {
		// 親子関係を結ぶ
		worldTransform_.parent_ = parent;
	}
private:
	
	void ApplyGlobalVariables();
	void ImGui();
	//kamataEngine
	Input* input = nullptr;
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

	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	int floatcycle_ = 120;
	float floatingAmplitude_ = 0.2f;
};

