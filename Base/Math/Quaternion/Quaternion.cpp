#include "Quaternion.h"

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	Vector3 cross = Cross({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z });
	float dot = Dot({ lhs.x,lhs.y,lhs.z },{ rhs.x,rhs.y,rhs.z });
	result.x = cross.x + rhs.w * lhs.x + lhs.w * rhs.x;
	result.y = cross.y + rhs.w * lhs.y + lhs.w * rhs.y;
	result.z = cross.z + rhs.w * lhs.z + lhs.w * rhs.z;
	result.w = lhs.w * rhs.w - dot;

	return result;
}

Quaternion IdentityQuaternion()
{
	return Quaternion({0.0f,0.0f,0.0f,1.0f});
}

Quaternion Conjugate(const Quaternion& quaternion)
{
	return Quaternion({ quaternion.x*-1.0f,quaternion.y * -1.0f ,quaternion.z * -1.0f ,quaternion.w});
}

float Norm(const Quaternion& quaternion)
{
	return sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

Quaternion Normalize(const Quaternion& quaternion)
{
	Quaternion result{};
	float norm = Norm(quaternion);
	if (norm != 0.0f) {
		result.x = quaternion.x / norm;
		result.y = quaternion.y / norm;
		result.z = quaternion.z / norm;
		result.w = quaternion.w / norm;
	}
	return result;
}

Quaternion Inverse(const Quaternion& quaternion)
{
	Quaternion result;
	float norm = Norm(quaternion);
	norm = norm * norm;
	Quaternion conj = Conjugate(quaternion);
	if (norm != 0.0f) {
		result.x = conj.x / norm;
		result.y = conj.y / norm;
		result.z = conj.z / norm;
		result.w = conj.w / norm;
	}
	return result;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	Quaternion result;
	result.x = axis.x * sin(angle/2);
	result.y = axis.y * sin(angle/2);
	result.z = axis.z * sin(angle/2);
	result.w = cos(angle/2);
	return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion conj = Conjugate(quaternion);
	Vector3 result;
	result.x = quaternion.x * vector.x * conj.x;
	result.y = quaternion.y * vector.y * conj.y;
	result.z = quaternion.z * vector.z * conj.z;

	return result;
}
