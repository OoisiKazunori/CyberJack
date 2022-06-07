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

	void RockOn(const XMVECTOR &START_POS, const XMVECTOR &END_POS, const XMVECTOR &START_PLAYER_DISTANCE, const XMVECTOR &END_PLAYER_DISTANCE);
	void StartEffect();
	void ReleaseEffect();
	void Finalize();
	void Update();
	void Draw();

	void MoveLine(const XMVECTOR &VALUE);
	bool FinishFlag();

	bool finishRockOnFlag;
	bool finishReleaseFlag;
	XMVECTOR playerPos;
private:
	const float NORMAL_LOCKON_SPEED = 6.0f;
	const float RAPID_LOCKON_SPEED = 12.0f;

	LineRenderPtr line;
	Sprite3DRenderPtr circle;
	Sprite3DRenderPtr releaseCircle;
	short constBufferHandle;
	ConstLineData lineData;

	bool rockOnFlag;
	XMVECTOR startPos, endPos;
	XMVECTOR startPlayerDistance, endPlayerDistance;
	XMVECTOR distance;
	XMVECTOR value;

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

