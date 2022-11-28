#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../Game/Debug/ParameterMgr.h"
#include"../Game/Effect/GalacticParticle.h"
#include"../Game/Effect/TextureParticle.h"
#include"../Game/Effect/SplineParticle.h"

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
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle,
		outputInitBufferHandle, outputBufferHandle,
		particleDataHandle, drawCommandHandle, counterBufferHandle,
		commonInitBufferHandle, commonBufferHandle;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	BufferMemorySize computeMemSize;
	RESOURCE_HANDLE outputInitViewHandle, outputViewHandle, particleDataViewHandle;
	//バッファ

	static const int PARTICLE_MAX_NUM = 4000;
	static const int PER_USE_PARTICLE_MAX_NUM = 20;
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

	struct OutputInitData
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 color;
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
		DirectX::XMMATRIX billboardMat;
		DirectX::XMFLOAT4 vertices[8];
	};


	struct CommonMoveData
	{
		DirectX::XMMATRIX viewProjectionMat;
		DirectX::XMMATRIX scaleRotateBillboardMat;
		DirectX::XMFLOAT2 flash;
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	CommonMoveData constBufferData;
	int num;

	ParameterMgr blockFileMgr;


	GalacticParticle galacticParticle;

	bool highFlag, prepareFlag;
	int prepareTimer;

	int flashTimer;

	std::array<std::unique_ptr<TextureParticle>, 6> floorParticleModel;
	std::array<std::unique_ptr<SplineParticle>, 1> splineParticle;

	RESOURCE_HANDLE floorResourceHandle;
	KazMath::Vec3<float>v;
};

