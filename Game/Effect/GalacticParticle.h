#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../Game/Debug/ParameterMgr.h"

class GalacticParticle
{
public:
	GalacticParticle();
	~GalacticParticle();
	void Update();
	void Draw();

private:
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

	static const int PARTICLE_MAX_NUM = 20000;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};


	struct OutputInitData
	{
		DirectX::XMFLOAT4 pos;
	};

	struct OutputData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
	};

	struct CommonMoveData
	{
		DirectX::XMMATRIX cameraMat;
		DirectX::XMMATRIX projectionMat;
		DirectX::XMMATRIX billboardMat;
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	CommonMoveData constBufferData;
	int num;

	ParameterMgr blockFileMgr;

	std::array<BoxPolygonRender, 50> box;
	std::array<KazMath::Vec3<float>, 50> vel;
};

