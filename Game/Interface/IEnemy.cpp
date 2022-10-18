#include "IEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

IEnemy::IEnemy() :hpDirtyFlag(&iOperationData.rockOnNum)
{
	//�`��̏�����----------------------------------------------------------------
	iEnemy_ObjModelRender = std::make_unique<ObjModelRender>();
	iEnemy_ObjModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;

	iEnemy_FbxModelRender = std::make_unique<FbxModelRender>();
	iEnemy_FbxModelRender->data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
	//�`��̏�����----------------------------------------------------------------

	//�G���̏�����----------------------------------------------------------------
	iEnemy_EnemyStatusData = std::make_unique<EnemyData>(
		&iEnemy_ObjModelRender->data.transform.pos,
		-1.0f,
		&iOperationData
		);
	//�G���̏�����----------------------------------------------------------------

	lockOnWindowRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::LinePath + "LockOn.png");
	lockOnWindowRender.data.transform.scale = { 0.3f,0.3f };
	lockOnWindowRender.data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;

	deadSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "EnemyDead.wav", false);
	shotSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Shot.wav", false);

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
		iEnemy_ObjModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		iEnemy_ObjModelRender->data.removeMaterialFlag = true;
		iEnemy_ObjModelRender->data.colorData.color.x = 255;
		iEnemy_ObjModelRender->data.colorData.color.y = 255;
		iEnemy_ObjModelRender->data.colorData.color.z = 255;

		if (!initDeadSoundFlag)
		{
			DeadSound();
			initDeadSoundFlag = true;
		}

		switch (TYPE)
		{
		case DEATH_ROLL:
			DeadEffect(&iEnemy_ObjModelRender->data.transform.pos, &iEnemy_ObjModelRender->data.transform.rotation, &iEnemy_ObjModelRender->data.colorData.color.a);
			break;
		case DEATH_SINK:
			iEnemy_ObjModelRender->data.transform.pos.y -= 0.5f;
			iEnemy_ObjModelRender->data.transform.rotation.x += 0.5f;
			iEnemy_ObjModelRender->data.colorData.color.a -= 5;
			break;
		default:
			break;
		}
	}

	if (iEnemy_ObjModelRender->data.colorData.color.a <= 0)
	{
		iEnemy_ObjModelRender->data.colorData.color.a = 0;
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}

	return !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag;
}

bool IEnemy::ProcessingOfDeathFbx(EnemyDeathType TYPE)
{
	if (!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_FbxModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		iEnemy_FbxModelRender->data.removeMaterialFlag = true;
		iEnemy_FbxModelRender->data.colorData.color.x = 255;
		iEnemy_FbxModelRender->data.colorData.color.y = 255;
		iEnemy_FbxModelRender->data.colorData.color.z = 255;

		if (!initDeadSoundFlag)
		{
			DeadSound();
			initDeadSoundFlag = true;
		}

		switch (TYPE)
		{
		case DEATH_ROLL:
			DeadEffect(&iEnemy_FbxModelRender->data.transform.pos, &iEnemy_FbxModelRender->data.transform.rotation, &iEnemy_FbxModelRender->data.colorData.color.a);
			break;
		case DEATH_SINK:
			iEnemy_FbxModelRender->data.transform.pos.y -= 0.5f;
			iEnemy_FbxModelRender->data.transform.rotation.x += 0.5f;
			iEnemy_FbxModelRender->data.colorData.color.a -= 5;
			break;
		default:
			break;
		}
	}

	if (iEnemy_FbxModelRender->data.colorData.color.a <= 0)
	{
		iEnemy_FbxModelRender->data.colorData.color.a = 0;
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}

	return !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag;
}

const unique_ptr<EnemyData> &IEnemy::GetData()
{
	return iEnemy_EnemyStatusData;
}

void IEnemy::DeadSound()
{
	SoundManager::Instance()->PlaySoundMem(deadSoundHandle, 1);
}

void IEnemy::ShotSound()
{
	SoundManager::Instance()->PlaySoundMem(shotSoundHandle, 1);
}

void IEnemy::InitModel(const KazMath::Transform3D &TRANSFORM, const std::string &MODEL_FILEPASS, float HITBOX_RADIUS, bool FBX_OR_OBJ_FLAG, bool REV_UV_FLAG)
{
	if (FBX_OR_OBJ_FLAG)
	{
		iEnemy_FbxModelRender->data.handle = FbxModelResourceMgr::Instance()->LoadModel(MODEL_FILEPASS, REV_UV_FLAG);	//���f���ǂݍ���
		iEnemy_FbxModelRender->data.transform = TRANSFORM;
		iEnemy_EnemyStatusData->hitBox.radius = HITBOX_RADIUS;	//�����蔻��̑傫���ύX
		iEnemy_EnemyStatusData->hitBox.center = &iEnemy_FbxModelRender->data.transform.pos;
		iEnemy_FbxModelRender->data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;

		iEnemy_FbxModelRender->data.removeMaterialFlag = false;
		iEnemy_FbxModelRender->data.colorData.color.x = 255;
		iEnemy_FbxModelRender->data.colorData.color.y = 255;
		iEnemy_FbxModelRender->data.colorData.color.z = 255;
		iEnemy_FbxModelRender->data.colorData.color.a = 1;

		iEnemy_FbxModelRender->data.isPlayFlag = false;
	}
	else
	{
		iEnemy_ObjModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(MODEL_FILEPASS);	//���f���ǂݍ���
		iEnemy_ObjModelRender->data.transform = TRANSFORM;
		iEnemy_EnemyStatusData->hitBox.radius = HITBOX_RADIUS;	//�����蔻��̑傫���ύX
		iEnemy_EnemyStatusData->hitBox.center = &iEnemy_ObjModelRender->data.transform.pos;
		iEnemy_ObjModelRender->data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;

		iEnemy_ObjModelRender->data.removeMaterialFlag = false;
		iEnemy_ObjModelRender->data.colorData.color.x = 255;
		iEnemy_ObjModelRender->data.colorData.color.y = 255;
		iEnemy_ObjModelRender->data.colorData.color.z = 255;
		iEnemy_ObjModelRender->data.colorData.color.a = 1;
	}
}
