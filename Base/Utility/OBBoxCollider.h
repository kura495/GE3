#pragma once

#include "Collider.h"

struct OBB {
	Vector3 center;//中心点の座標
	Vector3 orientations[3];//XYZの各座標軸の傾きを表す方向ベクトル
	Vector3 size;//中心から面までの距離
};

class OBBoxCollider : public Collider
{
public:
	void Initalize();

	void SetSize(Vector3 size) {obb_.size = size;}
	void SetCenter(Vector3 CenterPoint) { obb_.center = CenterPoint; }
	void SetOrientations(Matrix4x4 Matrix);
	void OBBIndex(const OBB& obb, std::vector<Vector3>& outputvertices);
	bool SeparationAxis(const Vector3 axis, const OBB obb_1, const OBB obb_2);
	Vector3 GetOrientations(uint32_t num) { return obb_.orientations[num]; }
private:

	OBB obb_;



};