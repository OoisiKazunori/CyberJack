#include "NormalMisileEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Math/KazMath.h"

NormalMisileEnemy::NormalMisileEnemy()
{
	shotSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Shot.wav", false);
}

void NormalMisileEnemy::Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	lerpPos = POS;	//���W�̏�����
	iEnemy_ModelRender->data.transform.scale = { 10.0f,10.0f,10.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "MisileEnemy.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.color.color.x = 255;
	iEnemy_ModelRender->data.color.color.y = 255;
	iEnemy_ModelRender->data.color.color.z = 255;
	iEnemy_ModelRender->data.color.color.a = 0;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 90.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;
	initShotFlag = false;

	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	shotTimer = 0;
}

void NormalMisileEnemy::Finalize()
{
}

void NormalMisileEnemy::Update()
{
	//����
	//�Q�[�����ōU�����[�V�����̋N��
	bool lGameShotFlag = !demoFlag && 120 <= shotTimer && !initShotFlag && iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag;
	bool lDebugShotFlag = demoFlag && debugShotFlag;

	if (lGameShotFlag || lDebugShotFlag)
	{
		iEnemy_EnemyStatusData->genarateData.initPos = iEnemy_ModelRender->data.transform.pos;
		iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;

		//�m�b�N�o�b�N
		KazMath::Vec3<float> kockBackVel = { 0.0f,0.0f,60.0f };
		lerpPos = iEnemy_ModelRender->data.transform.pos + kockBackVel;
		SoundManager::Instance()->PlaySoundMem(shotSoundHandle, 1);
		initShotFlag = true;
		debugShotFlag = false;
	}

	//���S���o����
	//�f�o�b�N�L�[or�����蔻���&&���S��
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.color.color.x = 255;
		iEnemy_ModelRender->data.color.color.y = 255;
		iEnemy_ModelRender->data.color.color.z = 255;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.color.a);

		if (!initDeadSoundFlag)
		{
			DeadSound();
			initDeadSoundFlag = true;
		}
	}
	//���S���o���ɓo�ꉉ�o�͍s��Ȃ�
	else
	{
		//�o�ꏈ��
		if (iEnemy_ModelRender->data.color.color.a < 255)
		{
			iEnemy_ModelRender->data.color.color.a += 5;
		}
		else
		{
			iEnemy_ModelRender->data.color.color.a = 255;
		}


		++shotTimer;
		KazMath::Vec3<float> vel = { 0.0f,0.0f,-1.0f };
		if (!demoFlag)
		{
			lerpPos += vel;
		}
	}

	KazMath::Larp(lerpPos.z, &iEnemy_ModelRender->data.transform.pos.z, 0.1f);

	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.z))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}

}

void NormalMisileEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
	LockOnWindow(iEnemy_ModelRender->data.transform.pos);
}
