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
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle, 
		outputInitBufferHandle, verticesDataHandle, indexDataHandle,
		drawCommandHandle, counterBufferHandle, commonInitBufferHandle,
		countIndexBuffHandle;

	RESOURCE_HANDLE outputInitViewHandle, vertDataViewHandle, indexViewDataHandle, counterIndexBufferViewHandle;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	//バッファ

	static const int PARTICLE_MAX_NUM = 1;
	static const int PER_USE_PARTICLE_MAX_NUM = 5000;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};


	struct OutputInitData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
	};

	struct CommonData
	{
		DirectX::XMMATRIX cameraMat;
		DirectX::XMMATRIX projectionMat;
		DirectX::XMMATRIX bollboardMat;
		DirectX::XMFLOAT4 worldPos;
		UINT vertMaxNum;
		UINT indexMaxNum;
		UINT dev;
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	CommonData constBufferData;
	KazMath::Vec3<float>pos;
	ObjModelRender model;
	int lDev = 0;
	std::array<std::string, 10>filePass;
};
