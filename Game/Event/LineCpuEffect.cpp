#include "LineCpuEffect.h"

LineCpuEffect::LineCpuEffect()
{
	for (int i = 0; i < lineRender.size(); ++i)
	{
		lineRender[i].data.pipelineName = PIPELINE_NAME_LINE_FLASHEFFECT;
		constBufferHandle[i] = lineRender[i].CreateConstBuffer(sizeof(XMFLOAT4), typeid(XMFLOAT4).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);

		lineEffectData[i].x = 1.0f;
		lineEffectData[i].y = 0.0f;
		lineEffectData[i].z = 0.0f;
		flashTimer[i] = 0;
	}
	maxTimer = 60;
}

void LineCpuEffect::Init(LineEffectVec VEC, KazMath::Vec3<float> &POS)
{
	lineRender[0].data.startPos = POS;
	KazMath::Vec3<float>endPos = POS;

	switch (VEC)
	{
	case LINE_UPVEC:
		endPos += KazMath::Vec3<float>(0.0f, 10.0f, 0.0f);
		break;
	case LINE_DOWNVEC:
		endPos += KazMath::Vec3<float>(0.0f, -10.0f, 0.0f);
		break;
	case LINE_LEFTVEC:
		endPos += KazMath::Vec3<float>(-10.0f, 0.0f, 0.0f);
		break;
	case LINE_RIGHTVEC:
		endPos += KazMath::Vec3<float>(10.0f, 0.0f, 0.0f);
		break;
	default:
		break;
	}
	lineRender[0].data.endPos = endPos;
	flashFlag = false;
}

void LineCpuEffect::Update()
{
	if (flashFlag)
	{
		//フラッシュ用のデータ
		for (int i = 0; i < lineRender.size(); ++i)
		{
			lineEffectData[i].w = static_cast<float>(flashTimer[i]) / static_cast<float>(maxTimer);
			lineRender[i].TransData(&lineEffectData[i], constBufferHandle[i], typeid(XMFLOAT4).name());
			++flashTimer[i];
		}
	}

	circleRender.data.transform.pos = { 0.0f,0.0f,0.0f };
	circleRender.data.radius = 10.0f;
	circleRender.data.change3DFlag = true;
}

void LineCpuEffect::Draw()
{
	for (int i = 0; i < lineRender.size(); ++i)
	{
		lineRender[i].Draw();
	}
	circleRender.Draw();
}

void LineCpuEffect::FlashLight()
{
	flashFlag = true;
}

void LineCpuEffect::InitFlash()
{
	flashFlag = false;
	for (int i = 0; i < lineRender.size(); ++i)
	{
		flashTimer[i] = 0;
	}
}
