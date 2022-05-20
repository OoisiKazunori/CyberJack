#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Interface/IStage.h"

class SecondStage :public IStage
{
public:
	SecondStage();
	void Update()override;
	void Draw()override;

private:
};

