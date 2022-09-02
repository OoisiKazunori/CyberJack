#include "IEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

IEnemy::IEnemy()
{
	//描画の初期化----------------------------------------------------------------
	iEnemy_ModelRender = std::make_unique<ObjModelRender>();
	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	//描画の初期化----------------------------------------------------------------

	//敵情報の初期化----------------------------------------------------------------
	iEnemy_EnemyStatusData = std::make_unique<EnemyData>(
		&iEnemy_ModelRender->data.transform.pos,
		-1.0f,
		&iOperationData
		);
	//敵情報の初期化----------------------------------------------------------------

	lockOnWindowRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::LinePath + "LockOn.png");
	lockOnWindowRender.data.transform.scale = { 0.3f,0.3f };
	//lockOnWindowRender.data.billBoardFlag = true;
	lockOnWindowRender.data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;

	deadSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "EnemyDead.wav", false);
	//damageSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "EnemyDamage.wav");

	debugShotFlag = false;
}

void IEnemy::Dead()
{
	iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
}

void IEnemy::DeadEffect(KazMath::Vec3<float> *POS, KazMath::Vec3<float> *ROTATION, int *ALPHA)
{
	if (*ALPHA <= 0)
	{
		*ALPHA = 0;
	}
	else
	{
		KazMath::Vec3<float> rota{ 5.0f,5.0f,5.0f };
		*ROTATION += rota;
		POS->y -= 0.5f;
		*ALPHA -= 5;
	}
}

bool IEnemy::ProcessingOfDeath(EnemyDeathType TYPE)
{
	if (!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.colorData.color.x = 255;
		iEnemy_ModelRender->data.colorData.color.y = 255;
		iEnemy_ModelRender->data.colorData.color.z = 255;

		if (!initDeadSoundFlag)
		{
			DeadSound();
			initDeadSoundFlag = true;
		}

		switch (TYPE)
		{
		case DEATH_ROLL:
			DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.colorData.color.a);
			break;
		case DEATH_SINK:
			iEnemy_ModelRender->data.transform.pos.y -= 0.5f;
			iEnemy_ModelRender->data.transform.rotation.x += 0.5f;
			iEnemy_ModelRender->data.colorData.color.a -= 5;
			break;
		default:
			break;
		}
	}

	if (iEnemy_ModelRender->data.colorData.color.a <= 0)
	{
		iEnemy_ModelRender->data.colorData.color.a = 0;
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}

	return !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag;
}

const unique_ptr<EnemyData> &IEnemy::GetData()
{
	return iEnemy_EnemyStatusData;
}

void IEnemy::HitSound()
{
	SoundManager::Instance()->PlaySoundMem(damageSoundHandle, 1);
}

void IEnemy::DeadSound()
{
	SoundManager::Instance()->PlaySoundMem(deadSoundHandle, 1);
}
