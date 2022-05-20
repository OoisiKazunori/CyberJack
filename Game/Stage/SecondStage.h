#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Interface/IStage.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../KazLibrary/Render/KazRender.h"
#include<array>

class SecondStage :public IStage
{
public:
	SecondStage();
	void Update()override;
	void Draw()override;

private:
	std::array<LineRender, 50> line;
	ParameterMgr stageParamLoader;

};

