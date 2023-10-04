#pragma once

#include "ModelData.h"

class Particle
{
public:
	Particle();
	~Particle();

	void Initalize();

	void Update();

	void Draw();

private:
	ModelData modelData;
};

