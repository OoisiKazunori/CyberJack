#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

NormalEnemy::NormalEnemy()
{
	MeshParticleLoadData lData;
	lData.bias = 70;
	lData.faceCountNum = 100;
	lData.perTriangleNum = 50;

	iEnemy_EnemyStatusData->meshParticleData.emplace_back(MeshData(0, &iEnemy_FbxModelRender->motherMat));
	iEnemy_EnemyStatusData->meshParticleData[0].meshParticleData =
		MeshParticleLoader::Instance()->Load(
			KazFilePathName::EnemyPath + "Move/" + "MoveEnemy_Model.fbx",
			true, 
			&iEnemy_FbxModelRender->motherMat,
			lData
		);
	iEnemy_EnemyStatusData->meshParticleData[0].meshParticleData.alpha = &alpha;
	iEnemy_EnemyStatusData->meshParticleFlag = true;
}

void NormalEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	EnemyModelType lModelType = ENEMY_MODEL_FBX;
	InitModel(KazMath::Transform3D(GENERATE_DATA.initPos, { 1.0f,1.0f,1.0f }, { 0.0f,180.0f,0.0f }), KazFilePathName::EnemyPath + "Move/" + "MoveEnemy_Model.fbx", 15.0f, lModelType, true);
	iOperationData.Init(1, "gw-1");							//残りロックオン数等の初期化


	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	speed = GENERATE_DATA.speed;
	iEnemy_EnemyStatusData->startFlag = true;
	iEnemy_EnemyStatusData->objFlag = false;
	iEnemy_EnemyStatusData->radius = 10.0f;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	++iEnemy_FbxModelRender->data.transform.rotation.z;
	//移動
	if (!demoFlag && iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_FbxModelRender->data.transform.pos.z += speed;
	}

	//死亡演出中に登場演出は行わない
	if (!ProcessingOfDeathFbx(DEATH_SINK))
	{
		//登場処理
		if (iEnemy_FbxModelRender->data.colorData.color.a < 255)
		{
			iEnemy_FbxModelRender->data.colorData.color.a += 5;
		}
		else
		{
			iEnemy_FbxModelRender->data.colorData.color.a = 255;
		}
	}

	if (iEnemy_FbxModelRender->data.transform.pos.z <= -50.0f)
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		iEnemy_EnemyStatusData->outOfStageFlag = true;
	}

	if (!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_FbxModelRender->data.colorData.color.a = 0;
		iEnemy_EnemyStatusData->hitBox.radius = 0.0f;
		iEnemy_FbxModelRender->CaluMat();
	}

	alpha = static_cast<float>(iEnemy_FbxModelRender->data.colorData.color.a) / 255;
}

void NormalEnemy::Draw()
{
	if (1.0f <= iEnemy_FbxModelRender->data.colorData.color.a)
	{
		if (!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
		{
			iEnemy_FbxModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		}
		iEnemy_FbxModelRender->Draw(!iEnemy_EnemyStatusData->meshParticleFlag);
		LockOnWindow(iEnemy_FbxModelRender->data.transform.pos);
	}
}
