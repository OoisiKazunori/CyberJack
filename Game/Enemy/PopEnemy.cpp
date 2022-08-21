#include"PopEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

PopEnemy::PopEnemy()
{
}

void PopEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = GENERATE_DATA.initPos;	//���W�̏�����
	iEnemy_ModelRender->data.transform.scale = { 1.0f,1.0f,1.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "PopEnemy/" + "PopEnemy_Model.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.colorData.color.x = 255;
	iEnemy_ModelRender->data.colorData.color.y = 255;
	iEnemy_ModelRender->data.colorData.color.z = 255;
	iEnemy_ModelRender->data.colorData.color.a = 255;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 180.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	basePos = GENERATE_DATA.initPos;
	bottomPos = basePos + KazMath::Vec3<float>(0.0f, -100.0f, 0.0f);

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
			vel.x = -0.2f;
		}
		else
		{
			vel.x = 0.2f;
		}
		vel.z = -0.1f;
	}
	
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
	if (iEnemy_ModelRender->data.transform.pos.y <= bottomPos.y)
	{
		reverseFlag = true;
	}


	if (reverseFlag)
	{
		vel.y += -0.1f;
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


	iEnemy_ModelRender->data.transform.pos += vel;
}

void PopEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
