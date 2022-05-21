#include "SplineMisile.h"
#include"../KazLibrary/Math/KazMath.h"

SplineMisile::SplineMisile()
{
	maxTime = 60 * 10;
	splineBox.data.color = { 255.0f,0.0f,0.0f,255.0f };
}

void SplineMisile::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;
	iEnemy_EnemyStatusData->timer = maxTime;

	iEnemy_EnemyStatusData->hitBox.radius = 30.0f;
	iEnemy_EnemyStatusData->hitBox.center = &splineBox.data.transform.pos;
	iOperationData.Init(1);

	startIndex = 1;
	timeRate = 0.0f;


	points.clear();
	points.shrink_to_fit();

	XMVECTOR startPos = POS;
	XMVECTOR endPos = { 0.0f,0.0f,0.0f };

	XMVECTOR distance = startPos - endPos;
	float addDistance = distance.m128_f32[2] / 4.0f;

	distance.m128_f32[2] /= 2.0f;	//中間はZ距離の半分
	distance.m128_f32[2] -= addDistance;


	//スタート地点
	points.push_back(startPos);
	points.push_back(startPos);

	//中間
	points.push_back(distance);

	//ゴール地点
	points.push_back(endPos);
	points.push_back(endPos);

	pointTime = maxTime /( points.size() - 3);
}

void SplineMisile::Finalize()
{
}

void SplineMisile::Update()
{
	if (iOperationData.enableToHitFlag)
	{
		--iEnemy_EnemyStatusData->timer;
	}

	//移動時間----------------------------------------------
	timeRate = static_cast<float>(nowTime) / static_cast<float>(pointTime);
	if (1.0f <= timeRate)
	{
		if (startIndex < points.size() - 3)
		{
			startIndex++;
			timeRate = 0.0f;
			nowTime = 0;
		}
		else
		{
			timeRate = 1.0f;
		}
	}
	++nowTime;
	//移動時間----------------------------------------------


	//座標設定----------------------------------------------
	position = KazMath::SplinePosition(points, startIndex, timeRate, true);

	for (int i = 0; i < pointsRender.size(); ++i)
	{
		pointsRender[i].data.transform.pos = points[i];
	}

	splineBox.data.transform.pos = position;
	//座標設定----------------------------------------------



	//死亡処理
	if (!iOperationData.enableToHitFlag)
	{
		iOperationData.initFlag = false;
	}
}

void SplineMisile::Draw()
{
	for (int i = 0; i < pointsRender.size(); ++i)
	{
		pointsRender[i].Draw();
	}
	splineBox.Draw();
}
