#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Debug/ParameterMgr.h"
#include<array>
#include<vector>

struct FogD
{
	//フォグカラー
	XMFLOAT3 endColor;
	float pad;
	XMFLOAT3 subValue;
	float pad2;
	//x...RataMin,y...RateMax,z...StartFogDepth,w...EndFogDepth
	XMFLOAT4 rateAndFogLine;
};

class FirstStage
{
public:
	FirstStage();
	void Update();
	void Draw();

private:
	std::array<BoxPolygonRender, 30> stageDebugBox;
	std::array<FogD, 30> fogData;
	std::array<float, 30> constHandle;

	ParameterMgr stageParamLoader;
};

