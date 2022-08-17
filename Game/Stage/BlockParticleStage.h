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

	static const int PARTICLE_MAX_NUM = 1;
	static const int PER_USE_PARTICLE_MAX_NUM = 125;
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
		unsigned int index0[2];
		unsigned int pad[2];
		unsigned int index1[2];
		unsigned int pad1[2];
		unsigned int index2[2];
		unsigned int pad2[2];
		unsigned int index3[2];
		unsigned int pad3[2];
		unsigned int index4[2];
		unsigned int pad4[2];
		unsigned int index5[2];
		unsigned int pad5[2];
		unsigned int index6[2];
		unsigned int pad6[2];
		unsigned int index7[2];
		unsigned int pad7[2];
		unsigned int index8[2];
		unsigned int pad8[2];
		unsigned int index9[2];
		unsigned int pad9[2];
		unsigned int index10[2];
		unsigned int pad10[2];
		unsigned int index11[2];
		unsigned int pad11[2];
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;

	CommonData constBufferData;
};

