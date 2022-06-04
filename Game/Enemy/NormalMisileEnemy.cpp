#include "NormalMisileEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Math/KazMath.h"

NormalMisileEnemy::NormalMisileEnemy()
{
}

void NormalMisileEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	lerpPos = POS;	//���W�̏�����
	iEnemy_ModelRender->data.transform.scale = { 10.0f,10.0f,10.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.color.x = 255.0f;
	iEnemy_ModelRender->data.color.y = 255.0f;
	iEnemy_ModelRender->data.color.z = 255.0f;
	iEnemy_ModelRender->data.color.w = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.m128_f32[0] = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.m128_f32[1] = 90.0f;
	iEnemy_ModelRender->data.transform.rotation.m128_f32[2] = 0.0f;
	initShotFlag = false;
}

void NormalMisileEnemy::Finalize()
{
}

void NormalMisileEnemy::Update()
{
	//����
	if (120 <= shotTimer && !initShotFlag)
	{
		iEnemy_EnemyStatusData->genarateData.initPos = iEnemy_ModelRender->data.transform.pos;
		iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;

		//�m�b�N�o�b�N
		XMVECTOR kockBackVel = { 0.0f,0.0f,60.0f };
		lerpPos = iEnemy_ModelRender->data.transform.pos + kockBackVel;
		initShotFlag = true;
	}

	//�o�ꏈ��
	if (iEnemy_ModelRender->data.color.w < 255.0f)
	{
		iEnemy_ModelRender->data.color.w += 5.0f;
	}
	else
	{
		iEnemy_ModelRender->data.color.w = 255.0f;
	}


	++shotTimer;

	XMVECTOR vel = { 0.0f,0.0f,-1.0f };
	lerpPos += vel;

	KazMath::Larp(lerpPos.m128_f32[2], &iEnemy_ModelRender->data.transform.pos.m128_f32[2], 0.1f);

}

void NormalMisileEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
