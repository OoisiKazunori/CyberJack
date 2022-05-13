#include "LineEffect.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"

LineEffect::LineEffect()
{
	line = std::make_unique<LineRender>();//仮引数にtrue入れていた

	circle = std::make_unique<Sprite3DRender>();
	releaseCircle = std::make_unique<Sprite3DRender>();
	rockOnFlag = false;

	circleTexHandle = TextureResourceMgr::Instance()->LoadGraph("Resource/Line/circle_fire.png");
	circle->data.billBoardFlag = true;
	circle->data.handle = circleTexHandle;
	circle->data.transform.scale = { 0.2f,0.2f,0.2f };

	releaseCircle->data = circle->data;
	releaseCircleEffectFlag = false;


	line->data.pipelineName = PIPELINE_NAME_LINE_UV_MULTITEX;		//線の色変える用のパイプライン
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

	//リリース時の円の座標
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


	//ロックオン処理
	if (rockOnFlag && !finishRockOnFlag)
	{
		//始点を初期化
		line->data.startPos = startPos;

		//経過時間から線の伸ばす値を計算
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


	//リリース処理
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
		//終点までたどり着いたら透明度を上げる
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

		//完全に透明になったら全部終了する
		if (lineDataColorFlag)
		{
			lineData.color.w -= sub;
		}
		else
		{
			lineData.color.w = 0.0f;
		}

		//完全に透明になったら全部終了する
		if (releaseCircleColorFlag)
		{
			releaseCircle->data.alpha -= subSpeed;
		}
		else
		{
			releaseCircle->data.alpha = 0.0f;
		}

		//完全に透明になったら全部終了する
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
	//完全に透明になったら全部終了する
	return allFinishFlag;
}
