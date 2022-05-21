#include "MisileEnemy.h"
#include"../KazLibrary/Math/KazMath.h"

MisileEnemy::MisileEnemy()
{
	maxTime = 120;
	splineBox.data.color = { 255.0f,0.0f,0.0f,255.0f };
}

void MisileEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;
	iEnemy_EnemyStatusData->timer = maxTime;
	iOperationData.Init(1);

	startIndex = 0;
	timeRate = 0.0f;


	points.clear();
	points.shrink_to_fit();

	//スタート地点
	points.push_back(POS);
	points.push_back(POS);

	//中間
	points.push_back({ 0.0f,50.0f,50.0f });
	points.push_back({ 0.0f,-30.0f,-50.0f });

	//ゴール地点
	points.push_back({ 0.0f,0.0f,0.0f });
	points.push_back({ 0.0f,0.0f,0.0f });

	pointTime = maxTime/ points.size();
}

void MisileEnemy::Finalize()
{
}

void MisileEnemy::Update()
{
	--iEnemy_EnemyStatusData->timer;

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

void MisileEnemy::Draw()
{
	for (int i = 0; i < pointsRender.size(); ++i)
	{
		pointsRender[i].Draw();
	}
	splineBox.Draw();
}
