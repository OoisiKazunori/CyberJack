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
	XMFLOAT4 pos;
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



	XMMATRIX Translate(XMFLOAT3 VECTOR)
	{
		XMMATRIX result;
		XMVECTOR first = { 1.0f, 0.0f, 0.0f, 0.0f };
		XMVECTOR second = { 0.0f, 1.0f, 0.0f, 0.0f };
		XMVECTOR third = { 0.0f, 0.0f, 1.0f, 0.0f };
		XMVECTOR forth = { VECTOR.x, VECTOR.y, VECTOR.z, 1.0f };

		result.r[0] = first;
		result.r[1] = second;
		result.r[2] = third;
		result.r[3] = forth;
		return result;
	}

	XMMATRIX Scale(XMFLOAT3 VECTOR)
	{
		XMMATRIX result;
		XMVECTOR first = { VECTOR.x, 0.0f, 0.0f, 0.0f };
		XMVECTOR second = { 0.0f, VECTOR.y, 0.0f, 0.0f };
		XMVECTOR third = { 0.0f, 0.0f, VECTOR.z, 0.0f };
		XMVECTOR forth = { 0.0f, 0.0f, 0.0f, 1.0f };

		result.r[0] = first;
		result.r[1] = second;
		result.r[2] = third;
		result.r[3] = forth;
		return result;
	}

	XMMATRIX RotateX(float ANGLE)
	{
		float lsin = sin(ANGLE);
		float lcos = sin(ANGLE);

		XMMATRIX result;
		XMVECTOR first = { 0.0f, 0.0f, 0.0f, 0.0f };
		XMVECTOR second = { 0.0f, lcos, lsin, 0.0f };
		XMVECTOR third = { 0.0f, -lsin, lcos, 0.0f };
		XMVECTOR forth = { 0.0f, 0.0f, 0.0f, 1.0f };

		result.r[0] = first;
		result.r[1] = second;
		result.r[2] = third;
		result.r[3] = forth;
		return result;
	}

	XMMATRIX RotateY(float ANGLE)
	{
		float lsin = sin(ANGLE);
		float lcos = sin(ANGLE);

		XMMATRIX result;
		XMVECTOR first = { lcos, 0.0f, -lsin, 0.0f };
		XMVECTOR second = { 0.0f, 1.0f, 0.0f, 0.0f };
		XMVECTOR third = { lsin, 0.0f, lcos, 0.0f };
		XMVECTOR forth = { 0.0f, 0.0f, 0.0f, 1.0f };

		result.r[0] = first;
		result.r[1] = second;
		result.r[2] = third;
		result.r[3] = forth;

		return result;
	}

	XMMATRIX RotateZ(float ANGLE)
	{
		float lsin = sin(ANGLE);
		float lcos = sin(ANGLE);

		XMMATRIX result;
		XMVECTOR first = { lcos, lsin, 0.0f, 0.0f };
		XMVECTOR second = { -lsin, lcos, 0.0f, 0.0f };
		XMVECTOR third = { 0.0f, 0.0f, 1.0f, 0.0f };
		XMVECTOR forth = { 0.0f, 0.0f, 0.0f, 1.0f };

		result.r[0] = first;
		result.r[1] = second;
		result.r[2] = third;
		result.r[3] = forth;

		return result;
	}

	XMMATRIX Rotate(XMFLOAT3 ANGLE)
	{
		XMMATRIX rota = XMMatrixIdentity();
		rota *= RotateZ(ANGLE.z);
		rota *= RotateX(ANGLE.x);
		rota *= RotateY(ANGLE.y);

		return rota;
	}
};
