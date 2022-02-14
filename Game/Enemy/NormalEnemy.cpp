#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	iEnemy_EnemyStatusData->genarateData.enemyType = 1;
	iEnemy_EnemyStatusData->genarateData.initPos = { 30.0f, 30.0f,30.0f };

	generateTimer = 0;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	if (120 <= generateTimer && generateTimer < 121)
	{
		iEnemy_EnemyStatusData->genarateData.enemyType = 1;
		iEnemy_EnemyStatusData->genarateData.initPos= { -30.0f, 30.0f,30.0f };
	}

	++generateTimer;
}

void NormalEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
