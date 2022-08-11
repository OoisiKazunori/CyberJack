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
	model.data.transform.pos = POS;	//���W�̏�����
	lerpPos = POS;	//���W�̏�����
	model.data.transform.scale = { 1.0f,1.0f,1.0f };
	model.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Gunner_Model.fbx");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	model.data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
	model.data.color.color.x = 255;
	model.data.color.color.y = 255;
	model.data.color.color.z = 255;
	model.data.color.color.a = 0;
	model.data.transform.rotation.x = 180.0f;
	model.data.transform.rotation.y = 0.0f;
	model.data.transform.rotation.z = 0.0f;
	initShotFlag = false;

	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	shotTimer = 0;

	rocketEffect.Init(&model.data.transform.pos);
}

void NormalMisileEnemy::Finalize()
{
}

void NormalMisileEnemy::Update()
{

	++model.data.transform.rotation.z;
	//����
	//�Q�[�����ōU�����[�V�����̋N��
	bool lGameShotFlag = !demoFlag && 120 <= shotTimer && !initShotFlag && iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag;
	bool lDebugShotFlag = demoFlag && debugShotFlag;

	if (lGameShotFlag || lDebugShotFlag)
	{
		iEnemy_EnemyStatusData->genarateData.initPos = model.data.transform.pos;
		iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;

		//�m�b�N�o�b�N
		KazMath::Vec3<float> kockBackVel = { 0.0f,0.0f,60.0f };
		lerpPos = model.data.transform.pos + kockBackVel;
		SoundManager::Instance()->PlaySoundMem(shotSoundHandle, 1);
		initShotFlag = true;
		debugShotFlag = false;
	}

	//���S���o����
	//�f�o�b�N�L�[or�����蔻���&&���S��
	if (EnableToHit(model.data.transform.pos.z) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		//model.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		//model.data.removeMaterialFlag = true;
		model.data.color.color.x = 255;
		model.data.color.color.y = 255;
		model.data.color.color.z = 255;
		DeadEffect(&model.data.transform.pos, &model.data.transform.rotation, &model.data.color.color.a);

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
		if (model.data.color.color.a < 255)
		{
			model.data.color.color.a += 5;
		}
		else
		{
			model.data.color.color.a = 255;
		}


		++shotTimer;
		KazMath::Vec3<float> vel = { 0.0f,0.0f,-1.0f };
		if (!demoFlag)
		{
			lerpPos += vel;
		}
	}

	KazMath::Larp(lerpPos.z, &model.data.transform.pos.z, 0.1f);

	if (!EnableToHit(model.data.transform.pos.z))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}


	rocketEffect.Update();

}

void NormalMisileEnemy::Draw()
{
	model.data.isPlay = true;
	model.Draw();
	rocketEffect.Draw();
	LockOnWindow(model.data.transform.pos);
}
