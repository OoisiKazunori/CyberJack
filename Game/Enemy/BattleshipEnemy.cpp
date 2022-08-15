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

	model = std::make_unique<FbxModelRender>();
}

void BattleshipEnemy::Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG)
{
	model->data.transform.pos = POS;	//���W�̏�����
	model->data.transform.scale = { 0.5f,0.5f,0.5f };
	model->data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "BattleShip/" + "BattleshipEnemy_HachOpen_anim.fbx");	//���f���ǂݍ���
	iEnemy_EnemyStatusData->hitBox.radius = 15.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(1);							//�c�胍�b�N�I�������̏�����

	model->data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
	//model->data.removeMaterialFlag = false;
	model->data.colorData.color.x = 255;
	model->data.colorData.color.y = 255;
	model->data.colorData.color.z = 255;
	model->data.colorData.color.a = 255;
	model->data.transform.rotation.x = 0.0f;
	model->data.transform.rotation.y = 0.0f;
	model->data.transform.rotation.z = 0.0f;
	model->data.isPlayFlag = false;
	model->data.isReverseFlag = false;
	initDeadSoundFlag = false;
	demoFlag = DEMO_FLAG;

	model->data.transform.pos.z = -1000.0f;
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
		model->data.transform.pos.z += vel;
		vel += 0.1f;

		if (2.0f <= vel)
		{
			vel = 2.0f;
		}
	}
	//�~�T�C������,�ړI�n�ɂ��ǂ蒅��
	else
	{
		KazMath::Larp(basePos.z, &model->data.transform.pos.z, 0.01f);

		int lBaseTimer = KazMath::ConvertSecondToFlame(8);
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
				misileR[i].data.transform.pos = model->data.transform.pos + KazMath::Vec3<float>(-5.0f, 88.0f, 25.0f) + KazMath::Vec3<float>(0.0f, 0.0f, 10.0f * retuIndex);
			}
			else
			{
				misileR[i].data.transform.pos = model->data.transform.pos + KazMath::Vec3<float>(5.0f, 88.0f, 25.0f) + KazMath::Vec3<float>(0.0f, 0.0f, 10.0f * retuIndex);
			}
			//�~�T�C�����˒n�_�̐ݒ�--------------------------------------

			if (!model->data.isPlayFlag && !model->data.isReverseFlag && lBaseTimer - KazMath::ConvertSecondToFlame(3) <= appearTimer)
			{
				model->data.isPlayFlag = true;
			}
			if (!model->data.isReverseFlag && KazMath::ConvertSecondToFlame(11) <= appearTimer)
			{
				model->data.isReverseFlag = true;
				model->data.isPlayFlag = false;
			}

			//�~�T�C������
			if (appearTimer == lBaseTimer + i)
			{
				iEnemy_EnemyStatusData->genarateData.initPos = misileR[misileShotIndex].data.transform.pos;
				iEnemy_EnemyStatusData->genarateData.enemyType = ENEMY_TYPE_BATTLESHIP_MISILE;
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
	model->Draw();
}
