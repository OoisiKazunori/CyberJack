#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../KazLibrary/RenderTarget/GaussianBuler.h"
#include"../GameRenderTarget.h"
#include"../Player.h"
#include"../Game/UI/Cursor.h"

class PortalIntermediateDirection
{
public:
	PortalIntermediateDirection();
	~PortalIntermediateDirection();
	void Init();
	void Finalize();
	void Update(const KazMath::Vec3<float> &POS);
	void Draw(Cursor *CURSOR);

	void Start();
	bool IsStart();
	bool IsFinish();
	bool DrawNextPortal();

	float GetRate()
	{
		return backRate;
	}

	float GetNextRate()
	{
		return nextBackRate;
	}

	Sprite3DRender portalRender;
	std::unique_ptr<GameRenderTarget> renderTarget;
	bool disappearFlag;
	Sprite3DRender nextPortalRender;
private:
	int disappearTimer;
	bool nextPortalFlag;
	bool finishFlag;
	Player player;

	bool startFlag;


	//バッファ
	std::unique_ptr<CreateGpuBuffer> buffers;
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle, outputBufferHandle, particleDataHandle, drawCommandHandle, commonBufferHandle;
	RESOURCE_HANDLE outputMatBufferHandle;
	RESOURCE_HANDLE texHandle;

	RESOURCE_HANDLE outputInitViewHandle, particleViewHandle, outputMatHandle;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	BufferMemorySize computeMemSize;
	//バッファ

	static const int PARTICLE_MAX_NUM = 40000;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};

	struct ParticleData
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 vel;
		DirectX::XMFLOAT4 color;
		UINT timer;
	};

	struct OutputData
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 scale;
		DirectX::XMFLOAT4 color;
	};

	struct CommonData
	{
		DirectX::XMMATRIX cameraMat;
		DirectX::XMMATRIX projectionMat;
		DirectX::XMMATRIX billboardMat;
		DirectX::XMFLOAT4 emittPos;
		UINT increSize;
		UINT64 gpuAddress;
		int seed;
	};

	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	CommonData lData;


	struct OutputMatData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
	};

	float rate,nextRate;
	float backRate,nextBackRate;
	float baseZ;
};