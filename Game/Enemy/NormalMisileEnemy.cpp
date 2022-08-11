#include "NormalMisileEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Math/KazMath.h"

NormalMisileEnemy::NormalMisileEnemy()
{
	shotSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Shot.wav", false);
}

void NormalMisileEnemy::Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG)
{
	model.data.transform.pos = POS;	//座標の初期化
	lerpPos = POS;	//座標の初期化
	model.data.transform.scale = { 1.0f,1.0f,1.0f };
	model.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Gunner_Model.fbx");	//モデル読み込み
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化

	model.data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
	model.data.color.color.x = 255;
	model.data.color.color.y = 255;
	model.data.color.color.z = 255;
	model.data.color.color.a = 0;
	model.data.transform.rotation.x = 180.0f;
	model.data.transform.rotation.y = 0.0f;
	model.data.transform.rotation.z = 0.0f;
	initShotFlag = false;

	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	shotTimer = 0;

	rocketEffect.Init(&model.data.transform.pos);
}

void NormalMisileEnemy::Finalize()
{
}

void NormalMisileEnemy::Update()
{

	++model.data.transform.rotation.z;
	//発射
	//ゲーム内で攻撃モーションの起動
	bool lGameShotFlag = !demoFlag && 120 <= shotTimer && !initShotFlag && iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag;
	bool lDebugShotFlag = demoFlag && debugShotFlag;

	if (lGameShotFlag || lDebugShotFlag)
	{
		iEnemy_EnemyStatusData->genarateData.initPos = model.data.transform.pos;
		iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;

		//ノックバック
		KazMath::Vec3<float> kockBackVel = { 0.0f,0.0f,60.0f };
		lerpPos = model.data.transform.pos + kockBackVel;
		SoundManager::Instance()->PlaySoundMem(shotSoundHandle, 1);
		initShotFlag = true;
		debugShotFlag = false;
	}

	//死亡演出処理
	//デバックキーor当たり判定内&&死亡時
	if (EnableToHit(model.data.transform.pos.z) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		//model.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		//model.data.removeMaterialFlag = true;
		model.data.color.color.x = 255;
		model.data.color.color.y = 255;
		model.data.color.color.z = 255;
		DeadEffect(&model.data.transform.pos, &model.data.transform.rotation, &model.data.color.color.a);

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
		if (model.data.color.color.a < 255)
		{
			model.data.color.color.a += 5;
		}
		else
		{
			model.data.color.color.a = 255;
		}


		++shotTimer;
		KazMath::Vec3<float> vel = { 0.0f,0.0f,-1.0f };
		if (!demoFlag)
		{
			lerpPos += vel;
		}
	}

	KazMath::Larp(lerpPos.z, &model.data.transform.pos.z, 0.1f);

	if (!EnableToHit(model.data.transform.pos.z))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}


	rocketEffect.Update();

}

void NormalMisileEnemy::Draw()
{
	model.data.isPlay = true;
	model.Draw();
	rocketEffect.Draw();
	LockOnWindow(model.data.transform.pos);
}
