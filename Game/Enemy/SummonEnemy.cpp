#include "SummonEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Easing/easing.h"

SummonEnemy::SummonEnemy()
{
}

void SummonEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	basePos = GENERATE_DATA.initPos;
	iEnemy_ModelRender->data.transform.pos = GENERATE_DATA.initPos + KazMath::Vec3<float>(0.0f, 0.0f, 30.0f);	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 0.5f,0.5f,0.5f };
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化
	iEnemy_EnemyStatusData->hitBox.center = &iEnemy_ModelRender->data.transform.pos;

	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Summon/" + "SummonEnemy_Model.obj");	//モデル読み込み

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.colorData.color.x = 255;
	iEnemy_ModelRender->data.colorData.color.y = 255;
	iEnemy_ModelRender->data.colorData.color.z = 255;
	iEnemy_ModelRender->data.colorData.color.a = 0;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 180.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;
	//iEnemy_ModelRender->data.isPlayFlag = false;
	//iEnemy_ModelRender->data.isReverseFlag = false;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;



	summonPos[0] = { -30.0f,-18.0f,12.0f };
	summonPos[1] = { 30.0f,-18.0f,12.0f };
	appearTimer = 0;
	disappearTime = 10;


	summonRate = 0.0f;
	disappearRate = 0.0f;
	rotaMaxAngle = 15.0f;
}

void SummonEnemy::Finalize()
{
}

void SummonEnemy::Update()
{

	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.z) || !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
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
		++appearTimer;
		if (appearTimer < KazMath::ConvertSecondToFlame(2))
		{
			status = SummonEnemy::APPEAR;
		}
		else if (KazMath::ConvertSecondToFlame(2) < appearTimer && appearTimer < KazMath::ConvertSecondToFlame(disappearTime))
		{
			status = SummonEnemy::SUMMON;
		}
		else
		{
			status = SummonEnemy::DISAPPEAR;
		}

		summonR[0].data.transform.pos = iEnemy_ModelRender->data.transform.pos + summonPos[0];
		summonR[1].data.transform.pos = iEnemy_ModelRender->data.transform.pos + summonPos[1];


		float disappearPosZ = basePos.z + 30.0f;
		float baseTimer = 120 + 120;
		switch (status)
		{
		case SummonEnemy::APPEAR:
			KazMath::Larp(basePos.z, &iEnemy_ModelRender->data.transform.pos.z, 0.01f);
			if (iEnemy_ModelRender->data.colorData.color.a < 255)
			{
				iEnemy_ModelRender->data.colorData.color.a += 5;
			}
			else
			{
				iEnemy_ModelRender->data.colorData.color.a = 255;
			}
			break;
		case SummonEnemy::SUMMON:

			Rate(&summonRate, 0.01f, 1.0f);
			iEnemy_ModelRender->data.transform.rotation.x = 0.0f + EasingMaker(In, Back, summonRate) * -rotaMaxAngle;

			for (int i = 0; i < 20; ++i)
			{
				if (appearTimer == baseTimer + i * 30)
				{
					iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_POP;
					if (i % 2 == 0)
					{
						iEnemy_EnemyStatusData->genarateData.enemyGenerateData.initPos = summonR[0].data.transform.pos;
						iEnemy_EnemyStatusData->genarateData.enemyGenerateData.popEnemyData.leftRightFlag = true;
					}
					else
					{
						iEnemy_EnemyStatusData->genarateData.enemyGenerateData.initPos = summonR[1].data.transform.pos;
						iEnemy_EnemyStatusData->genarateData.enemyGenerateData.popEnemyData.leftRightFlag = false;
					}
					break;
				}
			}


			if (KazMath::ConvertSecondToFlame(disappearTime) - KazMath::ConvertSecondToFlame(2) < appearTimer)
			{
				Rate(&disappearRate, 0.01f, 1.0f);
				iEnemy_ModelRender->data.transform.rotation.x = -rotaMaxAngle + EasingMaker(Out, Back, disappearRate) * rotaMaxAngle;
			}
			break;

		case SummonEnemy::DISAPPEAR:

			KazMath::Larp(disappearPosZ, &iEnemy_ModelRender->data.transform.pos.z, 0.01f);
			if (0 < iEnemy_ModelRender->data.colorData.color.a)
			{
				iEnemy_ModelRender->data.colorData.color.a += -5;
			}
			else
			{
				iEnemy_ModelRender->data.colorData.color.a = 0;
			}
			break;
		default:
			break;
		}

	//	ImGui::Begin("Summon");
	//	ImGui::DragFloat("PosX1", &summonR[0].data.transform.pos.x);
	//	ImGui::DragFloat("PosY1", &summonR[0].data.transform.pos.y);
	//	ImGui::DragFloat("PosZ1", &summonR[0].data.transform.pos.z);
	//	ImGui::DragFloat("PosX2", &summonR[1].data.transform.pos.x);
	//	ImGui::DragFloat("PosY2", &summonR[1].data.transform.pos.y);
	//	ImGui::DragFloat("PosZ2", &summonR[1].data.transform.pos.z);
	//	ImGui::End();
	}
}

void SummonEnemy::Draw()
{
	iEnemy_ModelRender->Draw();

	//for (int i = 0; i < summonR.size(); ++i)
	//{
	//	summonR[i].Draw();
	//}
}
