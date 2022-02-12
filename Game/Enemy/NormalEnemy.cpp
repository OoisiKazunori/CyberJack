#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//座標の初期化
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);	//残りロックオン数等の初期化
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
