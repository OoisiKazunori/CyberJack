#include "IEnemy.h"

IEnemy::IEnemy()
{
	//�`��̏�����----------------------------------------------------------------
	iEnemy_ModelRender = std::make_unique<ObjModelRender>();
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

void IEnemy::DradEffect(XMVECTOR *POS, XMVECTOR *ROTATION, float *ALPHA)
{
	if (*ALPHA <= 0)
	{
		*ALPHA = 0.0f;
	}
	else
	{
		XMVECTOR rota{ 10.0f,10.0f,10.0f };
		*ROTATION += rota;
		POS->m128_f32[1] -= 0.1f;
		*ALPHA -= 5.0f;
	}
}

const unique_ptr<EnemyData> &IEnemy::GetData()
{
	return iEnemy_EnemyStatusData;
}
