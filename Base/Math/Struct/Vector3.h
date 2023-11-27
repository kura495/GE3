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
	Vector3* operator +=(Vector3 other) {
	
		this->x = this->x + other.x;
		this->y = this->y + other.y;
		this->z = this->z + other.z;
			return this;
	}
};

