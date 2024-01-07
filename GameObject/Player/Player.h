#pragma once

#include <optional>
#include "GameObject/BaseCharacter/BaseCharacter.h"
#include "GameObject/Weapon/Weapon.h"
#include "Base/Math/Calc.h"
#include "Audio.h"

//前方宣言
class LockOn;
enum class Behavior {
	kRoot,//通常
	kAttack,//攻撃中
	kDash,//ダッシュ中
	kJump,//ジャンプ中
	kGrap,//つかみ
};
enum class GrapBehavior {
	kLeft,//左
	kRight//右
};
//ダッシュ用ワーク
struct WorkDash {
	//ダッシュ用の媒介変数
	uint32_t dashParameter_ = 0;
	//ダッシュ用スピード
	float dashSpeed_ = 5.0f;
};
//攻撃用定数
struct ConstAttack {
	//振りかぶりの時間
	uint32_t anticipationTime;
	//ための時間
	uint32_t chargeTime;
	//攻撃振りの時間
	uint32_t swingTime;
	//硬直時間
	uint32_t recoveryTime;
	//振りかぶりの移動の速さ
	float anticipationSpeed;	
	//ための移動の速さ
	float chargeSpeed;
	//攻撃振りの移動の速さ
	float swingSpeed;
};
//攻撃用ワーク
struct WorkAttack {
	//攻撃ギミック用
	uint32_t attackAnimeTimer_ = 0;
	int32_t comboIndex = 0;
	int32_t inComboPhase = 0;
	uint32_t anticipationTime = 0;
	bool comboNext = false;
};
enum audioHundle {
	Rotation,
	GrapJump,
};

class Player : public BaseCharacter , public BoxCollider
{
public:
	~Player() {};

	void Initialize(const std::vector<Model*>& models) override;
	void Update() override;
	void Draw(const ViewProjection& viewProjection) override;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	const WorldTransform& GetWorldTransform() {
		return worldTransform_;
	}

	void OnCollision(const Collider* collider)override;
	
	void SetParent(const WorldTransform* parent);
	void DeleteParent() {
		if (!worldTransform_.parent_) {
			return;
		}
		worldTransform_.translation_ = Add(worldTransform_.GetTranslateFromMatWorld(),{0.0f,0.0f,0.0f});
		worldTransform_.parent_ = nullptr;
	}
	//コンボの数
	static const int ComboNum = 3;
	//コンボ定数表
	static const std::array<ConstAttack, ComboNum> kConstAttacks_;

	void SetLockOn(LockOn* lockOn) { this->lockOn_ = lockOn; }

	bool GetIsAlive()const { return IsAlive; }
	void Reset() { IsAlive = true; }

	Behavior GetBehavior() const {
		return behavior_;
	};
	bool GetCanGrap()const {
		return canGrap;
	}

	void SetSound(std::vector<uint32_t>sound){
		Sound = sound;
	}

private:
	bool IsAlive = true;

	void WorldTransformInitalize();
	
	void Move();
	float targetAngle;
	
	//通常
	void BehaviorRootInit();
	void BehaviorRootUpdate();
	Vector3 move;
	//プレイヤーの移動
	float speed = 0.5f;
	//ジャンプ
	void BehaviorJumpInit();
	void BehaviorJumpUpdate();
	Vector3 jumpForce;
	//武器や腕の回転クォータニオン
	Quaternion moveQuaternion_;


	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	//キー入力とパッド
	Input* input = nullptr;
	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;
	//各パーツのローカル座標
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformArrow_;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	GlobalVariables* globalVariables = nullptr;

	//ロックオン
	const LockOn* lockOn_ = nullptr;

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
	float DownForce = 0.05f;

	Vector3 floorPos;

	//つかむ
		//ふるまい
	GrapBehavior GrapBehavior_ = GrapBehavior::kRight;
	//次のふるまいリクエスト
	std::optional<GrapBehavior> GrapBehaviorRequest_ = std::nullopt;
	void GrapInit();
	void GrapUpdate();
	void GrapJumpLeftInitalize();
	void GrapJumpLeftUpdate();
	void GrapJumpRightInitalize();
	void GrapJumpRightUpdate();
	Quaternion playerLock;
	bool canGrap = false;
	Vector3 grapPoint;
	Quaternion rotateQua;
	bool grapJump = false;
	uint32_t grapJumpAnime = 0;
	Vector3 grapJumpVec;
	Quaternion endVecQua;
	Quaternion beginVecQua;
	Quaternion lerpQua;
	float angle = 1.0f;
	float angleParam = 0.0f;
	Vector3 moveVector;
	float jumpPower = 0.0f;

	std::vector<uint32_t>Sound;
	float soundSpeed = 1.0f;
};

