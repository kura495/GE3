#pragma once

#include "Collider.h"

class OBBoxCollider : public Collider
{
public:
	void Initalize();
	void Update();

private:

};

OBBoxCollider::OBBoxCollider()
{
}

OBBoxCollider::~OBBoxCollider()
{
}