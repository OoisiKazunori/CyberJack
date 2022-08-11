#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

NormalEnemy::NormalEnemy()
{
	fireRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::EffectFirePath + "Fire.png");
	fireRender.data.billBoardFlag = false;

	fireTexPos = { 0.0f,0.0f,45.0f };
	fireRender.data.transform.scale = { 0.5f,1.5f,1.0f };
	fireRender.data.transform.rotation = { 0.0f,90.0f,90.0f };

	bloomTexPos = { 0.0f,0.0f,20.0f };
	bloomRender.data.change3DFlag = true;
	bloomRender.data.billBoardFlag = true;
	bloomRender.data.radius = 10.0f;
}

void NormalEnemy::Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 1.0f,1.0f,1.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "MoveEnemy_Model.obj");	//モデル読み込み
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.color.color.x = 255;
	iEnemy_ModelRender->data.color.color.y = 255;
	iEnemy_ModelRender->data.color.color.z = 255;
	iEnemy_ModelRender->data.color.color.a = 0;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 180.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;

	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	++iEnemy_ModelRender->data.transform.rotation.z;
	//移動
	if (!demoFlag)
	{
		float speed = 0.5f;
		iEnemy_ModelRender->data.transform.pos.z += -speed;
	}

	//死亡演出処理
	//デバックキーor当たり判定内&&死亡時
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z) &&!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.color.color.x = 255;
		iEnemy_ModelRender->data.color.color.y = 255;
		iEnemy_ModelRender->data.color.color.z = 255;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.color.a);

		if (!initDeadSoundFlag)
		{
			DeadSound();
			initDeadSoundFlag = true;
		}
	}
	//死亡演出中に登場演出は行わない
	else
	{
		//登場処理
		if (iEnemy_ModelRender->data.color.color.a < 255)
		{
			iEnemy_ModelRender->data.color.color.a += 5;
		}
		else
		{
			iEnemy_ModelRender->data.color.color.a = 255;
		}
	}


	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.z))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}

	if (3 <= bloomTimer)
	{
		bloomTimer = 0;
		bloomRender.data.radius = 8.0f;
	}
	else
	{
		bloomRender.data.radius = 10.0f;
		++bloomTimer;
	}

	bloomRender.data.transform.pos = {};
	bloomRender.data.transform.pos = iEnemy_ModelRender->data.transform.pos + bloomTexPos;
	fireRender.data.transform.pos = {};
	fireRender.data.transform.pos = iEnemy_ModelRender->data.transform.pos + fireTexPos;
}

void NormalEnemy::Draw()
{
	if (1.0f <= iEnemy_ModelRender->data.color.color.a)
	{
		if (iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
		{
			bloomRender.Draw();
		}
		iEnemy_ModelRender->Draw();
		if (iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
		{
			fireRender.Draw();
		}
		LockOnWindow(iEnemy_ModelRender->data.transform.pos);
	}
}
