#include "SplineMisileForBattleShip.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

SplineMisileForBattleShip::SplineMisileForBattleShip()
{
	maxTime = 60 * 5;
	splineBox.data.color = { 255,0,0,255 };

	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + +"BattleShipMisile/" + "BattleshipMissile_Model.obj");
}

void SplineMisileForBattleShip::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = GENERATE_DATA.initPos;
	iEnemy_ModelRender->data.transform.scale = { 0.4f,0.4f,0.4f };
	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.colorData.color.x = 255;
	iEnemy_ModelRender->data.colorData.color.y = 255;
	iEnemy_ModelRender->data.colorData.color.z = 255;
	iEnemy_ModelRender->data.colorData.color.a = 255;

	iEnemy_EnemyStatusData->timer = maxTime;
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;
	iEnemy_EnemyStatusData->hitBox.center = &iEnemy_ModelRender->data.transform.pos;

	iOperationData.Init(1);

	startIndex = 1;
	timeRate = 0.0f;


	points.clear();
	points.shrink_to_fit();

	KazMath::Vec3<float> startPos = GENERATE_DATA.initPos;
	KazMath::Vec3<float> endPos = { 0.0f,0.0f,0.0f };


	KazMath::Vec3<float>upMaxPos = {};
	KazMath::Vec3<float>downMaxPos = {};
	KazMath::Vec3<float> moveToPlayerPos = {};

	switch (GENERATE_DATA.battleShipEnemyData.shotEnum)
	{
	case BATTLESHIP_MISILE_SHOT_RIGHT:
		upMaxPos = startPos + KazMath::Vec3<float>(10.0f, 100.0f, 0.0f);
		downMaxPos = upMaxPos + KazMath::Vec3<float>(10.0f, -20.0f, -30.0f);
		moveToPlayerPos = downMaxPos + KazMath::Vec3<float>(0.0f, -5.0f, -30.0f);
		break;
	case BATTLESHIP_MISILE_SHOT_LEFT:
		upMaxPos = startPos + KazMath::Vec3<float>(-10.0f, 100.0f, 0.0f);
		downMaxPos = upMaxPos + KazMath::Vec3<float>(-10.0f, -20.0f, -30.0f);
		moveToPlayerPos = downMaxPos + KazMath::Vec3<float>(0.0f, -5.0f, -30.0f);
		break;
	default:
		break;
	}


	//スタート地点
	points.push_back(startPos);
	points.push_back(startPos);

	//中間
	points.push_back(upMaxPos);
	points.push_back(downMaxPos);
	points.push_back(moveToPlayerPos);

	//ゴール地点
	points.push_back(endPos);
	points.push_back(endPos);

	pointTime = maxTime / (static_cast<int>(points.size()) - 3);
	nowTime = 0;
	initDeadSoundFlag = false;
	hitFlag = false;

	demoFlag = DEMO_FLAG;

	rocketLight.Fisnish();

	smokeEmitter.Init(&iEnemy_ModelRender->data.transform.pos);
}
void SplineMisileForBattleShip::Finalize()
{
	smokeEmitter.Finalize();
}

void SplineMisileForBattleShip::Update()
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
		iEnemy_ModelRender->data.colorData.color.x = 255;
		iEnemy_ModelRender->data.colorData.color.y = 255;
		iEnemy_ModelRender->data.colorData.color.z = 255;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.colorData.color.a);

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


		if (startIndex != 2)
		{
			KazMath::Vec3<float> upVector = nextpos - position;
			upVector.Normalize();
			iEnemy_ModelRender->data.upVector = upVector;
		}
		//炎点火
		if (startIndex == 2 && !rocketLight.IsStart())
		{
			rocketLight.Init(&iEnemy_ModelRender->data.transform.pos, KazMath::Vec3<float>(0.0f, 0.0f, -5.0f), false);
			rocketLight.ChangeLightRadius(3.0f);
		}
		//炎縮小
		if (3 <= startIndex)
		{
			rocketLight.ChangeLightRadius(0.1f);
		}
		//前ベクトルの設定----------------------------------------------
	}

	//死亡処理
	if (iEnemy_ModelRender->data.colorData.color.a <= 0.0f)
	{
		iOperationData.initFlag = false;
	}

	rocketLight.Update();
	smokeEmitter.Update();
}

void SplineMisileForBattleShip::Draw()
{
	if (demoFlag)
	{
		for (int i = 0; i < pointsRender.size(); ++i)
		{
			pointsRender[i].Draw();
		}
	}
	rocketLight.Draw();
	iEnemy_ModelRender->Draw();
	smokeEmitter.Draw();
	LockOnWindow(iEnemy_ModelRender->data.transform.pos);
}
