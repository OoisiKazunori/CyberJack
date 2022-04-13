#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	//iEnemy_EnemyStatusData->genarateData.enemyType = 1;
	//iEnemy_EnemyStatusData->genarateData.initPos = { 30.0f, 30.0f,30.0f };

	generateTimer = 0;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	if (120 <= generateTimer && generateTimer < 121)
	{
		//iEnemy_EnemyStatusData->genarateData.enemyType = 1;
		//iEnemy_EnemyStatusData->genarateData.initPos= { -30.0f, 30.0f,30.0f };
	}

	if (iEnemy_EnemyStatusData->oprationObjData->rockOnNum <= 0)
	{
		iEnemy_ModelRender->data.color = { 255.0f,0.0f,0.0f,255.0f };
	}


	++generateTimer;
}

void NormalEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
