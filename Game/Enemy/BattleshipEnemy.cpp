#include "BattleshipEnemy.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

BattleshipEnemy::BattleshipEnemy()
{
	for (int i = 0; i < misileR.size(); ++i)
	{
		misileR[i].data.transform.pos.x = 5.0f;
		misileR[i].data.transform.pos.y = 88.0f;
		misileR[i].data.transform.pos.z = 25.0f;
	}
}

void BattleshipEnemy::Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	iEnemy_ModelRender->data.transform.scale = { 0.5f,0.5f,0.5f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "BattleshipEnemy_Model.obj");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ModelRender->data.removeMaterialFlag = false;
	iEnemy_ModelRender->data.color.color.x = 255;
	iEnemy_ModelRender->data.color.color.y = 255;
	iEnemy_ModelRender->data.color.color.z = 255;
	iEnemy_ModelRender->data.color.color.a = 0;
	iEnemy_ModelRender->data.transform.rotation.x = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.y = 0.0f;
	iEnemy_ModelRender->data.transform.rotation.z = 0.0f;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	iEnemy_ModelRender->data.transform.pos.z = -1000.0f;
	basePos = POS;
	vel = 0.0f;
	appearTimer = 0;
	misileShotIndex = 0;
}

void BattleshipEnemy::Finalize()
{
}

void BattleshipEnemy::Update()
{
	//��莞�Ԏ~�܂����炻�̂܂܉�ʊO�Ɍ�����
	if (KazMath::ConvertSecondToFlame(10) <= appearTimer && false)
	{
		iEnemy_ModelRender->data.transform.pos.z += vel;
		vel += 0.1f;

		if (2.0f <= vel)
		{
			vel = 2.0f;
		}
	}
	//�~�T�C������,�ړI�n�ɂ��ǂ蒅��
	else
	{
		KazMath::Larp(basePos.z, &iEnemy_ModelRender->data.transform.pos.z, 0.01f);

		int lBaseTimer = KazMath::ConvertSecondToFlame(7);
		int retuIndex = 0;
		int countIndex = 0;
		for (int i = 0; i < misileR.size(); ++i)
		{
			//�~�T�C�����˒n�_�̐ݒ�--------------------------------------
			bool minusFlag = i % 2 != 0;
			if (2 <= countIndex)
			{
				countIndex = 1;
				++retuIndex;
			}
			else
			{
				++countIndex;
			}

			if (minusFlag)
			{
				misileR[i].data.transform.pos = iEnemy_ModelRender->data.transform.pos + KazMath::Vec3<float>(-5.0f, 88.0f, 25.0f) + KazMath::Vec3<float>(0.0f, 0.0f, 10.0f * retuIndex);
			}
			else
			{
				misileR[i].data.transform.pos = iEnemy_ModelRender->data.transform.pos + KazMath::Vec3<float>(5.0f, 88.0f, 25.0f) + KazMath::Vec3<float>(0.0f, 0.0f, 10.0f * retuIndex);
			}
			//�~�T�C�����˒n�_�̐ݒ�--------------------------------------


			//�~�T�C������
			if (appearTimer == lBaseTimer + i)
			{
				iEnemy_EnemyStatusData->genarateData.initPos = misileR[misileShotIndex].data.transform.pos;
				iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_MISILE_SPLINE;
				++misileShotIndex;
				break;
			}
		}
	}

	ImGui::Begin("Misile");
	for (int i = 0; i < misileR.size(); ++i)
	{
		misileR[i].data.color.color = { 255,0,0,255 };
		std::string name = "POS";
		std::string tag = name + "X" + std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &misileR[i].data.transform.pos.x);
		tag = name + "Y" + std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &misileR[i].data.transform.pos.y);
		tag = name + "Z" + std::to_string(i);
		ImGui::DragFloat(tag.c_str(), &misileR[i].data.transform.pos.z);
	}
	ImGui::End();


	++appearTimer;
}

void BattleshipEnemy::Draw()
{
	for (int i = 0; i < misileR.size(); ++i)
	{
		misileR[i].Draw();
	}
	iEnemy_ModelRender->Draw();
}
