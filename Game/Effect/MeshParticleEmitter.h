#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"

class MeshParticleEmitter
{
public:
	MeshParticleEmitter(int NUM);
	~MeshParticleEmitter();
	void Init(const KazMath::Vec3<float> &POS);
	void Update();
	void Draw();

	int enemyIndex;
	bool resetSceneFlag;

private:
	int sceneNum;
	int oldEnemyNum;

	//バッファ
	std::unique_ptr<CreateGpuBuffer> buffers;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	//バッファ

	static const int PARTICLE_MAX_NUM = 50000;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};



	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	ObjModelRender model;
	std::array<std::string, 10>filePass;

	//必要情報--------------------------------------------
	KazMath::Vec3<float>pos;
	//必要情報--------------------------------------------

	//初期化--------------------------------------------
	struct OutputData
	{
		DirectX::XMFLOAT4 pos;
	};
	struct InitCommonData
	{
		DirectX::XMFLOAT4 worldPos;
		UINT vertMaxNum;
		UINT indexMaxNum;
	};
	InitCommonData constBufferData;
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle;
	RESOURCE_HANDLE verticesDataHandle, indexDataHandle, initCommonHandle;

	RESOURCE_HANDLE outputHandle,outputCounterHandle;

	RESOURCE_HANDLE outputViewHandle, vertDataViewHandle, indexViewDataHandle;
	//初期化--------------------------------------------

	//更新用--------------------------------------------
	struct UpdateData
	{
		DirectX::XMMATRIX matrix;
		DirectX::XMFLOAT4 color;
	};
	struct UpdateCommonData
	{
		DirectX::XMMATRIX scaleRotateBillboardMat;
		DirectX::XMMATRIX viewProjection;
		UINT indexMaxNum;
	};
	UpdateCommonData updateCommonData;
	RESOURCE_HANDLE updateHandle,updateCommonHandle;
	RESOURCE_HANDLE updateViewHandle;
	DirectX::XMMATRIX scaleRotaMat;


	UINT indexNum;

	//更新用--------------------------------------------

	RESOURCE_HANDLE drawCommandHandle;
};
