#include "LineEffect.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"

LineEffect::LineEffect()
{
	line = std::make_unique<LineRender>();//��������true����Ă���

	circle = std::make_unique<Sprite3DRender>();
	releaseCircle = std::make_unique<Sprite3DRender>();
	rockOnFlag = false;

	circleTexHandle = TextureResourceMgr::Instance()->LoadGraph("Resource/Line/circle_fire.png");
	circle->data.billBoardFlag = true;
	circle->data.handle = circleTexHandle;
	circle->data.transform.scale = { 0.2f,0.2f,0.2f };

	releaseCircle->data = circle->data;
	releaseCircleEffectFlag = false;


	line->data.pipelineName = PIPELINE_NAME_LINE_UV_MULTITEX;		//���̐F�ς���p�̃p�C�v���C��
	circle->data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;
	releaseCircle->data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;

	constBufferHandle = line->CreateConstBuffer(sizeof(ConstLineData), typeid(ConstLineData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);

	finishFlag = false;
	allFinishFlag = false;

	lockOnSpeed = 6.0f;
	releaseSpeed = 10.0f;
}

LineEffect::~LineEffect()
{
	rockOnFlag = false;
	finishFlag = false;
	allFinishFlag = false;
}

void LineEffect::RockOn(const XMVECTOR &START_POS, const XMVECTOR &END_POS, const XMVECTOR &START_PLAYER_DISTANCE, const XMVECTOR &END_PLAYER_DISTANCE)
{
	if (!rockOnFlag)
	{
		startPos = START_POS;
		endPos = END_POS;
		startPlayerDistance = START_PLAYER_DISTANCE;
		endPlayerDistance = END_PLAYER_DISTANCE;


		float color = 255.0f;
		line->data.color = { color,color,color ,255.0f };
		//234.0f,69.0f,50.0f

		line->data.startPos = startPos;
		line->data.endPos = endPos;


		lockOnTime = 0.0f;
		releaseTime = 0.0f;


		releaseCircle->data.alpha = 255.0f;
		circle->data.alpha = 255.0f;


		lineData.distance.x = -1.0f;
		lineData.color = { 510.0f / 255.0f,0.0f,0.0f,255.0f / 255.0f };
		finishRockOnFlag = false;
		finishReleaseFlag = false;
		allFinishFlag = false;
	}
}

void LineEffect::StartEffect()
{
	rockOnFlag = true;
}

void LineEffect::ReleaseEffect()
{
	releaseFlag = true;
}

void LineEffect::Finalize()
{
	finishFlag = true;
}

void LineEffect::Update()
{
	startPos = playerPos + startPlayerDistance * value;
	endPos = playerPos + endPlayerDistance * value;

	//�����[�X���̉~�̍��W
	releaseCircle->data.transform.pos = endPos;


	distance = endPos - startPos;

	for (int i = 0; i < 3; ++i)
	{
		if (distance.m128_f32[i] != 0.0f)
		{
			rate = distance.m128_f32[i];
			axis = i;
			break;
		}
	}
	minusFlag = signbit(rate);
	rate = (float)fabs(rate);
	if (rate <= 0)
	{
		rate = 1.0f;
	}


	//���b�N�I������
	if (rockOnFlag && !finishRockOnFlag)
	{
		//�n�_��������
		line->data.startPos = startPos;

		//�o�ߎ��Ԃ�����̐L�΂��l���v�Z
		float distance = (lockOnTime / rate) * rate;
		if (minusFlag)
		{
			distance *= -1;
		}
		line->data.endPos = endPos;
		line->data.endPos.m128_f32[axis] = startPos.m128_f32[axis] + distance;

		if (lockOnTime < rate)
		{
			lockOnTime += lockOnSpeed;
		}
		else
		{
			lockOnTime = rate;
			line->data.endPos = endPos;
			finishRockOnFlag = true;
		}

		circle->data.transform.pos = line->data.endPos;
	}
	else
	{
		line->data.startPos = startPos;
		line->data.endPos = endPos;
	}


	//�����[�X����
	if (releaseFlag && !finishReleaseFlag)
	{
		lineData.distance = { (releaseTime / rate),0.0f,0.0f,0.0f };

		if (releaseTime < rate)
		{
			releaseTime += releaseSpeed;
		}
		else
		{
			releaseTime = rate;
			lineData.distance.x = 1.0f;
		}
		//�I�_�܂ł��ǂ蒅�����瓧���x���グ��
		if (1.0f <= lineData.distance.x)
		{
			finishReleaseFlag = true;
			releaseCircleEffectFlag = true;
		}
	}

	if (finishFlag)
	{
		float subSpeed = 10.0f;
		float sub = (subSpeed / 255.0f);

		bool lineDataColorFlag = 0.0f < lineData.color.w;
		bool releaseCircleColorFlag = 0.0f < releaseCircle->data.alpha;
		bool circleColorFlag = 0.0f < circle->data.alpha;

		//���S�ɓ����ɂȂ�����S���I������
		if (lineDataColorFlag)
		{
			lineData.color.w -= sub;
		}
		else
		{
			lineData.color.w = 0.0f;
		}

		//���S�ɓ����ɂȂ�����S���I������
		if (releaseCircleColorFlag)
		{
			releaseCircle->data.alpha -= subSpeed;
		}
		else
		{
			releaseCircle->data.alpha = 0.0f;
		}

		//���S�ɓ����ɂȂ�����S���I������
		if (circleColorFlag)
		{
			circle->data.alpha -= subSpeed;
		}
		else
		{
			circle->data.alpha = 0.0f;
		}


		if (!lineDataColorFlag && !releaseCircleColorFlag && !circleColorFlag)
		{
			allFinishFlag = true;
		}
	}

	line->TransData(&lineData, constBufferHandle, typeid(ConstLineData).name());

}

void LineEffect::Draw()
{
	if (rockOnFlag && !finishRockOnFlag)
	{
		circle->Draw();
	}

	if (releaseFlag && releaseCircleEffectFlag)
	{
		releaseCircle->Draw();
	}


	if (rockOnFlag)
	{
		line->Draw();
	}
}

void LineEffect::MoveLine(const XMVECTOR &VALUE)
{
	value = VALUE;
}

bool LineEffect::FinishFlag()
{
	//���S�ɓ����ɂȂ�����S���I������
	return allFinishFlag;
}
