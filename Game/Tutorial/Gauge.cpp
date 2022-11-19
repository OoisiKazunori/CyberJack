#include "Gauge.h"

Gauge::Gauge(RESOURCE_HANDLE FLAME_TEXTURE_HANDLE, RESOURCE_HANDLE GAUGE_TEXTURE_HANDLE)
{
	flameR.data.handleData = FLAME_TEXTURE_HANDLE;
	gaugeR.data.handleData = GAUGE_TEXTURE_HANDLE;
}

void Gauge::Init(int MAX)
{
	maxNum = MAX;
	nowNum = 0;
	isMaxFlag = false;
	isHalfFlag = false;


	flameR.data.transform.pos = { WIN_X / 2.0f,WIN_Y - 100.0f };
	flameR.data.transform.scale = { 10.0f,3.0f };
	gaugeR.data.transform.pos = { WIN_X / 2.0f - 500.0f,WIN_Y - 100.0f };
	gaugeR.data.transform.scale = { 1.0f,55.0f };
}

void Gauge::Update()
{
	if (maxNum <= nowNum)
	{
		nowNum = maxNum;
		isMaxFlag = true;
	}
	rate = static_cast<float>(nowNum) / static_cast<float>(maxNum);

	if (0.5f <= rate)
	{
		isHalfFlag = true;
	}

	float lMaxRate = 2000.0f;
	gaugeR.data.rightScale.x = rate * lMaxRate;
}

void Gauge::Draw()
{
	gaugeR.Draw();
	flameR.Draw();
}

void Gauge::Add(int NUM)
{
	nowNum += NUM;
}

bool Gauge::IsMax()
{
	return isMaxFlag;
}
