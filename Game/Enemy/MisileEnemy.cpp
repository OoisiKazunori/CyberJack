#include "MisileEnemy.h"

MisileEnemy::MisileEnemy()
{
}

void MisileEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;
}

void MisileEnemy::Finalize()
{
}

void MisileEnemy::Update()
{

}

void MisileEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
