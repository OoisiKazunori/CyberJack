#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

class GoalBoxPortalEffect
{
public:
	GoalBoxPortalEffect();

	void Init(const KazMath::Vec3<float> &POS);
	void Update();
	void Draw();

	void Start();
	bool AllHidden();
	Sprite3DRenderPtr sprite;
	int timer;
private:
	bool startFlag;

	bool appearNextStageFlag;
	RESOURCE_HANDLE constBuffHandle;
	KazMath::Vec3<float> lerpScale;
};
