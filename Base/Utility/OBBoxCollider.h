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
	void Update();

private:


};