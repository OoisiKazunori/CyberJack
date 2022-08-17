#include "EnemyDebugScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include"../Camera/CameraMgr.h"
#include"../KazLibrary/Loader/FbxModelResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../Game/Enemy/BattleshipEnemy.h"
#include"../Game/Enemy/PopEnemy.h"
#include"../Game/Enemy/BikeEnemy.h"
#include"../Game/Enemy/SplineMisileForBikeEnemy.h"
#include"../Game/Enemy/SplineMisileForBattleShip.h"

EnemyDebugScene::EnemyDebugScene()
{
	int lIndex = 0;
	enemies[lIndex] = std::make_unique<BattleshipEnemy>();
	misiles[lIndex][0] = std::make_unique<SplineMisileForBattleShip>();
	misiles[lIndex][1] = std::make_unique<SplineMisileForBattleShip>();
	misiles[lIndex][2] = std::make_unique<SplineMisileForBattleShip>();
	misiles[lIndex][3] = std::make_unique<SplineMisileForBattleShip>();
	misiles[lIndex][4] = std::make_unique<SplineMisileForBattleShip>();
	misiles[lIndex][5] = std::make_unique<SplineMisileForBattleShip>();
	misiles[lIndex][6] = std::make_unique<SplineMisileForBattleShip>();
	misiles[lIndex][7] = std::make_unique<SplineMisileForBattleShip>();
	++lIndex;
	enemies[lIndex] = std::make_unique<NormalEnemy>();
	++lIndex;
	enemies[lIndex] = std::make_unique<NormalMisileEnemy>();
	misiles[lIndex][0] = std::make_unique<SplineMisile>();
	++lIndex;
	enemies[lIndex] = std::make_unique<PopEnemy>();
	++lIndex;
	enemies[lIndex] = std::make_unique<BikeEnemy>();
	misiles[lIndex][0] = std::make_unique<SplineMisileForBikeEnemy>();
	misiles[lIndex][1] = std::make_unique<SplineMisileForBikeEnemy>();
}

EnemyDebugScene::~EnemyDebugScene()
{
}

void EnemyDebugScene::Init()
{
	gameFlame = 2;
	oldEnemyType = -1;
	for (int i = 0; i < kidEnemyHandles.size(); ++i)
	{
		kidEnemyHandles[i] = 0;
	}

	player.data.color = { 0,255,0,255 };

	responePos = { -20.0f,0.0f,100.0f };
}

void EnemyDebugScene::Finalize()
{
}

void EnemyDebugScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();

#pragma region camera
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 1;
	if (input->InputState(DIK_D))
	{
		debugCameraMove.x = -debugSpeed;
	}
	if (input->InputState(DIK_A))
	{
		debugCameraMove.x = debugSpeed;
	}
	if (input->InputState(DIK_W))
	{
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}

	if (input->InputState(DIK_RIGHTARROW))
	{
		angle.x += debugSpeed;
	}
	if (input->InputState(DIK_LEFTARROW))
	{
		angle.x += -debugSpeed;
	}

	if (input->InputState(DIK_UPARROW))
	{
		angle.y += debugSpeed;
	}
	if (input->InputState(DIK_DOWNARROW))
	{
		angle.y += -debugSpeed;
	}

#pragma endregion

}

void EnemyDebugScene::Update()
{

	gameFlame = 1 + specifiedEnemyType * 10;


	//�f�o�b�N�p
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);

	ImGui::Begin("EnemyList");
	ImGui::InputInt("enemyIndex", &specifiedEnemyType);
	initFlag = ImGui::Button("Init");
	ImGui::SameLine();
	deadFlag = ImGui::Button("Dead");
	ImGui::SameLine();
	attackFlag = ImGui::Button("Attack");
	ImGui::DragFloat("ResponePosX", &responePos.x);
	ImGui::DragFloat("ResponePosY", &responePos.y);
	ImGui::DragFloat("ResponePosZ", &responePos.z);
	ImGui::End();

	//�T�C�Y���--------------------------------------
	if (specifiedEnemyType <= -1)
	{
		specifiedEnemyType = 0;
	}
	else if (enemies.size() <= specifiedEnemyType)
	{
		specifiedEnemyType = static_cast<int>(enemies.size() - 1);
	}
	//�T�C�Y���--------------------------------------


	//������--------------------------------------
	if (specifiedEnemyType != oldEnemyType || initFlag)
	{
		if (oldEnemyType <= -1)
		{
			oldEnemyType = 0;
		}

		//�G�Ǝq�G�̎��S--------------------------------------
		if (oldEnemyType != -1)
		{
			enemies[oldEnemyType]->Dead();

			for (int i = 0; i < misiles[oldEnemyType].size(); ++i)
			{
				if (misiles[oldEnemyType][i] != nullptr)
				{
					misiles[oldEnemyType][i]->Dead();
				}
			}
		}
		kidEnemyHandles[oldEnemyType] = 0;
		//�G�Ǝq�G�̎��S--------------------------------------

		//������--------------------------------------
		EnemyGenerateData lData;
		lData.initPos = responePos;
		lData.battleShipData.isShotFlag = true;
		enemies[specifiedEnemyType]->Init(lData, true);
		//������--------------------------------------

		//�ȈՃc�[���̏�����--------------------------------------
		gameFlame = 0;
		initFlag = false;
		oldEnemyType = specifiedEnemyType;
		//�ȈՃc�[���̏�����--------------------------------------
	}
	//������--------------------------------------

	//���S--------------------------------------
	if (deadFlag)
	{
		enemies[specifiedEnemyType]->Dead();
		deadFlag = false;
	}
	//���S--------------------------------------


	//�U��--------------------------------------
	if (attackFlag)
	{
		enemies[specifiedEnemyType]->DebugShot();
		kidEnemyHandles[specifiedEnemyType] = 0;
		attackFlag = false;
	}
	//�U��--------------------------------------

	EnemyData *lEnemyData = enemies[specifiedEnemyType]->GetData().get();
	if (enemies[specifiedEnemyType]->GetData()->genarateData.enemyType != -1)
	{
		//��������G�̎��
		misiles[specifiedEnemyType][kidEnemyHandles[specifiedEnemyType]]->Init(lEnemyData->genarateData.enemyGenerateData, true);

		++kidEnemyHandles[specifiedEnemyType];
		//�ǉ�������I������������
		lEnemyData->genarateData.Finalize();
	}

	for (int i = 0; i < misiles[specifiedEnemyType].size(); ++i)
	{
		if (misiles[specifiedEnemyType][i] != nullptr && misiles[specifiedEnemyType][i]->GetData()->timer <= 0)
		{
			misiles[specifiedEnemyType][i]->Hit();
		}
	}


	enemies[specifiedEnemyType]->Update();

	hitBox.data.transform.pos = *enemies[specifiedEnemyType]->GetData()->hitBox.center;
	float lScale = enemies[specifiedEnemyType]->GetData()->hitBox.radius;
	hitBox.data.transform.scale = { lScale ,lScale,lScale };
	hitBox.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;

	for (int i = 0; i < misiles[specifiedEnemyType].size(); ++i)
	{
		if (misiles[specifiedEnemyType][i] != nullptr)
		{
			misiles[specifiedEnemyType][i]->Update();

		
		}
	}

}

void EnemyDebugScene::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);



	bg.Draw();

	player.Draw();

	for (int i = 0; i < misiles[specifiedEnemyType].size(); ++i)
	{
		if (misiles[specifiedEnemyType][i] != nullptr)
		{
			misiles[specifiedEnemyType][i]->Draw();
		}
	}
	enemies[specifiedEnemyType]->Draw();

	hitBox.Draw();

	//kidFbxModel.Draw();

}

int EnemyDebugScene::SceneChange()
{
	if (ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_START))
	{
		return -2;
	}
	else if (ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_BACK))
	{
		return 1;
	}
	else
	{
		return SCENE_NONE;
	}
}
