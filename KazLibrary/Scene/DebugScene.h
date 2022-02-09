#pragma once
#include"../DirectXCommon/Base.h"
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"

struct InputData
{
	float data;
};

struct OutPutData
{
	float data2;
};

class DebugScene :public SceneBase {
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
	short inputHandle, outPutHandle;

	InputData inputData;
	BufferMemorySize size;
};
