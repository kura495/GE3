#include "Particle.h"
Particle::Particle(){}
Particle::~Particle(){}

void Particle::Initalize(int particleVolume)
{
	//TODO　パーティクルシステム作る
	modelData.vertices.push_back({ .position = { 1.0f,1.0f,0.0f,1.0f },.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
	modelData.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},  .texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左上
	modelData.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//右下
	modelData.material.textureFilePath = ".resources/uvChecker.png";

	directX_ = DirectXCommon::GetInstance();
	particleVolume;
	/*for () {
		transform_[] = CreateIdentity4x4();
	}*/
}

void Particle::Update()
{

}

void Particle::Draw()
{
	
	directX_->GetcommandList()->DrawInstanced(3, 1, 0, 0);
}

void Particle::CreateResources()
{
	vertexResource = directX_->CreateBufferResource(sizeof(VertexData) * 3);
	MakeVertexBufferView();
	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

}

void Particle::MakeVertexBufferView()
{
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}
