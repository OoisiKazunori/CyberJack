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
}

void PopEnemy::Finalize()
{
}

void PopEnemy::Update()
{
}

void PopEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
