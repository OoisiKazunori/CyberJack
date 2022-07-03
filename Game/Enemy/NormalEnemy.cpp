#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const KazMath::Vec3<float> &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	iEnemy_ModelRender->data.transform.scale = { 10.0f,10.0f,10.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//�����蔻��̑傫���ύX
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

	initDeadSoundFlag = false;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	//�ړ�
	float speed = 0.5f;
	iEnemy_ModelRender->data.transform.pos.z += -speed;

	//���S���o����
	//�f�o�b�N�L�[or�����蔻���&&���S��
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z) &&!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
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
	}


	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.z))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}
}

void NormalEnemy::Draw()
{
	if (1.0f <= iEnemy_ModelRender->data.color.color.a)
	{
		iEnemy_ModelRender->Draw();
		LockOnWindow(iEnemy_ModelRender->data.transform.pos);
	}
}
