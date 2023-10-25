#pragma once

#include "Base/Math/Struct/Vector3.h"
#include "Base/WorldTransform.h"


struct AABB {
	Vector3 min;//一番小さいポイント
	Vector3 max;//一番大きいポイント
};
class BoxCollider
{
public:
	BoxCollider();
	~BoxCollider();
	
	//void Initalize();
	void Update();
	//void Draw();

	///worldの親を設定
	void SetParent(const WorldTransform* Parent) { world_.parent_ = Parent; }
	
	void SetSize(Vector3 range) { range_ = range; }
	AABB GetSize() { return aabb_; }
	uint32_t GetcollitionAttribute() const { return collisionAttribute_; }
	/// <summary>
	/// 衝突属性
	/// </summary>
	/// <param name="collisionAttribute"></param>
	void SetcollitionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	uint32_t GetcollisionMask() const { return collisionMask_; }
	/// <summary>
	/// 衝突マスク
	/// </summary>
	/// <param name="collisionMask">基本はSetcollitionAttributeに入れたものに~を付けてビット反転</param>
	void SetcollisionMask(uint32_t collisionMask) {

		collisionMask_ = collisionMask;
	}

private:
	/// <summary>
	/// x,y,zそれぞれの幅
	/// </summary>
	Vector3 range_;
	/// <summary>
	/// 箱の中心
	/// </summary>
	WorldTransform world_;
	/// <summary>
	/// 箱の大きさ
	/// </summary>
	AABB aabb_ = {};
	/// <summary>
/// 衝突属性(自分)
/// </summary>
	uint32_t collisionAttribute_ = 0xffffffff;
	/// <summary>
	/// 衝突マスク(相手)
	/// </summary>
	uint32_t collisionMask_ = 0xffffffff;
};
