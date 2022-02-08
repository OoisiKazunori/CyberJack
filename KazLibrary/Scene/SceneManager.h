#pragma once
#include"../DirectXCommon/Base.h"
#include"../Scene/SceneBase.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"DebugScene.h"
#include"../Scene/SceneChange.h"
#include"../RenderTarget/GaussianBuler.h"
#include"../RenderTarget/RenderTargetStatus.h"
/// <summary>
/// ?V?[???????????s?????
/// </summary>
class SceneManager {
public:
	SceneManager();
	~SceneManager();
	void Update();
	void Draw();

private:
	std::vector<SceneBase *> scene;
	ChangeScene::SceneChange change;
	int nowScene, nextScene;

	bool sceneChangeFlag;

	bool itisInArrayFlag;
};

