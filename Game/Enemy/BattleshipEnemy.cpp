#include "BattleshipEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

BattleshipEnemy::BattleshipEnemy()
{
}

void BattleshipEnemy::Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 1.0f,1.0f,1.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "BattleshipEnemy_Model.obj");	//モデル読み込み
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

void BattleshipEnemy::Finalize()
{
}

void BattleshipEnemy::Update()
{
}

void BattleshipEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
