#include "PortalScene.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Input/ControllerInputManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/KazImGuiHelper.h"

PortalScene::PortalScene()
{

	redPortalRenderHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, DirectX::XMFLOAT3(0, 0, 100.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
	redPortal.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;
	redPortal.data.handleData = redPortalRenderHandle;

	greenPortalRenderHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, DirectX::XMFLOAT3(0, 100.0f, 100.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
	greenPortal.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;
	greenPortal.data.handleData = greenPortalRenderHandle;

	//mainRenderTarget.data.handleData = TextureResourceMgr::Instance()->LoadDivGraph(KazFilePathName::TestPath + "AnimationTest.png", 2, 1, 32, 32);

	changeFlag = true;
	animFlag = false;
	gameModeFlag = false;

	const float SCALE = 0.15f;
	const float HEIGHT = 10.0f;
	for (int i = 0; i < stages.size(); ++i)
	{
		stages[i] = std::make_unique<PortalRender>(i);

		stages[i]->stage[FLOOR_GREEN].data.transform.pos = { -30.0f,0.0f,0.0f };
		stages[i]->stage[FLOOR_GREEN].data.transform.scale = { 20.0f,1.0f,30.0f };
		stages[i]->stage[FLOOR_GREEN].data.color = { 0,150,0,255 };
		stages[i]->stage[FLOOR_GREEN].data.cameraIndex = i;
		//RedPortal
		redPortal.data.transform.pos = { -30.0f,HEIGHT,0.0f };
		redPortal.data.transform.scale = { SCALE,SCALE,SCALE };
		redPortal.data.cameraIndex = i;

		stages[i]->stage[FLOOR_RED].data.transform.pos = { 30.0f,0.0f,0.0f };
		stages[i]->stage[FLOOR_RED].data.transform.scale = { 20.0f,1.0f,30.0f };
		stages[i]->stage[FLOOR_RED].data.color = { 150,0,0,255 };
		stages[i]->stage[FLOOR_RED].data.cameraIndex = i;

		//GreenPortal
		greenPortal.data.transform.pos = { 30.0f,HEIGHT,0.0f };
		greenPortal.data.transform.scale = { SCALE,SCALE,SCALE };
		greenPortal.data.cameraIndex = i;
	}

	stages[2]->stage[FLOOR_GREEN].data.color = { 0,100,0,255 };
	stages[2]->stage[FLOOR_RED].data.color = { 100,0,0,255 };
}

PortalScene::~PortalScene()
{
}

void PortalScene::Init()
{
	initPos = { 0.0f, 3.0f, 45.0f };
	portal.Init(initPos);
	stringEffect.Init(initPos);

	portalFlame.Init(initPos, KazMath::Vec2<float>(41.5f, 23.5f));

	mulValue = { 10.0f,30.0f };
	mulValue2 = { 60.0f,60.0f };

	forceCameraDirVel.x = -90.0f;
}

void PortalScene::Finalize()
{
}

void PortalScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();
	ControllerInputManager *inputController = ControllerInputManager::Instance();

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
#pragma endregion

	bool upFlag = false;
	bool downFlag = false;
	bool leftFlag = false;
	bool rightFlag = false;
	bool doneFlag = false;
	bool releaseFlag = false;


	const int DEAD_ZONE = 3000;
	if (inputController->InputState(XINPUT_GAMEPAD_A))
	{
		doneFlag = true;
	}
	if (inputController->InputRelease(XINPUT_GAMEPAD_A))
	{
		releaseFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, UP_SIDE, DEAD_ZONE))
	{
		upFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, DOWN_SIDE, DEAD_ZONE))
	{
		downFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, LEFT_SIDE, DEAD_ZONE))
	{
		leftFlag = true;
	}
	if (inputController->InputStickState(LEFT_STICK, RIGHT_SIDE, DEAD_ZONE))
	{
		rightFlag = true;
	}


	KazMath::Vec3<float> vel = {};
	KazMath::Vec3<float> verticalVel = {};
	KazMath::Vec3<float> besideVel = {};

	float speed = 1.0f;
	if (upFlag)
	{
		vel.y = -speed;
		verticalVel = { -speed,-speed,0.0f };
	}
	if (downFlag)
	{
		vel.y = speed;
		verticalVel = { speed,speed,0.0f };
	}
	if (leftFlag)
	{
		vel.x = -speed;
		besideVel = { speed,speed,0.0f };
	}
	if (rightFlag)
	{
		vel.x = speed;
		besideVel = { -speed,-speed,0.0f };
	}


	KazMath::Vec2<float> joyStick;
	joyStick.x = inputController->GetJoyStickLXNum(0) / 32767.0f;
	joyStick.y = inputController->GetJoyStickLYNum(0) / 32767.0f;

	cursor.Input
	(
		upFlag,
		downFlag,
		leftFlag,
		rightFlag,
		doneFlag,
		releaseFlag,
		joyStick
	);


	KazMath::Vec2<float> cursorValue = cursor.GetValue();
	nowTargerPos += vel;

	upDownAngleVel =
	{
		mulValue.x * -cursorValue.y,
		mulValue.y * -cursorValue.y
	};
	leftRightAngleVel =
	{
		forceCameraDirVel.x + mulValue2.x * -cursorValue.x,
		forceCameraDirVel.x + mulValue2.y * -cursorValue.x
	};



	if (input->InputTrigger(DIK_SPACE))
	{
		changeFlag = !changeFlag;
	}



	if (input->InputTrigger(DIK_T))
	{
		
	}
	if (input->InputTrigger(DIK_R))
	{
		gameModeFlag = !gameModeFlag;
		eyePos = { 0.0f,5.0f,-10.0f };
		targetPos = { 0.0f,3.0f,0.0f };
	}

}

void PortalScene::Update()
{
	if (!gameModeFlag)
	{
		//デバック用
		eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
		targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	}
	else
	{
		//左右の角度変更のイージング
		{
			KazMath::Vec2<float> distance = leftRightAngleVel - trackLeftRightAngleVel;
			trackLeftRightAngleVel += distance * 0.1f;
		}
		//上下の角度変更のイージング
		{
			KazMath::Vec2<float> distance = upDownAngleVel - trackUpDownAngleVel;
			trackUpDownAngleVel += distance * 0.1f;
		}
		//左右の回転
		besidePoly.data.transform.pos =
		{
			cosf(KazMath::AngleToRadian(trackLeftRightAngleVel.x)) * 8.0f,
			0.0f,
			sinf(KazMath::AngleToRadian(trackLeftRightAngleVel.y)) * 8.0f
		};
		//上下の回転
		verticlaPoly.data.transform.pos =
		{
			0.0f,
			trackUpDownAngleVel.x,
			0.0f,
		};


		eyePos = KazMath::Vec3<float>(0.0f, 3.0f, 0.0f) + (besidePoly.data.transform.pos + verticlaPoly.data.transform.pos);
	}

	//赤ポータル
	redPortalCameraPos = stages[STAGE_RED]->stage[FLOOR_RED].data.transform.pos;
	redPortalCameraPos.y = 10.0f;
	CameraMgr::Instance()->Camera(redPortalCameraPos, redPortalCameraPos + KazMath::Vec3<float>(0.0f, 0.0f, -6.0f), { 0.0f,1.0f,0.0f }, STAGE_RED);

	//緑ポータル
	greenPortalCameraPos = stages[STAGE_RED]->stage[FLOOR_GREEN].data.transform.pos;
	greenPortalCameraPos.y = 10.0f;
	CameraMgr::Instance()->Camera(greenPortalCameraPos, greenPortalCameraPos + KazMath::Vec3<float>(0.0f, 0.0f, -6.0f), { 0.0f,1.0f,0.0f }, STAGE_GREEN);

	//ゲーム画面
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, STAGE_GAME);


	//ImGui::Begin("CheckDirtyFlag");
	//KazImGuiHelper::InputTransform3D(&box.data.transform);
	//KazImGuiHelper::InputVec4(&box.data.colorData.color, "Color");
	//ImGui::Checkbox("BillBoardFlag", &box.data.billBoardFlag);
	//ImGui::Checkbox("FlipX", &box.data.flip.x);
	//ImGui::Checkbox("FlipY", &box.data.flip.y);
	//ImGui::InputInt("Animation", &box.data.animationHandle.handle);
	//ImGui::End();


	portal.Update();
	stringEffect.Update();
	portalFlame.Update();
	cursor.Update();
}

void PortalScene::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);

	RenderTargetStatus::Instance()->PrepareToChangeBarrier(redPortalRenderHandle);
	RenderTargetStatus::Instance()->ClearRenderTarget(redPortalRenderHandle);

	stages[STAGE_RED]->bg.Draw();
	for (int i = 0; i < stages[STAGE_RED]->stage.size(); ++i)
	{
		stages[STAGE_RED]->stage[i].Draw();
	}

	RenderTargetStatus::Instance()->PrepareToChangeBarrier(greenPortalRenderHandle, redPortalRenderHandle);
	RenderTargetStatus::Instance()->ClearRenderTarget(greenPortalRenderHandle);

	stages[STAGE_GREEN]->bg.Draw();
	for (int i = 0; i < stages[STAGE_GREEN]->stage.size(); ++i)
	{
		stages[STAGE_GREEN]->stage[i].Draw();
	}

	RenderTargetStatus::Instance()->PrepareToCloseBarrier(greenPortalRenderHandle);
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();

	stages[STAGE_GAME]->bg.Draw();
	for (int i = 0; i < stages[STAGE_GAME]->stage.size(); ++i)
	{
		stages[STAGE_GAME]->stage[i].Draw();
	}

	greenPortal.Draw();
	redPortal.Draw();

	cursor.Draw();

}

int PortalScene::SceneChange()
{
	return 0;
}
