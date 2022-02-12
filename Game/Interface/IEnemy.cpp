#include "IEnemy.h"

IEnemy::IEnemy()
{
	//�`��̏�����----------------------------------------------------------------
	iEnemy_ModelRender = std::make_unique<BoxPolygonRender>();
	//�`��̏�����----------------------------------------------------------------

	//�G���̏�����----------------------------------------------------------------
	iEnemy_EnemyStatusData = std::make_unique<EnemyData>(
		&iEnemy_ModelRender->data.transform.pos,
		-1.0f,
		&iOperationData
	);
	//�G���̏�����----------------------------------------------------------------
}

void IEnemy::Dead()
{
	iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
}

const unique_ptr<EnemyData> &IEnemy::GetData()
{
	return iEnemy_EnemyStatusData;
}
