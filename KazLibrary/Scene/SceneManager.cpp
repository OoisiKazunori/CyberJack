#include "SceneManager.h"
#include"../Helper/KazHelper.h"
#include"../Sound/SoundManager.h"
#include"../Sound/SoundManager.h"
#include"../Helper/ResourceFilePass.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../Fps/FPSManager.h"
#include"../Scene/ClassScene.h"
#include"../Scene/PortalScene.h"


SceneManager::SceneManager()
{
	scene.emplace_back(std::make_unique<GameScene>());
	scene.emplace_back(std::make_unique<DebugScene>());	
	//scene.emplace_back(std::make_unique<PortalScene>());
	//scene.emplace_back(std::make_unique<TitleScene>());

	nowScene = 0;
	nextScene = 0;
	scene[nowScene]->Init();
	itisInArrayFlag = true;
}

SceneManager::~SceneManager()
{
	SoundManager::Instance()->Finalize();
}

void SceneManager::Update()
{
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();

	//�V�[���J�ڂ̋N��
	if (nextScene != nowScene)
	{
		change.Start();
	}


	const int RESTART_NUM = -2;

	//��ʂ��B�ꂽ�u��
	if (change.AllHiden())
	{
		scene[nowScene]->Finalize();
		//�ʏ�̃V�[���؂�ւ�
		if (nextScene != RESTART_NUM)
		{
			nowScene = nextScene;
		}
		//���V�[���̃��Z�b�g
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

	if (itisInArrayFlag)
	{

		scene[nowScene]->Input();
		scene[nowScene]->Update();

		int sceneNum = scene[nowScene]->SceneChange();
		if (sceneNum != SCENE_NONE)
		{
			nextScene = sceneNum;
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
	RenderTargetStatus::Instance()->SwapResourceBarrier();
}
