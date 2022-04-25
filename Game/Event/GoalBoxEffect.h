#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Render/KazRender.h"
#include<array>
#include<memory>

class GoalBoxEffect
{
public:
	GoalBoxEffect();

	void Init(XMMATRIX *ADRESS);
	void Update();
	void Draw();

	void Appear();

	static const int LIGHT_MAX_NUM = 8;
	std::array<Sprite3DRenderPtr, LIGHT_MAX_NUM> lightRender;
private:
	XMMATRIX *motherPtr;
	std::array<short, LIGHT_MAX_NUM> uvHandle;
	GoalLightData uvData;

	int drawHandle;
};

