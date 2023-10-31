#pragma once

#include "DirectXCommon.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Math/MatrixCalc.h"
#include "ModelData.h"

class Particle
{
public:
	Particle();
	~Particle();

	void Initalize(int particleVolume);

	void Update();

	void Draw();

private:
	ModelData modelData;

	DirectXCommon* directX_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource = nullptr;
	VertexData* vertexData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	Material* materialData = nullptr;

	//Transform transform_[];

	void CreateResources();
	void MakeVertexBufferView();
};

