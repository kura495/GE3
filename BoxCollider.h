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
	
	void SetSize(Vector3 range) { range_ = range; }
	AABB GetSize() { return aabb_; }

private:
	Vector3 range_;
	WorldTransform world_;
	AABB aabb_ = {};
};
