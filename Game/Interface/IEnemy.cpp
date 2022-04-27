#include "IEnemy.h"

IEnemy::IEnemy()
{
	//•`‰æ‚Ì‰Šú‰»----------------------------------------------------------------
	iEnemy_ModelRender = std::make_unique<ObjModelRender>();
	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
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

void IEnemy::DradEffect(XMVECTOR *POS, XMVECTOR *ROTATION, float *ALPHA)
{
	if (*ALPHA <= 0)
	{
		*ALPHA = 0.0f;
	}
	else
	{
		XMVECTOR rota{ 5.0f,0.0f,5.0f };
		*ROTATION += rota;
		POS->m128_f32[1] -= 0.5f;
		*ALPHA -= 5.0f;
	}
}

const unique_ptr<EnemyData> &IEnemy::GetData()
{
	return iEnemy_EnemyStatusData;
}
