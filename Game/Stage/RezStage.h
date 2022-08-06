#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/LineFlashLight.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../KazLibrary/Collision/CollisionManager.h"


class RezStage :public IStage
{
public:
	RezStage();
	void Update()override;
	void Draw()override;

private:

	//グリッドの描画ーーー
	static const int LINE_MAX = 100;
	static const int LINE_Z_MAX = LINE_MAX / 2;
	static const int LINE_X_MAX = LINE_MAX / 2;

	std::array<LineRender, LINE_MAX> gridLineRender;
	std::array<std::array<KazMath::Transform3D, 2>, LINE_Z_MAX> gridFloorZLinePos;
	std::array<std::array<KazMath::Transform3D, 2>, LINE_X_MAX> gridFloorXLinePos;

	int timer;
	int maxTimer;
	int lightEffectIndex;
	std::array<std::array<KazMath::Vec3<float>, 5>, LINE_MAX> lightEffectGridFloorLinePos;
	std::array<std::array<LineFlashLight, 5>, LINE_MAX> lightEffectArray;
	std::array<bool, LINE_MAX> lightEffectInitFlagArray;
	//グリッドの描画ーーー


	//床にOBJ配置
	struct FloorObjData
	{
		std::array<BoxPolygonRender, 2> objRender;
		KazMath::Vec3<float>initScale;
	};
	std::array<FloorObjData, 50>floorObjectRender;

	float scaleRate;
	bool reversValueFlag;

	Sprite3DRender vaporWaveSunRender;


	FbxModelRender model;
	ObjModelRender objModel;

	std::array<std::array<char, 100>, 50>filePassChar;
	ParameterMgr stageParamLoader;

	int selectNum;
	BoxPolygonRender selectingR;


	Sphere sphere;
	KazMath::Vec3<float>pos;
	LineRenderPtr ray;

	BoxPolygonRender c1, c2;
};