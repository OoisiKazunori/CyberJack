#include "IEnemy.h"

IEnemy::IEnemy()
{
	//•`‰æ‚Ì‰Šú‰»----------------------------------------------------------------
	iEnemy_ModelRender = std::make_unique<BoxPolygonRender>();
	//•`‰æ‚Ì‰Šú‰»----------------------------------------------------------------

	//“Gî•ñ‚Ì‰Šú‰»----------------------------------------------------------------
	iEnemy_EnemyStatusData = std::make_unique<EnemyData>(
		&iEnemy_ModelRender->data.transform.pos,
		-1.0f,
		&iOperationData
	);
	//“Gî•ñ‚Ì‰Šú‰»----------------------------------------------------------------
}

void IEnemy::Dead()
{
	iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
}

const unique_ptr<EnemyData> &IEnemy::GetData()
{
	return iEnemy_EnemyStatusData;
}
