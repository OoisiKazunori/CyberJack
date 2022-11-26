#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../Game/Debug/ParameterMgr.h"

class DeadParticle
{
public:
	DeadParticle(const D3D12_GPU_VIRTUAL_ADDRESS &ADDRESS, int VERT_NUM, float PARTICLE_SCALE = 0.18f);
	void Init(const DirectX::XMMATRIX *MAT);
	void Update(int ALPHA = 0);
	void Draw();

private:
	bool startFlag;
	const DirectX::XMMATRIX *motherMat;
	int timer;
	//バッファ
	std::unique_ptr<CreateGpuBuffer> buffers;
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle,
		outputInitBufferHandle, outputBufferHandle,
		drawCommandHandle, counterBufferHandle,
		commonInitBufferHandle, commonBufferHandle;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	BufferMemorySize computeMemSize;
	RESOURCE_HANDLE outputInitViewHandle, outputViewHandle;
	//バッファ

	int PARTICLE_MAX_NUM = 100000;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};


	struct OutputInitData
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 vel;
	};

	struct OutputData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
	};

	struct CommonMoveData
	{
		DirectX::XMMATRIX scaleRotateBillboardMat;
		DirectX::XMMATRIX viewProjection;
		DirectX::XMMATRIX motherMat;
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	CommonMoveData constBufferData;
	int num;
	DirectX::XMMATRIX scaleRotaMat;

	RESOURCE_HANDLE texHandle;
};