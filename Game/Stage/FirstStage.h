#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../Game/Interface/IStage.h"
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
	XMFLOAT3 depthX;
	float pad3;
};

class FirstStage :public IStage
{
public:
	FirstStage();
	void Update()override;
	void Draw()override;

private:
	std::array<BoxPolygonRender, 30> stageDebugBox;
	std::array<KazMath::Vec3<float>, 30> stageYPos;
	std::array<FogD, 30> fogData;
	std::array<short, 30> constHandle;

	ParameterMgr stageParamLoader;
	XMFLOAT3 depthX;

	std::array<std::unique_ptr<PolygonRender>, 4>polygon;
	std::unique_ptr<PolygonRender> topPolygon;

	float easeY;
	float t;
};

