#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>

class BlockParticleStage :public IStage
{
public:
	BlockParticleStage();
	~BlockParticleStage();
	void Update()override;
	void Draw()override;

private:
	//バッファ
	std::unique_ptr<CreateGpuBuffer> buffers;
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle, outputBufferHandle, particleDataHandle, drawCommandHandle, counterBufferHandle, commonBufferHandle;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	BufferMemorySize computeMemSize;
	//バッファ

	static const int PARTICLE_MAX_NUM = 2500;
	static const int PER_USE_PARTICLE_MAX_NUM = 120;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};

	struct ParticleData
	{
		DirectX::XMFLOAT4 pos;
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
		DirectX::XMFLOAT4 vertices[8];
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;

	CommonData constBufferData;
	int num;
};

