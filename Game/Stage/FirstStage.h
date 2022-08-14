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
	DirectX::XMFLOAT3 endColor;
	float pad;
	DirectX::XMFLOAT3 subValue;
	float pad2;
	//x...RataMin,y...RateMax,z...StartFogDepth,w...EndFogDepth
	DirectX::XMFLOAT4 rateAndFogLine;
	DirectX::XMFLOAT3 depthX;
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
	std::array<RESOURCE_HANDLE, 30> constHandle;

	ParameterMgr stageParamLoader;
	DirectX::XMFLOAT3 depthX;


	float easeY;
	float t;
};

