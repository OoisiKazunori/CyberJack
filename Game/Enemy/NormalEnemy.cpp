#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	EnemyModelType lModelType = ENEMY_MODEL_FBX;
	if (GENERATE_DATA.useMeshPaticleFlag)
	{
		lModelType = ENEMY_MODEL_MESHPARTICLE;
	}
	InitModel(KazMath::Transform3D(GENERATE_DATA.initPos, { 1.0f,1.0f,1.0f }, { 0.0f,180.0f,0.0f }), KazFilePathName::EnemyPath + "Move/" + "MoveEnemy_Model.obj", 15.0f, lModelType);
	iOperationData.Init(1, "gw-1");							//残りロックオン数等の初期化


	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	speed = GENERATE_DATA.speed;
	iEnemy_EnemyStatusData->startFlag = true;
	iEnemy_EnemyStatusData->objFlag = true;
	iEnemy_EnemyStatusData->radius = 10.0f;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	++iEnemy_ObjModelRender->data.transform.rotation.z;
	//移動
	if (!demoFlag)
	{
		iEnemy_ObjModelRender->data.transform.pos.z += speed;
	}

	//死亡演出中に登場演出は行わない
	if (!ProcessingOfDeath(DEATH_ROLL))
	{
		//登場処理
		if (iEnemy_ObjModelRender->data.colorData.color.a < 255)
		{
			iEnemy_ObjModelRender->data.colorData.color.a += 5;
		}
		else
		{
			iEnemy_ObjModelRender->data.colorData.color.a = 255;
		}
	}

	if (iEnemy_ObjModelRender->data.transform.pos.z <= -50.0f)
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}

}

void NormalEnemy::Draw()
{
	if (1.0f <= iEnemy_ObjModelRender->data.colorData.color.a)
	{
		if (!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
		{
			iEnemy_ObjModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		}
		//iEnemy_ObjModelRender->Draw(!iEnemy_EnemyStatusData->meshParticleFlag);
		LockOnWindow(iEnemy_ObjModelRender->data.transform.pos);
	}
}
