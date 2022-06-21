#include "PortalScene.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Input/ControllerInputManager.h"

PortalScene::PortalScene()
{
}

PortalScene::~PortalScene()
{
}

void PortalScene::Init()
{
	initPos = { 0.0f,0.0f,30.0f };

	portal.Init(initPos);
}

void PortalScene::Finalize()
{
}

void PortalScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();
#pragma region カメラ操作
	debugCameraMove = { 0.0f,0.0f,0.0f };
	float debugSpeed = 1.0f;

	//�J�����ړ�
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
		//y�Ȃ̂ɑO�ɐi��....
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}


	//�J�����p�x
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

	if (input->InputTrigger(DIK_SPACE))
	{
		portal.Start();
	}
	if (input->InputTrigger(DIK_R))
	{
		portal.Init(initPos);
	}

#pragma endregion
}

void PortalScene::Update()
{
	//デバック用
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);




	portal.Update();
}

void PortalScene::Draw()
{
	bg.Draw();
	portal.Draw();
}

int PortalScene::SceneChange()
{
	return 0;
}
