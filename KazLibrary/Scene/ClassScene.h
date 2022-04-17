#pragma once
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"

class ClassScene:public SceneBase
{
public:
	ClassScene();
	~ClassScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	BackGroundForDebug bg;
};
