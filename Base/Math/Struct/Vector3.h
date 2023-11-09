#pragma once
struct Vector3 final{
	float x;
	float y;
	float z;
	Vector3 operator +(Vector3 other) {
		Vector3 me;
		me.x = this->x + other.x;
		me.y = this->y + other.y;
		me.z = this->z + other.z;
			return me;
	}
};

