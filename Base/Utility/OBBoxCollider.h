#pragma once

#include "Collider.h"

struct OBB {
	Vector3 center;//���S�_�̍��W
	Vector3 orientations[3];//XYZ�̊e���W���̌X����\�������x�N�g��
	Vector3 size;//���S����ʂ܂ł̋���
};

class OBBoxCollider : public Collider
{
public:
	void Initalize();
	void Update();

private:


};