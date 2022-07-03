#include "PortalScene.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Input/ControllerInputManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/KazImGuiHelper.h"

PortalScene::PortalScene()
{
	std::vector<MultiRenderTargetData> data;
	data.push_back(MultiRenderTargetData());
	data.push_back(MultiRenderTargetData());
	data[0].graphSize = { WIN_X,WIN_Y };
	data[0].backGroundColor = BG_COLOR;
	data[1].graphSize = { WIN_X,WIN_Y };
	data[1].backGroundColor = { 0.0f,0.0f,0.0f };

	multipassHandle =
		RenderTargetStatus::Instance()->CreateMultiRenderTarget(data, DXGI_FORMAT_R8G8B8A8_UNORM);
	mainRenderTarget.data.handleData = multipassHandle[0];
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;


	addHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
	addRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	addRenderTarget.data.pipelineName = PIPELINE_NAME_ADDBLEND;


	luminaceTex.data.pipelineName = PIPELINE_NAME_SPRITE_LUMI;
	luminaceTex.data.handleData = multipassHandle[0];
	luminaceTex.data.addHandle.handle[0] = multipassHandle[1];
	luminaceTex.data.addHandle.paramType[0] = GRAPHICS_PRAMTYPE_TEX2;
	luminaceTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };

	buler = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X, WIN_Y));

	//mainRenderTarget.data.handleData = TextureResourceMgr::Instance()->LoadDivGraph(KazFilePathName::TestPath + "AnimationTest.png", 2, 1, 32, 32);

	changeFlag = true;
	animFlag = false;
	gameModeFlag = false;
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

	if (changeFlag)
	{
		box.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "tex.png");
	}
	else
	{
		box.data.handleData = TextureResourceMgr::Instance()->LoadDivGraph(KazFilePathName::TestPath + "AnimationTest.png", 2, 1, 32, 32);
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
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);


	ImGui::Begin("CheckDirtyFlag");
	KazImGuiHelper::InputTransform3D(&box.data.transform);
	KazImGuiHelper::InputVec4(&box.data.color.color, "Color");
	ImGui::Checkbox("BillBoardFlag", &box.data.billBoardFlag);
	ImGui::Checkbox("FlipX", &box.data.flip.x);
	ImGui::Checkbox("FlipY", &box.data.flip.y);
	ImGui::InputInt("Animation", &box.data.animationHandle.handle);
	ImGui::End();


	portal.Update();
	stringEffect.Update();
	portalFlame.Update();
	cursor.Update();
	//WirteCpuLineData::Instance()->importFlag = false;
}

void PortalScene::Draw()
{
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(multipassHandle[0]);
	RenderTargetStatus::Instance()->ClearRenderTarget(multipassHandle[0]);
	//portal.Draw();
	//stringEffect.Draw();
	//portalFlame.Draw();
	bg.Draw();

	box.Draw();
	
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, L"Draw Luminance");
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(addHandle, multipassHandle[0]);
	RenderTargetStatus::Instance()->ClearRenderTarget(addHandle);
	luminaceTex.Draw();
	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());


	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, L"Draw Main RenderTarget");
	RenderTargetStatus::Instance()->PrepareToCloseBarrier(addHandle);
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	mainRenderTarget.Draw();
	//addRenderTarget.data.handleData = buler->BlurImage(addHandle);
	//addRenderTarget.Draw();


	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

	cursor.Draw();

}

int PortalScene::SceneChange()
{
	return 0;
}
