#include"../Scene/TitleScene.h"
#include"../DirectXCommon/Base.h"
#include"../Input/KeyBoradInputManager.h"


TitleScene::TitleScene()
{

}

TitleScene::~TitleScene() {

}

void TitleScene::Init()
{
	sceneNum = -1;
}

void TitleScene::Finalize()
{

}

void TitleScene::Input()
{
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_O))
	{
		sceneNum = 1;
	}
}

void TitleScene::Update()
{

}

void TitleScene::Draw()
{

}

int TitleScene::SceneChange()
{
	if (sceneNum != -1)
	{
		int tmp = sceneNum;
		sceneNum = -1;
		return tmp;
	}

	return SCENE_NONE;
}