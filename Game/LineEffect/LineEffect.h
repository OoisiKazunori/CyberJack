#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

struct ConstLineData
{
	XMFLOAT4 distance;
	XMFLOAT4 color;
};

class LineEffect
{
public:
	LineEffect();
	~LineEffect();

	void RockOn(const XMVECTOR &START_POS, const XMVECTOR &END_POS);
	void StartEffect();
	void ReleaseEffect();
	void Finalize();
	void Update();
	void Draw();

	bool FinishFlag();

	bool finishRockOnFlag;
	bool finishReleaseFlag;
private:
	LineRenderPtr line;
	Sprite3DRenderPtr circle;
	Sprite3DRenderPtr releaseCircle;
	short constBufferHandle;
	ConstLineData lineData;

	bool rockOnFlag;
	XMVECTOR startPos, endPos;
	XMVECTOR distance;
	float rate,lockOnTime,releaseTime;
	float lockOnSpeed,releaseSpeed;
	int axis;

	bool minusFlag;
	short circleTexHandle;

	bool releaseFlag;
	bool releaseCircleEffectFlag;

	bool finishFlag;
	bool allFinishFlag;
};

