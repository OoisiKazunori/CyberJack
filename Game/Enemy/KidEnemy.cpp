#include "KidEnemy.h"

KidEnemy::KidEnemy()
{
}

void KidEnemy::Init(const KazMath::Vec3<float> &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//座標の初期化
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(4);							//残りロックオン数等の初期化

	iEnemy_ModelRender->data.color = { 255.0f,0.0f,0.0f,255.0f };
}

void KidEnemy::Finalize()
{
}

void KidEnemy::Update()
{
	
}

void KidEnemy::Draw()
{
	//iEnemy_ModelRender->Draw();
}
