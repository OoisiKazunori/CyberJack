#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Debug/ParameterMgr.h"
#include<array>
#include<vector>

class FirstStage
{
public:
	FirstStage();
	void Update();
	void Draw();

private:
	std::array<BoxPolygonRender, 20> stageDebugBox;
	std::array<FogData, 20> fogData;
	std::array<float, 20> constHandle;

	ParameterMgr stageParamLoader;
};

