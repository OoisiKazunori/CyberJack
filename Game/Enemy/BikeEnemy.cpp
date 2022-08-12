#include "BikeEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

const float BikeEnemy::DISAPPEAR_Z_POS = 1000.0f;

BikeEnemy::BikeEnemy()
{
}

void BikeEnemy::Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	float lScale = 0.7f;
	iEnemy_ModelRender->data.transform.scale = { lScale,lScale,lScale };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Bike_Model.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	basePos = POS;

	iEnemy_ModelRender->data.transform.pos.y += 7.0f;
	iEnemy_ModelRender->data.transform.pos.z = -1000.0f;
	line.data.startPos = POS;
	line.data.endPos = POS;
	line.data.startPos.z = -1000.0f;
	line.data.endPos.z = line.data.startPos.z;

	appearTimer = 0;
}

void BikeEnemy::Finalize()
{
}

void BikeEnemy::Update()
{
	++appearTimer;
	float lPos = 0.0f;
	float lMul = 0.019f;
	/*
	�o��
	���̓o���A�G��o�ꂳ����
	*/
	if (appearTimer <= 200)
	{
		lPos = basePos.z;
		kockBackPos.z = iEnemy_ModelRender->data.transform.pos.z - 20.0f;
	}
	/*
	����
	*/
	else if (200 <= appearTimer && appearTimer <= 320)
	{
		//�~�T�C���̔��ˌ���ݒ�
		misileR.data.transform.pos.x = iEnemy_ModelRender->data.transform.pos.x + 5.0f;
		misileR.data.transform.pos.y = iEnemy_ModelRender->data.transform.pos.y + 5.0f;
		misileR.data.transform.pos.z = iEnemy_ModelRender->data.transform.pos.z + 14.0f;
		misileR2.data.transform.pos = misileR.data.transform.pos;
		misileR2.data.transform.pos.x -= 10.0f;

		//���E����~�T�C���𔭎˂���--------------------------------------
		if (appearTimer == 201)
		{
			iEnemy_EnemyStatusData->genarateData.initPos = misileR.data.transform.pos;
			iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;
		}
		if (appearTimer == 202)
		{
			iEnemy_EnemyStatusData->genarateData.initPos = misileR2.data.transform.pos;
			iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;
		}
		//���E����~�T�C���𔭎˂���--------------------------------------


		lPos = basePos.z;
		kockBackPos.z = iEnemy_ModelRender->data.transform.pos.z - 20.0f;
	}
	/*
	�ޏo����
	�G�����ɏ��������m�b�N�o�b�N����
	*/
	else if (320 <= appearTimer && appearTimer <= 380)
	{
		lPos = kockBackPos.z;
		lMul = 0.1f;
	}
	/*
	�ޏo
	���ƓG��ޏo������
	*/
	else
	{
		lPos = DISAPPEAR_Z_POS;
		KazMath::Larp(DISAPPEAR_Z_POS, &line.data.startPos.z, 0.02f);
	}

	KazMath::Larp(lPos, &iEnemy_ModelRender->data.transform.pos.z, lMul);
	//����o�ꂳ����
	KazMath::Larp(DISAPPEAR_Z_POS, &line.data.endPos.z, 0.02f);

	ImGui::Begin("Misile");
	ImGui::DragFloat("POS_X", &misileR.data.transform.pos.x);
	ImGui::DragFloat("POS_Y", &misileR.data.transform.pos.y);
	ImGui::DragFloat("POS_Z", &misileR.data.transform.pos.z);
	ImGui::DragFloat("POS_X2", &misileR2.data.transform.pos.x);
	ImGui::DragFloat("POS_Y2", &misileR2.data.transform.pos.y);
	ImGui::DragFloat("POS_Z2", &misileR2.data.transform.pos.z);
	ImGui::End();
	misileR.data.color.color = { 255,0,0,255 };
	misileR2.data.color.color = { 255,0,0,255 };

	misileR.data.transform.pos.x = iEnemy_ModelRender->data.transform.pos.x + 5.0f;
	misileR.data.transform.pos.y = iEnemy_ModelRender->data.transform.pos.y + 5.0f;
	misileR.data.transform.pos.z = iEnemy_ModelRender->data.transform.pos.z + 14.0f;
	misileR2.data.transform.pos = misileR.data.transform.pos;
	misileR2.data.transform.pos.x -= 10.0f;
}

void BikeEnemy::Draw()
{
	line.Draw();

	misileR.Draw();
	misileR2.Draw();
	iEnemy_ModelRender->Draw();
}
