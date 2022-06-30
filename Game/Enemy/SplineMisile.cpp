#include "SplineMisile.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

SplineMisile::SplineMisile()
{
	maxTime = 60 * 5;
	splineBox.data.color = { 255,0,0,255 };

	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "misile.obj");
}

void SplineMisile::Init(const KazMath::Vec3<float> &POS)
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

	KazMath::Vec3<float> startPos = POS;
	KazMath::Vec3<float> endPos = { 0.0f,0.0f,0.0f };

	KazMath::Vec3<float> distance = startPos - endPos;
	float addDistance = distance.z / 4.0f;

	distance.z /= 2.0f;	//中間はZ距離の半分
	distance.z -= addDistance;


	//スタート地点
	points.push_back(startPos);
	points.push_back(startPos);

	//中間
	points.push_back(distance);

	//ゴール地点
	points.push_back(endPos);
	points.push_back(endPos);

	pointTime = maxTime / (static_cast<int>(points.size() )- 3);

	initDeadSoundFlag = false;
	hitFlag = false;
}

void SplineMisile::Finalize()
{
}

void SplineMisile::Update()
{
	//当たったら描画しなくなる
	if (iEnemy_EnemyStatusData->timer <= 0)
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iOperationData.initFlag = false;
		hitFlag = true;
	}


	//死亡演出処理
	//デバックキーor当たり判定内&&死亡時
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag && !hitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.color.x = 255.0f;
		iEnemy_ModelRender->data.color.y = 255.0f;
		iEnemy_ModelRender->data.color.z = 255.0f;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.w);

		if (!initDeadSoundFlag)
		{
			DeadSound();
			initDeadSoundFlag = true;
		}
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
		position = KazMath::SplinePosition(points, static_cast<size_t>(startIndex), timeRate, true);

		for (int i = 0; i < pointsRender.size(); ++i)
		{
			pointsRender[i].data.transform.pos = points[i];
		}

		iEnemy_ModelRender->data.transform.pos = position;
		//座標設定----------------------------------------------

		//前ベクトルの設定----------------------------------------------
		float nextTimeRate = static_cast<float>(nowTime + 1) / static_cast<float>(pointTime);
		KazMath::Vec3<float> nextpos = KazMath::SplinePosition(points, startIndex, nextTimeRate, true);

		KazMath::Vec3<float> upVector = nextpos - position;
		upVector.Normalize();
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
	//for (int i = 0; i < pointsRender.size(); ++i)
	//{
		//pointsRender[i].Draw();
	//}
	iEnemy_ModelRender->Draw();
	LockOnWindow(iEnemy_ModelRender->data.transform.pos);
}
