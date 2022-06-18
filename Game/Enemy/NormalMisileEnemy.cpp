#include "NormalMisileEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Math/KazMath.h"

NormalMisileEnemy::NormalMisileEnemy()
{
	shotSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Shot.wav", false);
}

void NormalMisileEnemy::Init(const KazMath::Vec3<float> &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//座標の初期化
	lerpPos = POS;	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 10.0f,10.0f,10.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "MisileEnemy.obj");	//モデル読み込み
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.color.x = 255.0f;
	iEnemy_ModelRender->data.color.y = 255.0f;
	iEnemy_ModelRender->data.color.z = 255.0f;
	iEnemy_ModelRender->data.color.w = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 90.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;
	initShotFlag = false;

	initDeadSoundFlag = false;
}

void NormalMisileEnemy::Finalize()
{
}

void NormalMisileEnemy::Update()
{
	//発射
	if (120 <= shotTimer && !initShotFlag && iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_EnemyStatusData->genarateData.initPos = iEnemy_ModelRender->data.transform.pos;
		iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;

		//ノックバック
		KazMath::Vec3<float> kockBackVel = { 0.0f,0.0f,60.0f };
		lerpPos = iEnemy_ModelRender->data.transform.pos + kockBackVel;
		SoundManager::Instance()->PlaySoundMem(shotSoundHandle, 1);
		initShotFlag = true;
	}

	//死亡演出処理
	//デバックキーor当たり判定内&&死亡時
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.removeMaterialFlag = true;
		iEnemy_ModelRender->data.color.x = 255.0f;
		iEnemy_ModelRender->data.color.y = 255.0f;
		iEnemy_ModelRender->data.color.z = 255.0f;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.w);

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
		if (iEnemy_ModelRender->data.color.w < 255.0f)
		{
			iEnemy_ModelRender->data.color.w += 5.0f;
		}
		else
		{
			iEnemy_ModelRender->data.color.w = 255.0f;
		}


		++shotTimer;
		KazMath::Vec3<float> vel = { 0.0f,0.0f,-1.0f };
		lerpPos += vel;
	}

	KazMath::Larp(lerpPos.z, &iEnemy_ModelRender->data.transform.pos.z, 0.1f);

	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.z))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}

}

void NormalMisileEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
	LockOnWindow(iEnemy_ModelRender->data.transform.pos);
}
