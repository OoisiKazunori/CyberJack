#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

class GoalBoxPortalEffect
{
public:
	GoalBoxPortalEffect();

	void Init(const XMVECTOR &POS);
	void Update();
	void Draw();

	void Start();
	bool AllHidden();
	Sprite3DRenderPtr sprite;
private:
	
	bool startFlag;
	bool goBeforeFlag;
	int timer;

	XMVECTOR lerpRota;
};
