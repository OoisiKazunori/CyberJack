#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);	//�c�胍�b�N�I�������̏�����
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
}

void NormalEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
