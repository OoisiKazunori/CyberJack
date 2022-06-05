#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	iEnemy_ModelRender->data.transform.scale = { 10.0f,10.0f,10.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//�����蔻��̑傫���ύX
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

}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	//�ړ�
	float speed = 0.5f;
	iEnemy_ModelRender->data.transform.pos.m128_f32[2] += -speed;

	//���S���o����
	//�f�o�b�N�L�[or�����蔻���&&���S��
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.m128_f32[2]) &&!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.color.x = 255.0f;
		iEnemy_ModelRender->data.color.y = 255.0f;
		iEnemy_ModelRender->data.color.z = 255.0f;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.w);
	}
	//���S���o���ɓo�ꉉ�o�͍s��Ȃ�
	else
	{
		//�o�ꏈ��
		if (iEnemy_ModelRender->data.color.w < 255.0f)
		{
			iEnemy_ModelRender->data.color.w += 5.0f;
		}
		else
		{
			iEnemy_ModelRender->data.color.w = 255.0f;
		}
	}


	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.m128_f32[2]))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
	}
}

void NormalEnemy::Draw()
{
	if (1.0f <= iEnemy_ModelRender->data.color.w)
	{
		iEnemy_ModelRender->Draw();
		LockOnWindow();
	}
}
