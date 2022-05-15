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
private:
	Sprite3DRenderPtr sprite;
	//PolygonRenderPtr sprite;
	bool startFlag;
	bool goBeforeFlag;
	int timer;
};
