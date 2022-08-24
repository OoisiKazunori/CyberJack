#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"

class MeshParticleEmitter
{
public:
	MeshParticleEmitter();
	~MeshParticleEmitter();
	void Init(const KazMath::Vec3<float> &POS);
	void Update();
	void Draw();

private:
	//バッファ
	std::unique_ptr<CreateGpuBuffer> buffers;
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle, outputBufferHandle, particleDataHandle, drawCommandHandle, counterBufferHandle, commonBufferHandle;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	BufferMemorySize computeMemSize;
	//バッファ

	static const int PARTICLE_MAX_NUM = 1;
	static const int PER_USE_PARTICLE_MAX_NUM = 5000;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};

	struct ParticleData
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 vertices;
		float verticesMaxNum;
	};

	struct OutputData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
	};

	struct CommonData
	{
		DirectX::XMMATRIX cameraMat;
		DirectX::XMMATRIX projectionMat;
		DirectX::XMMATRIX bollboardMat;
		DirectX::XMFLOAT4 vertices[5000];
		UINT indexMaxNum;
		DirectX::XMFLOAT3 pad;
		UINT index[5000];
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;

	CommonData constBufferData;
	
	KazMath::Vec3<float>pos;

	ObjModelRender model;
};
