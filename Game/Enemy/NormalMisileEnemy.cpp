#include "NormalMisileEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Easing/easing.h"

NormalMisileEnemy::NormalMisileEnemy()
{
	shotSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Shot.wav", false);

	circleFlashR.data.colorData.color = { 0,255,0,255 };
	flashR.data.colorData.color = { 0,255,0,255 };
}

void NormalMisileEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = GENERATE_DATA.initPos;	//座標の初期化
	lerpPos = GENERATE_DATA.initPos;	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 1.0f,1.0f,1.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Summon/" + "SummonEnemy_Model.obj");	//モデル読み込み
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.colorData.color.x = 255;
	iEnemy_ModelRender->data.colorData.color.y = 255;
	iEnemy_ModelRender->data.colorData.color.z = 255;
	iEnemy_ModelRender->data.colorData.color.a = 0;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;
	initShotFlag = false;

	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	shotTimer = 0;

	rocketEffect.Init(&iEnemy_ModelRender->data.transform.pos, KazMath::Vec3<float>(0.0f, 0.0f, 20.0f), KazMath::Vec3<float>(0.0f, 0.0f, 45.0f));
}

void NormalMisileEnemy::Finalize()
{
}

void NormalMisileEnemy::Update()
{
	//発射
	//ゲーム内で攻撃モーションの起動
	bool lGameShotFlag = !demoFlag && 120 <= shotTimer && !initShotFlag && iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag;
	bool lDebugShotFlag = demoFlag && debugShotFlag;

	if (lGameShotFlag || lDebugShotFlag)
	{
		iEnemy_EnemyStatusData->genarateData.enemyGenerateData.initPos = iEnemy_ModelRender->data.transform.pos;
		iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;

		//ノックバック
		KazMath::Vec3<float> kockBackVel = { 0.0f,0.0f,60.0f };
		lerpPos = iEnemy_ModelRender->data.transform.pos + kockBackVel;
		SoundManager::Instance()->PlaySoundMem(shotSoundHandle, 1);
		initShotFlag = true;
		debugShotFlag = false;
	}

	if (initShotFlag)
	{
		iEnemy_ModelRender->data.transform.rotation.z += 5.0f;
	}

	//死亡演出処理
	//デバックキーor当たり判定内&&死亡時
	if (EnableToHit(iEnemy_ModelRender->data.transform.pos.z) && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME_MULTITEX;
		iEnemy_ModelRender->data.colorData.color.x = 255;
		iEnemy_ModelRender->data.colorData.color.y = 255;
		iEnemy_ModelRender->data.colorData.color.z = 255;
		DeadEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.colorData.color.a);

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
		if (iEnemy_ModelRender->data.colorData.color.a < 255)
		{
			iEnemy_ModelRender->data.colorData.color.a += 5;
		}
		else
		{
			iEnemy_ModelRender->data.colorData.color.a = 255;
		}


		++shotTimer;
		KazMath::Vec3<float> vel = { 0.0f,0.0f,-1.0f };
		if (!demoFlag)
		{
			lerpPos += vel;
		}
	}

	KazMath::Larp(lerpPos.z, &iEnemy_ModelRender->data.transform.pos.z, 0.1f);

	if (!EnableToHit(iEnemy_ModelRender->data.transform.pos.z))
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}


	rocketEffect.Update();


	circleFlashR.data.transform.pos = iEnemy_ModelRender->data.transform.pos + KazMath::Vec3<float>(0.0f, 0.0f, -10.0f);
	flashR.data.transform.pos = iEnemy_ModelRender->data.transform.pos;


	ImGui::Begin("Flash");
	ImGui::DragFloat("POS_X", &circleFlashR.data.transform.pos.x);
	ImGui::DragFloat("POS_Y", &circleFlashR.data.transform.pos.y);
	ImGui::DragFloat("POS_Z", &circleFlashR.data.transform.pos.z);
	ImGui::DragFloat("SCALE_X", &circleFlashR.data.transform.scale.x);
	ImGui::DragFloat("SCALE_Y", &circleFlashR.data.transform.scale.y);
	ImGui::DragFloat("SCALE_Z", &circleFlashR.data.transform.scale.z);
	ImGui::Checkbox("S", &startFlag);
	ImGui::End();

	ImGui::Begin("R");
	ImGui::DragFloat("POS_X", &flashR.data.transform.pos.x);
	ImGui::DragFloat("POS_Y", &flashR.data.transform.pos.y);
	ImGui::DragFloat("POS_Z", &flashR.data.transform.pos.z);
	ImGui::DragFloat("SCALE_X", &flashR.data.transform.scale.x);
	ImGui::DragFloat("SCALE_Y", &flashR.data.transform.scale.y);
	ImGui::DragFloat("SCALE_Z", &flashR.data.transform.scale.z);
	ImGui::End();


	if (!startFlag)
	{
		disappearTimer = 0;
		flashTimer = 0;
		circleFlashTimer = 0;
	}

	int lT = 0;
	bool flashFlag = false;
	if (15 <= flashTimer)
	{
		++disappearTimer;
		lT = disappearTimer;
		flashTimer = 15;
		flashFlag = true;
	}
	else
	{
		++flashTimer;
		lT = flashTimer;
	}
	++circleFlashTimer;

	if (10 <= disappearTimer)
	{
		disappearTimer = 10;
	}

	if (!flashFlag)
	{
		flashR.data.transform.scale.x = EasingMaker(Out, Cubic, KazMath::ConvertTimerToRate(lT, 15)) * 3.0f;
	}
	else
	{
		flashR.data.transform.scale.x = 3.0f + -EasingMaker(In, Cubic, KazMath::ConvertTimerToRate(lT, 10)) * 3.0f;
	}


	float scale = 0.0f;
	if (flashTimer <= 1)
	{
		circleFlashR.data.transform.scale.x = 0.1f;
		circleFlashR.data.transform.scale.y = 0.1f;
		circleFlashR.data.transform.scale.z = 0.1f;
	}
	else if (flashTimer <= 2)
	{
		circleFlashR.data.transform.scale.x = 1.5f;
		circleFlashR.data.transform.scale.y = 1.0f;
		circleFlashR.data.transform.scale.z = 1.0f;
		scale = 0.15f;
	}
	else
	{
		circleFlashR.data.transform.scale.x = 1.0f + -EasingMaker(Out, Cubic, KazMath::ConvertTimerToRate(circleFlashTimer, 40)) * 1.0f;
		circleFlashR.data.transform.scale.y = 1.0f + -EasingMaker(Out, Cubic, KazMath::ConvertTimerToRate(circleFlashTimer, 40)) * 1.0f;
		circleFlashR.data.transform.scale.z = 1.0f + -EasingMaker(Out, Cubic, KazMath::ConvertTimerToRate(circleFlashTimer, 40)) * 1.0f;
		scale = 0.15f;
	}

	flashR.data.transform.scale.y = scale + -EasingMaker(Out, Cubic, KazMath::ConvertTimerToRate(disappearTimer, 10)) * scale;



	flashR.data.radius = 10.0f;
	circleFlashR.data.radius = 10.0f;
	flashR.data.change3DFlag = true;
	circleFlashR.data.change3DFlag = true;
}

void NormalMisileEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
	rocketEffect.Draw();
	LockOnWindow(iEnemy_ModelRender->data.transform.pos);

	flashR.Draw();
	circleFlashR.Draw();
}
