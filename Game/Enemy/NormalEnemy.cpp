#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;
	iOperationData.Init(1);
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
