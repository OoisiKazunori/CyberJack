#include "TutorialWindow.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"

TutorialWindow::TutorialWindow()
{
}

void TutorialWindow::Init()
{
	KazMath::Transform3D transform;
	transform.pos = { 0.0f,100.0f,100.0f };
	transform.scale = { 0.5f,0.5f,0.5f };
	transform.rotation = { 0.0f,0.0f,0.0f };

	windowFlame.Init(transform, { 1.0f,1.0f }, false);
}

void TutorialWindow::Update()
{
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_H))
	{
		KazMath::Transform3D transform;
		transform.pos = { 15.0f,0.0f,15.0f };
		transform.scale = { 0.0f,0.0f,0.0f };
		transform.rotation = { 0.0f,20.0f,0.0f };
		windowFlame.Init(transform, { 0.3f,0.35f }, false);
		windowFlame.Start();
	}
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_J))
	{
		windowFlame.End();
	}
	
	windowFlame.Update();
}

void TutorialWindow::Draw()
{
	windowFlame.Draw();
}
