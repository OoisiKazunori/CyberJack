#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = GENERATE_DATA.initPos;	//���W�̏�����
	iEnemy_ModelRender->data.transform.scale = { 1.0f,1.0f,1.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath +"Move/"+ "MoveEnemy_Model.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.colorData.color.x = 255;
	iEnemy_ModelRender->data.colorData.color.y = 255;
	iEnemy_ModelRender->data.colorData.color.z = 255;
	iEnemy_ModelRender->data.colorData.color.a = 0;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 180.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	speed = GENERATE_DATA.speed;

	rocketEffect.Init(&iEnemy_ModelRender->data.transform.pos, KazMath::Vec3<float>(0.0f, 0.0f, 20.0f), KazMath::Vec3<float>(0.0f, 0.0f, 45.0f));
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	++iEnemy_ModelRender->data.transform.rotation.z;
	//�ړ�
	if (!demoFlag)
	{
		iEnemy_ModelRender->data.transform.pos.z += speed;
	}

	//���S���o����
	//�f�o�b�N�L�[or�����蔻���&&���S��
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
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
	//���S���o���ɓo�ꉉ�o�͍s��Ȃ�
	else 
	{
		//�o�ꏈ��
		if (iEnemy_ModelRender->data.colorData.color.a < 255)
		{
			iEnemy_ModelRender->data.colorData.color.a += 5;
		}
		else
		{
			iEnemy_ModelRender->data.colorData.color.a = 255;
		}
	}

	rocketEffect.Update();

	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.z))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}
}

void NormalEnemy::Draw()
{
	if (1.0f <= iEnemy_ModelRender->data.colorData.color.a)
	{
		iEnemy_ModelRender->Draw();
		//rocketEffect.Draw();
		LockOnWindow(iEnemy_ModelRender->data.transform.pos);
	}
}
