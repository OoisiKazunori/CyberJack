#include "NormalEnemy.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//座標の初期化
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化

	iEnemy_EnemyStatusData->genarateData.enemyType = 1;
	iEnemy_EnemyStatusData->genarateData.generateNum = 1;
	iEnemy_EnemyStatusData->genarateData.intervalFlame = 0;
	iEnemy_EnemyStatusData->genarateData.initPos = { 30.0f, 30.0f,30.0f };
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
