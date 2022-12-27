#include "SceneManager.h"
#include"../Helper/KazHelper.h"
#include"../Sound/SoundManager.h"
#include"../Sound/SoundManager.h"
#include"../Helper/ResourceFilePass.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../Fps/FPSManager.h"
#include"../Scene/PortalScene.h"
#include"../Scene/EnemyDebugScene.h"
#include"../Scene/DebugStageScene.h"
#include"../Scene/DebugMeshParticle.h"

SceneManager::SceneManager()
{
	scene.emplace_back(std::make_unique<DebugMeshParticleScene>());
	//scene.emplace_back(std::make_unique<TitleScene>());
	//scene.emplace_back(std::make_unique<GameScene>());
	//scene.emplace_back(std::make_unique<DebugStageScene>());


	nowScene = 0;
	nextScene = 0;
	scene[nowScene]->Init();
	itisInArrayFlag = true;
	endGameFlag = false;
}

SceneManager::~SceneManager()
{
	SoundManager::Instance()->Finalize();
}

void SceneManager::Update()
{
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();

	//シーン遷移の開始
	if (nextScene != nowScene)
	{
		change.Start();
	}


	const int RESTART_NUM = -2;

	//ゲーム画面が隠された判定
	if (change.AllHiden())
	{
		scene[nowScene]->Finalize();
		if (nextScene != RESTART_NUM)
		{
			nowScene = nextScene;
		}
		else if (nextScene == RESTART_NUM)
		{
			nextScene = nowScene;
		}

		if (KazHelper::IsitInAnArray(nowScene, scene.size()))
		{
			scene[nowScene]->Init();
			itisInArrayFlag = true;
		}
		else
		{
			itisInArrayFlag = false;
		}
	}

	//更新処理
	if (itisInArrayFlag)
	{
		scene[nowScene]->Input();
		scene[nowScene]->Update();

		int sceneNum = scene[nowScene]->SceneChange();
		if (sceneNum != SCENE_NONE)
		{
			nextScene = sceneNum;
		}

		if (scene[nowScene]->endGameFlag)
		{
			endGameFlag = true;
		}
	}

	change.Update();

	// fpsを制限(今回は60fps)
	FpsManager::RegulateFps(60);
}

void SceneManager::Draw() {

	if (itisInArrayFlag)
	{
		scene[nowScene]->Draw();
	}

	change.Draw();
	//RenderTargetStatus::Instance()->SwapResourceBarrier();
}
