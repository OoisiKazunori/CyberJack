#include "BattleshipEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

BattleshipEnemy::BattleshipEnemy()
{
	for (int i = 0; i < misileR.size(); ++i)
	{
		misileR[i].data.transform.pos.x = 5.0f;
		misileR[i].data.transform.pos.y = 88.0f;
		misileR[i].data.transform.pos.z = 25.0f;
	}

	topModel = std::make_unique<ObjModelRender>();

	//iEnemy_ModelRender = std::make_unique<FbxModelRender>();
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "BattleShip/" + "BattleshipEnemy_Model_Body.obj");	//モデル読み込み
	topModel->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "BattleShip/" + "BattleshipEnemy_Model_Radar.obj");	//モデル読み込み

}

void BattleshipEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = GENERATE_DATA.initPos;	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 0.5f,0.5f,0.5f };
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//当たり判定の大きさ変更
	iOperationData.Init(3);							//残りロックオン数等の初期化
	iEnemy_EnemyStatusData->hitBox.center = &iEnemy_ModelRender->data.transform.pos;

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.colorData.color.x = 255;
	iEnemy_ModelRender->data.colorData.color.y = 255;
	iEnemy_ModelRender->data.colorData.color.z = 255;
	iEnemy_ModelRender->data.colorData.color.a = 0;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;
	//iEnemy_ModelRender->data.isPlayFlag = false;
	//iEnemy_ModelRender->data.isReverseFlag = false;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;


	iEnemy_ModelRender->data.transform.pos.z = -1000.0f;
	basePos = GENERATE_DATA.initPos;
	vel = 0.0f;
	appearTimer = 0;
	misileShotIndex = 0;

	isShotFlag = GENERATE_DATA.battleShipData.isShotFlag;
	if (isShotFlag)
	{
		disappearTime = 15;
	}
	else
	{
		disappearTime = 10;
	}

	adjHitBoxPos = { 0.0f,95.0f,-15.0f };


	if (signbit(iEnemy_ModelRender->data.transform.pos.x))
	{
		topRota.y = 90.0f;
	}
	else if(!signbit(iEnemy_ModelRender->data.transform.pos.x))
	{
		topRota.y = -90.0f;
	}
	if (iEnemy_ModelRender->data.transform.pos.x == 0.0f)
	{
		topRota.y = 180.0f;
	}

	topPos = { 0.0f,82.0f,-16.0f };
}

void BattleshipEnemy::Finalize()
{
}

void BattleshipEnemy::Update()
{
	hitBoxPos = {};
	hitBoxPos = model->data.transform.pos + adjHitBoxPos;
	iEnemy_EnemyStatusData->hitBox.center = &hitBoxPos;

	if (iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		//一定時間止まったらそのまま画面外に向かう
		if (KazMath::ConvertSecondToFlame(disappearTime) <= appearTimer && !demoFlag)
		{
			model->data.transform.pos.z += vel;
			vel += 0.1f;

			const float MAX_SPEED = 10.0f;
			if (MAX_SPEED <= vel)
			{
				vel = MAX_SPEED;

				if (0 < model->data.colorData.color.a)
				{
					model->data.colorData.color.a -= 5;
				}
				else
				{
					//死亡扱い
					model->data.colorData.color.a = 0;
					iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
					initDeadSoundFlag = true;
				}
			}
		}
		//ミサイル発射,目的地にたどり着く
		else if (isShotFlag && iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
		{

			int lBaseTimer = KazMath::ConvertSecondToFlame(8);
			int retuIndex = 0;
			int countIndex = 0;
			for (int i = 0; i < misileR.size(); ++i)
			{
				//ミサイル発射地点の設定--------------------------------------
				bool minusFlag = i % 2 != 0;
				if (2 <= countIndex)
				{
					countIndex = 1;
					++retuIndex;
				}
				else
				{
					++countIndex;
				}

				if (minusFlag)
				{
					misileR[i].data.transform.pos = model->data.transform.pos + KazMath::Vec3<float>(-5.0f, 88.0f, 25.0f) + KazMath::Vec3<float>(0.0f, 0.0f, 10.0f * retuIndex);
				}
				else
				{
					misileR[i].data.transform.pos = model->data.transform.pos + KazMath::Vec3<float>(5.0f, 88.0f, 25.0f) + KazMath::Vec3<float>(0.0f, 0.0f, 10.0f * retuIndex);
				}
				//ミサイル発射地点の設定--------------------------------------

				/*if (!model->data.isPlayFlag && !model->data.isReverseFlag && lBaseTimer - KazMath::ConvertSecondToFlame(3) <= appearTimer)
				{
					model->data.isPlayFlag = true;
				}
				if (!model->data.isReverseFlag && KazMath::ConvertSecondToFlame(11) <= appearTimer)
				{
					model->data.isReverseFlag = true;
					model->data.isPlayFlag = false;
				}*/

				//ミサイル発射
				if (appearTimer == lBaseTimer + i * 10)
				{
					iEnemy_EnemyStatusData->genarateData.enemyGenerateData.initPos = misileR[misileShotIndex].data.transform.pos;
					iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_BATTLESHIP_MISILE;
					if (i % 2 == 0)
					{
						iEnemy_EnemyStatusData->genarateData.enemyGenerateData.battleShipEnemyData.shotEnum = BATTLESHIP_MISILE_SHOT_RIGHT;
					}
					else
					{
						iEnemy_EnemyStatusData->genarateData.enemyGenerateData.battleShipEnemyData.shotEnum = BATTLESHIP_MISILE_SHOT_LEFT;
					}
					++misileShotIndex;
					break;
				}
			}
		}


		if (appearTimer < KazMath::ConvertSecondToFlame(disappearTime))
		{
			KazMath::Larp(basePos.z, &model->data.transform.pos.z, 0.01f);

			//登場処理
			if (model->data.colorData.color.a < 255)
			{
				model->data.colorData.color.a += 5;
			}
			else
			{
				model->data.colorData.color.a = 255;
			}
		}
	}

	if (!ProcessingOfDeath(DEATH_SINK))
	{
		++appearTimer;
	}
	else
	{
		topModel->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		topModel->data.removeMaterialFlag = true;
		topModel->data.colorData.color.x = 255;
		topModel->data.colorData.color.y = 255;
		topModel->data.colorData.color.z = 255;
	}

	topModel->data.transform = iEnemy_ModelRender->data.transform;
	topModel->data.colorData = iEnemy_ModelRender->data.colorData;
	topModel->data.transform.pos = iEnemy_ModelRender->data.transform.pos + topPos;
	topModel->data.transform.rotation = iEnemy_ModelRender->data.transform.rotation + topRota;
}

void BattleshipEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
	topModel->Draw();
	LockOnWindow(*iEnemy_EnemyStatusData->hitBox.center);
}
