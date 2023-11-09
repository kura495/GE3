#pragma once
#include "Base/Math/VectorCalc.h"
#include "Base/Math/MatrixCalc.h"

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

Quaternion Multiply(const Quaternion& lhs, const Quaternion&rhs);