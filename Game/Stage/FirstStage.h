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
	std::array<BoxPolygonRender, 30> stageDebugBox;
	std::array<FogData, 30> fogData;
	std::array<float, 30> constHandle;

	ParameterMgr stageParamLoader;
};

