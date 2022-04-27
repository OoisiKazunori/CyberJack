#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	iEnemy_ModelRender->data.transform.scale = { 10.0f,10.0f,10.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	iEnemy_ModelRender->data.color.w = 0.0f;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	if (iEnemy_EnemyStatusData->oprationObjData->rockOnNum <= 0)
	{
		iEnemy_ModelRender->data.color = { 255.0f,0.0f,0.0f,255.0f };
	}

	if (iEnemy_ModelRender->data.color.w < 255.0f)
	{
		iEnemy_ModelRender->data.color.w += 5.0f;
	}
	else
	{
		iEnemy_ModelRender->data.color.w = 255.0f;
	}
	

	//�ړ�
	float speed = 0.8f;
	iEnemy_ModelRender->data.transform.pos.m128_f32[2] += -speed;

	//���S���o����
	if (!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		//iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		DradEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.w);
	}
}

void NormalEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
