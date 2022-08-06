#include "EnemyDebugScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include"../Camera/CameraMgr.h"
#include"../KazLibrary/Loader/FbxModelResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

EnemyDebugScene::EnemyDebugScene()
{
	int lIndex = 0;
	enemies[lIndex] = std::make_unique<NormalEnemy>();
	++lIndex;
	enemies[lIndex] = std::make_unique<NormalMisileEnemy>();
	misiles[lIndex][0] = std::make_unique<SplineMisile>();
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


	//ÉfÉoÉbÉNóp
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);

	ImGui::Begin("EnemyList");
	ImGui::InputInt("enemyIndex", &specifiedEnemyType);
	initFlag = ImGui::Button("Init");
	deadFlag = ImGui::Button("Dead");
	attackFlag = ImGui::Button("Attack");
	ImGui::End();

	//ÉTÉCÉYè„å¿--------------------------------------
	if (specifiedEnemyType <= -1)
	{
		specifiedEnemyType = 0;
	}
	else if (enemies.size() <= specifiedEnemyType)
	{
		specifiedEnemyType = static_cast<int>(enemies.size() - 1);
	}
	//ÉTÉCÉYè„å¿--------------------------------------


	//èâä˙âª--------------------------------------
	if (specifiedEnemyType != oldEnemyType || initFlag)
	{
		if (oldEnemyType <= -1)
		{
			oldEnemyType = 0;
		}

		//ìGÇ∆éqìGÇÃéÄñS--------------------------------------
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
		//ìGÇ∆éqìGÇÃéÄñS--------------------------------------

		//èâä˙âª--------------------------------------
		enemies[specifiedEnemyType]->Init(KazMath::Vec3<float>(20.0f, 0.0f, 200.0f), true);
		//èâä˙âª--------------------------------------

		//ä»à’ÉcÅ[ÉãÇÃèâä˙âª--------------------------------------
		gameFlame = 0;
		initFlag = false;
		oldEnemyType = specifiedEnemyType;
		//ä»à’ÉcÅ[ÉãÇÃèâä˙âª--------------------------------------
	}
	//èâä˙âª--------------------------------------


	//éÄñS--------------------------------------
	if (deadFlag)
	{
		enemies[specifiedEnemyType]->Dead();
		deadFlag = false;
	}
	//éÄñS--------------------------------------


	//çUåÇ--------------------------------------
	if (attackFlag)
	{
		enemies[specifiedEnemyType]->DebugShot();
		kidEnemyHandles[specifiedEnemyType] = 0;
		attackFlag = false;
	}
	//çUåÇ--------------------------------------

	EnemyData *lEnemyData = enemies[specifiedEnemyType]->GetData().get();
	if (enemies[specifiedEnemyType]->GetData()->genarateData.enemyType != -1)
	{
		//ê∂ê¨Ç∑ÇÈìGÇÃéÌóﬁ
		misiles[specifiedEnemyType][kidEnemyHandles[specifiedEnemyType]]->Init(lEnemyData->genarateData.initPos, true);

		++kidEnemyHandles[specifiedEnemyType];
		//í«â¡ÇµÇΩÇÁèIóπèàóùÇì¸ÇÍÇÈ
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

	player.Draw();

	enemies[specifiedEnemyType]->Draw();

	for (int i = 0; i < misiles[specifiedEnemyType].size(); ++i)
	{
		if (misiles[specifiedEnemyType][i] != nullptr)
		{
			misiles[specifiedEnemyType][i]->Draw();
		}
	}

	bg.Draw();
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
