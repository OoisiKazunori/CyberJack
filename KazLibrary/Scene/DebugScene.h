#pragma once
#include"../DirectXCommon/Base.h"
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Render/KazRender.h"

/// <summary>
/// パーティクルの移動系
/// </summary>
struct InputData
{
	XMFLOAT3 pos;
	float pad;
	XMFLOAT4 velocity;
	XMFLOAT4 color;
};

/// <summary>
/// コマンドバッファのアドレスの参照先
/// </summary>
struct OutPutData
{
	XMMATRIX mat;
	XMFLOAT4 color;
};

struct CommonData
{
	XMMATRIX cameraMat;
	XMMATRIX projectionMat;
};

struct IndirectCommand
{
	D3D12_GPU_VIRTUAL_ADDRESS cbv;
	D3D12_DRAW_ARGUMENTS drawArguments;
};

struct CSRootConstants
{
	XMMATRIX view;
	XMMATRIX projection;
	float size;
	float gpuAddress;
	XMFLOAT2 pad;
};

//移動情報
struct SceneConstantBuffer
{
	XMFLOAT4 velocity;
	XMFLOAT4 offset;
	XMFLOAT4 color;
	XMFLOAT4X4 projection;
	// Constant buffers are 256-byte aligned. Add padding in the struct to allow multiple buffers
	// to be array-indexed.
	float padding[36];
};

struct DrawConstantBuffer
{
	XMFLOAT4 projection;
};

static const int INSTANCE_NUM_MAX = 800;
class DebugScene :public SceneBase
{
public:
	DebugScene();
	~DebugScene();
	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	//void Input(ControllerInputManager *INPUT);
	int SceneChange();


private:
	XMFLOAT3 debugCameraMove;
	XMFLOAT3 eyePos, targetPos;
	XMFLOAT2 angle;

	//BackGroundForDebug bg;

	unique_ptr<CreateGpuBuffer>buffer;
	short inputHandle, updateInputHandle, outputMatHandle, drawCommandHandle;
	void *pointer;

	InputData inputData;
	BufferMemorySize size;
	BufferMemorySize cbvSize;
	int uavHandle;
	int srvHandle;


	static const int TRIANGLE_ARRAY_NUM = 1024;
	static const int FRAME_COUNT = 2;
	static const int TRIANGLE_RESOURCE_COUNT = TRIANGLE_ARRAY_NUM * FRAME_COUNT;
	static const int ComputeThreadBlockSize = TRIANGLE_ARRAY_NUM * FRAME_COUNT;
	static const int CommandSizePerFrame = TRIANGLE_ARRAY_NUM * sizeof(IndirectCommand);

	short commandBufferHandle;
	ComPtr<ID3D12CommandSignature> commandSig;
	std::array<SceneConstantBuffer, TRIANGLE_ARRAY_NUM> constantBufferData;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;


	CSRootConstants rootConst;

	short paramCBHandle;
};
