#include "Skydome.h"

Skydome::Skydome(){}
Skydome::~Skydome(){}

void Skydome::Initalize()
{
	//TODO ���f���̓ǂݍ��݂���������
	model_.reset(Model::CreateModelFromObj("resources/skydome","skydome.obj"));
	worldTransform_.Initialize();
}

void Skydome::Update()
{
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection ViewProjection_)
{
	model_.get()->Draw(worldTransform_, ViewProjection_);
}
