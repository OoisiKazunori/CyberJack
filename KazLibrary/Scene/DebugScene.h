#pragma once
#include"../DirectXCommon/Base.h"
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Render/KazRender.h"

struct InputData
{
	XMFLOAT3 pos;
	float pad;
};

struct OutPutData
{
	XMMATRIX mat;
};

struct CommonData
{
	XMMATRIX cameraMat;
	XMMATRIX projectionMat;
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
	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec2<float> angle;

	//BackGroundForDebug bg;

	unique_ptr<CreateGpuBuffer>buffer;
	RESOURCE_HANDLE inputHandle, outPutHandle, commonHandle;

	InputData inputData;
	BufferMemorySize size;


	array<array<XMMATRIX, 800>, 13>matData;
	BoxPolygonRenderPtr instanceBox;
	array<int, 10000>test;
};
