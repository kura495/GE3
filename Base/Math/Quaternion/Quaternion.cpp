#include "Quaternion.h"

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	float dot = Dot({ lhs.x,lhs.y,lhs.z },{ rhs.x,rhs.y,rhs.z });
	result.x = lhs.y * rhs.z - lhs.z * rhs.y + lhs.w * rhs.x + lhs.w * rhs.x;
	result.y = lhs.z * rhs.x - lhs.x * rhs.z + lhs.w * rhs.y + lhs.w * rhs.y;
	result.z = lhs.x * rhs.y - lhs.y * rhs.x + lhs.w * rhs.z + lhs.w * rhs.z;
	result.w = lhs.w * rhs.w - dot;

	return result;
}
