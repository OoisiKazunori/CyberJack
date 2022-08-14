#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/LineFlashLight.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../Game/Stage/DrawGrid.h"
#include"../Game/Stage/MountainObject.h"

class RezStage :public IStage
{
public:
	RezStage();
	void Update()override;
	void Draw()override;

private:

	//グリッドの描画ーーー
	int timer;
	int maxTimer;
	//グリッドの描画ーーー


	//床にOBJ配置
	struct FloorObjData
	{
		std::array<ObjModelRender, 2> objRender;
		KazMath::Vec3<float>initScale;
	};
	std::array<FloorObjData, 50>floorObjectRender;


	std::array<MountainObject, 40>floorObjectRender2;

	float scaleRate;
	bool reversValueFlag;

	Sprite3DRender vaporWaveSunRender;


	std::array<int, 50>filePassNum;
	ParameterMgr stageParamLoader;

	int selectNum;
	BoxPolygonRender selectingR;


	Sphere sphere;
	KazMath::Vec3<float>pos;
	LineRenderPtr ray;

	BoxPolygonRender c1, c2;


	std::array<DrawGrid, 4> gridRender;

	std::unique_ptr<BoxPolygonRender> poly;
	RESOURCE_HANDLE lineDrawHandle;


	std::array<RESOURCE_HANDLE, 3> stageModelhandle;

	struct MatData
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMMATRIX mat;
	};
};