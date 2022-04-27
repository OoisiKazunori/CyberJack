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
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化

	iEnemy_ModelRender->data.color.w = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.m128_f32[1] = 90.0f;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	if (iEnemy_EnemyStatusData->oprationObjData->rockOnNum <= 0)
	{
		iEnemy_ModelRender->data.color = { 255.0f,0.0f,0.0f,255.0f };
	}

	//移動
	float speed = 0.8f;
	iEnemy_ModelRender->data.transform.pos.m128_f32[2] += -speed;

	//死亡演出処理
	if (!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag || KeyBoradInputManager::Instance()->InputState(DIK_J))
	{
		//iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		DradEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.w);
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
}

void NormalEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
