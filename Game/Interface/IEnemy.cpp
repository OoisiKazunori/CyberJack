#include "IEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

IEnemy::IEnemy()
{
	//•`‰æ‚Ì‰Šú‰»----------------------------------------------------------------
	iEnemy_ModelRender = std::make_unique<ObjModelRender>();
	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	//•`‰æ‚Ì‰Šú‰»----------------------------------------------------------------

	//“Gî•ñ‚Ì‰Šú‰»----------------------------------------------------------------
	iEnemy_EnemyStatusData = std::make_unique<EnemyData>(
		&iEnemy_ModelRender->data.transform.pos,
		-1.0f,
		&iOperationData
		);
	//“Gî•ñ‚Ì‰Šú‰»----------------------------------------------------------------

	lockOnWindowRender.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::LinePath + "LockOn.png");
	lockOnWindowRender.data.transform.scale = { 0.2f,0.2f };
	lockOnWindowRender.data.billBoardFlag = true;
	lockOnWindowRender.data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;

	deadSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "EnemyDead.wav", false);
	//damageSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "EnemyDamage.wav");
}

void IEnemy::Dead()
{
	iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
}

void IEnemy::DeadEffect(XMVECTOR *POS, XMVECTOR *ROTATION, float *ALPHA)
{
	if (*ALPHA <= 0)
	{
		*ALPHA = 0.0f;
	}
	else
	{
		XMVECTOR rota{ 5.0f,5.0f,5.0f };
		*ROTATION += rota;
		POS->m128_f32[1] -= 0.5f;
		*ALPHA -= 5.0f;
	}
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
