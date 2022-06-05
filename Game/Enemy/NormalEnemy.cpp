#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 10.0f,10.0f,10.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");	//モデル読み込み
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.color.x = 255.0f;
	iEnemy_ModelRender->data.color.y = 255.0f;
	iEnemy_ModelRender->data.color.z = 255.0f;
	iEnemy_ModelRender->data.color.w = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.m128_f32[0] = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.m128_f32[1] = 90.0f;
	iEnemy_ModelRender->data.transform.rotation.m128_f32[2] = 0.0f;

}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	//移動
	float speed = 0.5f;
	iEnemy_ModelRender->data.transform.pos.m128_f32[2] += -speed;

	//死亡演出処理
	//デバックキーor当たり判定内&&死亡時
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.m128_f32[2]) &&!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.color.x = 255.0f;
		iEnemy_ModelRender->data.color.y = 255.0f;
		iEnemy_ModelRender->data.color.z = 255.0f;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.w);
	}
	//死亡演出中に登場演出は行わない
	else
	{
		//登場処理
		if (iEnemy_ModelRender->data.color.w < 255.0f)
		{
			iEnemy_ModelRender->data.color.w += 5.0f;
		}
		else
		{
			iEnemy_ModelRender->data.color.w = 255.0f;
		}
	}


	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.m128_f32[2]))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
	}
}

void NormalEnemy::Draw()
{
	if (1.0f <= iEnemy_ModelRender->data.color.w)
	{
		iEnemy_ModelRender->Draw();
		LockOnWindow();
	}
}
