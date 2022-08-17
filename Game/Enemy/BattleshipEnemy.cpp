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

	model = std::make_unique<FbxModelRender>();
	model->data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "BattleShip/" + "BlenderBattleShip.fbx");	//モデル読み込み
}

void BattleshipEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	model->data.transform.pos = GENERATE_DATA.initPos;	//座標の初期化
	model->data.transform.scale = { 0.5f,0.5f,0.5f };
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化
	iEnemy_EnemyStatusData->hitBox.center = &model->data.transform.pos;


	model->data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
	//model->data.removeMaterialFlag = false;
	model->data.colorData.color.x = 255;
	model->data.colorData.color.y = 255;
	model->data.colorData.color.z = 255;
	model->data.colorData.color.a = 0;
	model->data.transform.rotation.x = 0.0f;
	model->data.transform.rotation.y = 0.0f;
	model->data.transform.rotation.z = 0.0f;
	model->data.isPlayFlag = false;
	model->data.isReverseFlag = false;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	model->data.transform.pos.z = -1000.0f;
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
	hitBoxPos = model->data.transform.pos + adjHitBoxPos;
	iEnemy_EnemyStatusData->hitBox.center = &hitBoxPos;

	//一定時間止まったらそのまま画面外に向かう
	if (KazMath::ConvertSecondToFlame(disappearTime) <= appearTimer&&false)
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
				model->data.colorData.color.a = 0;
				iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
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

			if (!model->data.isPlayFlag && !model->data.isReverseFlag && lBaseTimer - KazMath::ConvertSecondToFlame(3) <= appearTimer)
			{
				model->data.isPlayFlag = true;
			}
			if (!model->data.isReverseFlag && KazMath::ConvertSecondToFlame(11) <= appearTimer)
			{
				model->data.isReverseFlag = true;
				model->data.isPlayFlag = false;
			}

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


	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		//iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		//iEnemy_ModelRender->data.removeMaterialFlag = true;
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

	ImGui::Begin("Misile");
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
	ImGui::End();

	++appearTimer;
}

void BattleshipEnemy::Draw()
{
	for (int i = 0; i < misileR.size(); ++i)
	{
		misileR[i].Draw();
	}
	model->Draw();
}
