#include "IEnemy.h"

IEnemy::IEnemy()
{
	iEnemy_EnemyStatusData = std::make_unique<EnemyData>(iOprationObject_hitBox, iOprationObject_rockOnNum);
	iEnemy_ModelRender = std::make_unique<BoxPolygonRender>();
}

void IEnemy::Dead()
{
	iEnemy_EnemyStatusData->deadFlag = true;
}

const unique_ptr<EnemyData> &IEnemy::GetData()
{
	return iEnemy_EnemyStatusData;
}
