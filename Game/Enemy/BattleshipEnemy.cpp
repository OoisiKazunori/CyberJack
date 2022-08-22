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

	//iEnemy_ModelRender = std::make_unique<FbxModelRender>();
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "BattleShip/" + "BattleshipEnemy_Model.obj");	//モデル読み込み
}

void BattleshipEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = GENERATE_DATA.initPos;	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 0.5f,0.5f,0.5f };
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化
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
}

void BattleshipEnemy::Finalize()
{
}

void BattleshipEnemy::Update()
{
	hitBoxPos = {};
	hitBoxPos = iEnemy_ModelRender->data.transform.pos + adjHitBoxPos;
	iEnemy_EnemyStatusData->hitBox.center = &hitBoxPos;

	if (iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		//一定時間止まったらそのまま画面外に向かう
		if (KazMath::ConvertSecondToFlame(disappearTime) <= appearTimer)
		{
			iEnemy_ModelRender->data.transform.pos.z += vel;
			vel += 0.1f;

			const float MAX_SPEED = 10.0f;
			if (MAX_SPEED <= vel)
			{
				vel = MAX_SPEED;

				if (0 < iEnemy_ModelRender->data.colorData.color.a)
				{
					iEnemy_ModelRender->data.colorData.color.a -= 5;
				}
				else
				{
					//死亡扱い
					iEnemy_ModelRender->data.colorData.color.a = 0;
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
					misileR[i].data.transform.pos = iEnemy_ModelRender->data.transform.pos + KazMath::Vec3<float>(-5.0f, 88.0f, 25.0f) + KazMath::Vec3<float>(0.0f, 0.0f, 10.0f * retuIndex);
				}
				else
				{
					misileR[i].data.transform.pos = iEnemy_ModelRender->data.transform.pos + KazMath::Vec3<float>(5.0f, 88.0f, 25.0f) + KazMath::Vec3<float>(0.0f, 0.0f, 10.0f * retuIndex);
				}
				//ミサイル発射地点の設定--------------------------------------

				/*if (!iEnemy_ModelRender->data.isPlayFlag && !iEnemy_ModelRender->data.isReverseFlag && lBaseTimer - KazMath::ConvertSecondToFlame(3) <= appearTimer)
				{
					iEnemy_ModelRender->data.isPlayFlag = true;
				}
				if (!iEnemy_ModelRender->data.isReverseFlag && KazMath::ConvertSecondToFlame(11) <= appearTimer)
				{
					iEnemy_ModelRender->data.isReverseFlag = true;
					iEnemy_ModelRender->data.isPlayFlag = false;
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
			KazMath::Larp(basePos.z, &iEnemy_ModelRender->data.transform.pos.z, 0.01f);

			//登場処理
			if (iEnemy_ModelRender->data.colorData.color.a < 255)
			{
				iEnemy_ModelRender->data.colorData.color.a += 5;
			}
			else
			{
				iEnemy_ModelRender->data.colorData.color.a = 255;
			}
		}
	}


	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z, -5000.0f) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.colorData.color.x = 255;
		iEnemy_ModelRender->data.colorData.color.y = 255;
		iEnemy_ModelRender->data.colorData.color.z = 255;

		iEnemy_ModelRender->data.transform.pos.y -= 0.5f;
		iEnemy_ModelRender->data.colorData.color.a -= 5;

		iEnemy_ModelRender->data.transform.rotation.x += 0.5f;
		iEnemy_ModelRender->data.transform.rotation.y += 0.5f;

		if (iEnemy_ModelRender->data.colorData.color.a < 0)
		{
			iEnemy_ModelRender->data.colorData.color.a = 0;
		}

		if (!initDeadSoundFlag)
		{
			DeadSound();
			initDeadSoundFlag = true;
		}
	}
	else
	{
		++appearTimer;
	}

	/*ImGui::Begin("Misile");
	for (int i = 0; i < misileR.size(); ++i)
	{
		misileR[i].data.color.color = { 255,0,0,255 };
		std::string name = "POS";
		std::string tag = name + "X" + std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &misileR[i].data.transform.pos.x);
		tag = name + "Y" + std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &misileR[i].data.transform.pos.y);
		tag = name + "Z" + std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &misileR[i].data.transform.pos.z);
	}
	ImGui::End();*/

}

void BattleshipEnemy::Draw()
{
	//for (int i = 0; i < misileR.size(); ++i)
	//{
	//	misileR[i].Draw();
	//}
	iEnemy_ModelRender->Draw();
}
