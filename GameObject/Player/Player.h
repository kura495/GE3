#pragma once

#include <optional>
#include "GameObject/BaseCharacter/BaseCharacter.h"


enum class Behavior {
	kRoot,//通常
	kAttack,//攻撃中
};

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
	
	void SetParent(const WorldTransform* parent);
	void DeleteParent() {
		if (!worldTransform_.parent_) {
			return;
		}
		worldTransform_.translation_ = Add(worldTransform_.GetTranslateFromMatWorld(),{0.0f,0.0f,0.0f});
		worldTransform_.parent_ = nullptr; }
private:


	void WorldTransformInitalize();
	
	void Move();
	
	void ApplyGlobalVariables();
	void ImGui();
	//通常
	void BehaviorRootInit();
	void BehaviorRootUpdate();
	//攻撃
	void BehaviorAttackInit();
	void BehaviorAttackUpdate();
	int attackAnimationFrame;
	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//キー入力とパッド
	Input* input = nullptr;
	XINPUT_STATE joyState;
	//各パーツのローカル座標
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransform_Weapon_;
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	GlobalVariables* globalVariables = nullptr;
	
	float speed = 0.3f;
	

	//浮遊ギミック
	void InitializeFloatingGimmick();
	void UpdateFloatingGimmick();
	//浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;
	int floatcycle_ = 120;
	float floatingAmplitude_ = 0.2f;

	//プレイヤーを下に引っ張る
	void PullDown();
	bool IsOnGraund = true;
	float DownForce = 0.98f;
	
};

