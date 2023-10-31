#pragma once

#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Math/MatrixCalc.h"
#include "ModelData.h"

#include "Utility/ImGuiManager.h"

class Particle
{
public:
	Particle();
	~Particle();

	void Initalize(int particleVolume);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	ModelData modelData;

	DirectXCommon* directX_ = nullptr;
	TextureManager* textureManager_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource = nullptr;
	VertexData* vertexData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	Material* materialData = nullptr;

	//パーティクルの数
	int particleVolume_;
	//パーティクルの数分のtransform
	std::vector<WorldTransform> transform_;

	void CreateResources();


};

