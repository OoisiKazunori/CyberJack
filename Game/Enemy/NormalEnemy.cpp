#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ObjModelRender->data.transform.pos = GENERATE_DATA.initPos;	//���W�̏�����
	iEnemy_ObjModelRender->data.transform.scale = { 1.0f,1.0f,1.0f };
	iEnemy_ObjModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath +"Move/"+ "MoveEnemy_Model.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1, "gw-1");							//�c�胍�b�N�I�������̏�����

	iEnemy_ObjModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ObjModelRender->data.removeMaterialFlag = false;
	iEnemy_ObjModelRender->data.colorData.color.x = 255;
	iEnemy_ObjModelRender->data.colorData.color.y = 255;
	iEnemy_ObjModelRender->data.colorData.color.z = 255;
	iEnemy_ObjModelRender->data.colorData.color.a = 1;
	iEnemy_ObjModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ObjModelRender->data.transform.rotation.y = 180.0f;
	iEnemy_ObjModelRender->data.transform.rotation.z = 0.0f;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	speed = GENERATE_DATA.speed;
	iEnemy_EnemyStatusData->radius = 8.0f;
	iEnemy_EnemyStatusData->startFlag = true;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	++iEnemy_ObjModelRender->data.transform.rotation.z;
	//�ړ�
	if (!demoFlag)
	{
		iEnemy_ObjModelRender->data.transform.pos.z += speed;
	}

	//���S���o���ɓo�ꉉ�o�͍s��Ȃ�
	if (!ProcessingOfDeath(DEATH_ROLL))
	{
		//�o�ꏈ��
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

		iEnemy_ObjModelRender->Draw();
		LockOnWindow(iEnemy_ObjModelRender->data.transform.pos);
	}
}
