#include"PopEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

PopEnemy::PopEnemy()
{
}

void PopEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ObjModelRender->data.transform.pos = GENERATE_DATA.initPos;	//座標の初期化
	iEnemy_ObjModelRender->data.transform.scale = { 1.0f,1.0f,1.0f };
	iEnemy_ObjModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "PopEnemy/" + "PopEnemy_Model.obj");	//モデル読み込み
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1,"snkr_BR");							//残りロックオン数等の初期化

	iEnemy_ObjModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ObjModelRender->data.removeMaterialFlag = false;
	iEnemy_ObjModelRender->data.colorData.color.x = 255;
	iEnemy_ObjModelRender->data.colorData.color.y = 255;
	iEnemy_ObjModelRender->data.colorData.color.z = 255;
	iEnemy_ObjModelRender->data.colorData.color.a = 255;
	iEnemy_ObjModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ObjModelRender->data.transform.rotation.y = 180.0f;
	iEnemy_ObjModelRender->data.transform.rotation.z = 0.0f;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	basePos = GENERATE_DATA.initPos;
	bottomPos = basePos + KazMath::Vec3<float>(0.0f, -200.0f, 0.0f);

	reverseFlag = false;
	vel.y = 0.0f;
	
	if (demoFlag)
	{
		vel.z = 0.0f;
		vel.x = 0.0f;
	}
	else
	{
		if (GENERATE_DATA.popEnemyData.leftRightFlag)
		{
			vel.x = -0.5f;
		}
		else
		{
			vel.x = 0.5f;
		}
		vel.z = -0.1f;
	}

	firstTimeToJumpFlag = false;
}

void PopEnemy::Finalize()
{
}

void PopEnemy::Update()
{
	if (reverseFlag && vel.y <= 0.0f)
	{
		reverseFlag = false;
	}
	if (iEnemy_ObjModelRender->data.transform.pos.y <= bottomPos.y)
	{
		reverseFlag = true;
		vel.x += 0.1f;
		vel.x *= -1.0f;
		vel.z += -0.1f;

		firstTimeToJumpFlag = true;
	}

	if (firstTimeToJumpFlag)
	{
		vel.y += -0.15f;
	}
	else
	{
		vel.y += -0.1f;
	}

	
	if (reverseFlag != oldReverseFlag)
	{
		vel.y *= -1.0f;
	}
	oldReverseFlag = reverseFlag;


	iEnemy_ObjModelRender->data.transform.pos += vel;

	ProcessingOfDeath(DEATH_ROLL);
}

void PopEnemy::Draw()
{
	if (0 < iEnemy_ObjModelRender->data.colorData.color.a)
	{
		iEnemy_ObjModelRender->Draw();
		LockOnWindow(iEnemy_ObjModelRender->data.transform.pos);
	}
}
