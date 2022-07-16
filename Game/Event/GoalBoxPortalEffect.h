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
	Sprite3DRenderPtr noiseSprite;
	int timer;
private:
	bool startFlag;
	bool changeStageFlag;
	Sprite3DRender portalSprite;
	bool appearNextStageFlag;
	RESOURCE_HANDLE constBuffHandle;
	KazMath::Vec3<float> lerpScale;
};
