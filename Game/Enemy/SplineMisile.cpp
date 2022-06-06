#include "SplineMisile.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

SplineMisile::SplineMisile()
{
	maxTime = 60 * 5;
	splineBox.data.color = { 255.0f,0.0f,0.0f,255.0f };

	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "misile.obj");
}

void SplineMisile::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;
	iEnemy_ModelRender->data.transform.scale = { 1.3f,1.3f,1.3f };
	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.color.x = 255.0f;
	iEnemy_ModelRender->data.color.y = 255.0f;
	iEnemy_ModelRender->data.color.z = 255.0f;

	iEnemy_EnemyStatusData->timer = maxTime;
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;
	iEnemy_EnemyStatusData->hitBox.center = &iEnemy_ModelRender->data.transform.pos;

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


	//死亡演出処理
	//デバックキーor当たり判定内&&死亡時
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.m128_f32[2]) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.color.x = 255.0f;
		iEnemy_ModelRender->data.color.y = 255.0f;
		iEnemy_ModelRender->data.color.z = 255.0f;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.w);
	}
	else
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

		iEnemy_ModelRender->data.transform.pos = position;
		//座標設定----------------------------------------------

		//前ベクトルの設定----------------------------------------------
		float nextTimeRate = static_cast<float>(nowTime + 1) / static_cast<float>(pointTime);
		XMVECTOR nextpos = KazMath::SplinePosition(points, startIndex, nextTimeRate, true);

		XMVECTOR upVector = nextpos - position;
		upVector = XMVector3Normalize(upVector);
		iEnemy_ModelRender->data.upVector = upVector;
		//前ベクトルの設定----------------------------------------------
	}

	//死亡処理
	if (iEnemy_ModelRender->data.color.w <= 0.0f)
	{
		iOperationData.initFlag = false;
	}
}

void SplineMisile::Draw()
{
	for (int i = 0; i < pointsRender.size(); ++i)
	{
		//pointsRender[i].Draw();
	}
	iEnemy_ModelRender->Draw();
	LockOnWindow(iEnemy_ModelRender->data.transform.pos);
}
