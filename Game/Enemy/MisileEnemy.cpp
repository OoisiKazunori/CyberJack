#include "MisileEnemy.h"

MisileEnemy::MisileEnemy()
{
}

void MisileEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;
	iEnemy_EnemyStatusData->timer = 120;
	iOperationData.Init(1);
}

void MisileEnemy::Finalize()
{
}

void MisileEnemy::Update()
{
	--iEnemy_EnemyStatusData->timer;

	//Ž€–Sˆ—
	if (!iOperationData.enableToHitFlag)
	{
		iOperationData.initFlag = false;
	}
}

void MisileEnemy::Draw()
{
	//iEnemy_ModelRender->Draw();
}
