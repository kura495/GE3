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
	textureManager_ = TextureManager::GetInstance();

	particleVolume_ = particleVolume;

	for (int Volume_i = 0; Volume_i < particleVolume_;Volume_i++) {
		WorldTransform World_;
		World_.Initialize();
		World_.matWorld_ = CreateIdentity4x4();
		World_.UpdateMatrix();
		transform_.push_back(World_);
	}
	CreateResources();
	materialData->enableLighting = false;
	materialData->color = {1.0f,1.0f,1.0f,0.0f};
	materialData->uvTransform = CreateIdentity4x4();
}

void Particle::Update()
{
	ImGui::Begin("Particle");
	ImGui::SliderFloat3("rotate",&transform_[0].rotation_.x, -1, 1);
	transform_[0].UpdateMatrix();
	ImGui::End();
}

void Particle::Draw(const ViewProjection& viewProjection)
{
	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	
	//ViewProjection
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(4, viewProjection.constBuff_->GetGPUVirtualAddress());
	//色とuvTransform
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//テクスチャ
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(modelData.TextureIndex));

	for (int Volume_i = 0; Volume_i < particleVolume_; Volume_i++) {
		//WorldTransform
		directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transform_[Volume_i].constBuff_->GetGPUVirtualAddress());

		directX_->GetcommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
	}
	
}

void Particle::CreateResources()
{
	//CreateVertexResource
	vertexResource = directX_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	//CreateVertexbufferView
	vertexBufferView.BufferLocation = vertexResource.Get()->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//maping vertexResource
	vertexResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
	//CreatematerialResource
	materialResource = directX_->CreateBufferResource(sizeof(Material));
	//maping materialResource
	materialResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData));



}

