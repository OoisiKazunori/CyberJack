#include"TutorialBlock.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

TutorialBlock::TutorialBlock()
{
}

void TutorialBlock::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ObjModelRender->data.transform.pos = GENERATE_DATA.initPos;	//座標の初期化
	iEnemy_ObjModelRender->data.transform.scale = { 1.0f,1.0f,1.0f };
	iEnemy_ObjModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Move/" + "MoveEnemy_Model.obj");	//モデル読み込み
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1, "KeyBlock");							//残りロックオン数等の初期化

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
	iEnemy_EnemyStatusData->radius = 8.0f;
	iEnemy_EnemyStatusData->startFlag = true;

	box.data.transform.pos = GENERATE_DATA.initPos;
	larpScale = { 1.5f,1.5f,1.5f };
	box.data.color.color = { 255,255,255,255 };

	iEnemy_EnemyStatusData->hitBox.center = &box.data.transform.pos;
}

void TutorialBlock::Finalize()
{
}

void TutorialBlock::Update()
{
	++timer;

	KazMath::Larp(larpScale, &box.data.transform.scale, 0.1f);
	if (60 <= timer && iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		box.data.transform.scale = { 1.0f,1.0f,1.0f };
		timer = 0;
	}
	else if (30 <= timer && !iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		box.data.color.color = { 255,0,0,255 };
		box.data.transform.scale = { 1.0f,1.0f,1.0f };
		timer = 0;
	}
}

void TutorialBlock::Draw()
{
	if (1.0f <= iEnemy_ObjModelRender->data.colorData.color.a)
	{
		//iEnemy_ModelRender->Draw();
		box.Draw();
		LockOnWindow(iEnemy_ObjModelRender->data.transform.pos);
	}
}