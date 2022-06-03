#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/Sprite2DRender.h"
#include"../Event/WindowFlame.h"

class TutorialWindow
{
public:
	TutorialWindow();
	void Init();
	void Update();
	void Draw();

private:
	WindowFlame windowFlame;
};
