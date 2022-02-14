#pragma once
#include"../DirectXCommon/Base.h"
#include"../Scene/SceneBase.h"

class TitleScene :public SceneBase {

public:
	TitleScene();
	~TitleScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	int sceneNum;
};

