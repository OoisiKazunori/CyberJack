#include"../Scene/TitleScene.h"
#include"../DirectXCommon/Base.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include"../KazLibrary/Camera/CameraMgr.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../Imgui/MyImgui.h"
#include"../Helper/ResourceFilePass.h"
#include"../Easing/easing.h"

TitleScene::TitleScene()
{
	std::vector<MultiRenderTargetData> lData;
	lData.push_back(MultiRenderTargetData());
	lData.push_back(MultiRenderTargetData());
	lData[0].graphSize = { WIN_X,WIN_Y };
	lData[0].backGroundColor = KazMath::Color(14, 12, 13, 255).ConvertColorRateToXMFLOAT3();
	lData[1].graphSize = { WIN_X,WIN_Y };
	lData[1].backGroundColor = KazMath::Color(0, 0, 0, 255).ConvertColorRateToXMFLOAT3();

	handles =
		RenderTargetStatus::Instance()->CreateMultiRenderTarget(lData, DXGI_FORMAT_R8G8B8A8_UNORM);
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;
	mainRenderTarget.data.handleData = handles[0];

	luminaceTex.data.pipelineName = PIPELINE_NAME_SPRITE_LUMI;
	luminaceTex.data.handleData = handles[0];
	luminaceTex.data.addHandle.handle[0] = handles[1];
	luminaceTex.data.addHandle.paramType[0] = GRAPHICS_PRAMTYPE_TEX2;
	luminaceTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };

	addHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
	addRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	addRenderTarget.data.pipelineName = PIPELINE_NAME_BLOOM;
	addRenderTarget.data.addHandle.paramType[0] = GRAPHICS_PRAMTYPE_TEX;
	addRenderTarget.data.addHandle.paramType[1] = GRAPHICS_PRAMTYPE_TEX2;
	addRenderTarget.data.addHandle.paramType[2] = GRAPHICS_PRAMTYPE_TEX3;
	addRenderTarget.data.addHandle.paramType[3] = GRAPHICS_PRAMTYPE_TEX4;

	buler[0] = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X, WIN_Y));
	buler[1] = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X / 2, WIN_Y / 2));
	buler[2] = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X / 3, WIN_Y / 3));
	buler[3] = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X / 4, WIN_Y / 4));

	gridR[0] = std::make_unique<DrawGrid>(KazMath::Color(14, 12, 13, 255));
	gridR[1] = std::make_unique<DrawGrid>(KazMath::Color(14, 12, 13, 255));
	CameraMgr::Instance()->CameraSetting(60.0f, 10000.0f, 0);


	titleLogoR.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TitlePath + "TitleName.png");
	lineLogoR.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::UIPath + "Line.png");
	startButtonR.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TitlePath + "Start.png");

	titleLogoR.data.transform.pos = { 550.0f,260.0f };
	lineLogoR.data.transform.pos = { 700.0f,355.0f };
	lineLogoR.data.transform.scale = { 1000.0f,0.5f };
	startButtonR.data.transform.pos = { 270.0f,380.0f };
	startButtonR.data.transform.scale = { 0.5f,0.5f };
}

TitleScene::~TitleScene()
{
	//レンダーターゲットの破棄
	RenderTargetStatus::Instance()->DeleteRenderTarget(addHandle);
	RenderTargetStatus::Instance()->DeleteMultiRenderTarget(handles);
	RenderTargetStatus::Instance()->DeleteRenderTarget(potalTexHandle);
}

void TitleScene::Init()
{
	sceneNum = -1;
	cameraIndex = 0;

	KazMath::Color lBaseColor(155, 155, 155, 155);
	KazMath::Color lFlashColor(155, 155, 155, 255);
	std::array<KazMath::Color, 2>lFlashColorArray = { lBaseColor,lFlashColor };
	gridR[0]->Init(true, 800.0f, -200.0f, &cameraIndex, false, false, lFlashColorArray);
	gridR[1]->Init(true, 800.0f, -200.0f, &cameraIndex, false, false, lFlashColorArray);

	buttonTimer = 0;
	buttonFlashFlag = true;

	selectMenu.Init(MENU_MAX_NUM - 1);

	basePos[0] = { 150.0f,375.0f };
	basePos[1] = { 150.0f,380.0f };
	pressStartButtonFlag = false;

	startGameFlag = false;

	for (int i = 0; i < playerAroundCircleR.size(); ++i)
	{
		playerAroundCircleR[i].data.change3DFlag = true;
		playerAroundCircleR[i].data.radius = 0.0f;
		playerAroundCircleR[i].data.transform.pos = { 0.0f,0.0f,15.0f };
		playerAroundCircleR[i].data.fillFlag = false;
	}
	playerAroundCircleR[0].data.transform.rotation = { 50.0f,50.0f,50.0f };
	playerAroundCircleR[1].data.transform.rotation = { 10.0f,10.0f,10.0f };

	std::array< KazMath::Transform3D, 4> lTransform;
	lTransform[0].pos = { 250.0f,200.0f,0.0f };
	lTransform[1].pos = { 300.0f,550.0f,0.0f };
	lTransform[2].pos = { 1000.0f,150.0f,0.0f };
	lTransform[3].pos = { 1100.0f,530.0f,0.0f };
	std::array<KazMath::Vec2<float>, 4> lScale;
	lScale[0] = { 2.0f,2.0f };
	lScale[1] = { 3.0f,2.0f };
	lScale[2] = { 2.5f,2.0f };
	lScale[3] = { 2.0f,2.6f };
	std::array<int, 4> lLogMax;
	lLogMax[0] = 11;
	lLogMax[1] = 11;
	lLogMax[2] = 11;
	lLogMax[3] = 14;

	for (int i = 0; i < logWindow.size(); ++i)
	{
		logWindow[i].Init(lTransform[i], lScale[i], false, WINDOW_2D);
		log[i].Init(lTransform[i].pos.ConvertVec2() + KazMath::Vec2<float>(-128.0f / 2.0f + 10.0f, -128.0f / 2.0f + 25.0f) * lScale[i], false, lLogMax[i]);
		logWriteTimer[i] = 0;
		logWriteMaxTime[i] = KazMath::Rand(20, 10);
	}
	logAppearTimer = 0;
	logAppearMaxTime = KazMath::Rand(20, 10);
	logWindowIndex = 0;
	appearTimer = 0;
	gridTopRate = 0.0f;
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
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();

#pragma region カメラ操作
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 100.0f;
	float debugSideSpeed = 1.0f;
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
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}


	if (input->InputState(DIK_RIGHTARROW))
	{
		angle.x += debugSideSpeed;
	}
	if (input->InputState(DIK_LEFTARROW))
	{
		angle.x += -debugSideSpeed;
	}

	if (input->InputState(DIK_UPARROW))
	{
		angle.y += debugSideSpeed;
	}
	if (input->InputState(DIK_DOWNARROW))
	{
		angle.y += -debugSideSpeed;
	}

#pragma endregion


	selectMenu.Input(ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_DPAD_DOWN), ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_DPAD_UP));

	if (ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_A) && pressStartButtonFlag && !startGameFlag)
	{
		switch (selectMenu.GetNumber())
		{
		case 0:
			//ゲーム開始
			startGameFlag = true;
			player.Init({ 0.0f,0.0f,15.0f }, false,true);
			break;

		case 1:
			//ゲーム終了
			break;
		default:
			break;
		}
	}
	if (ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_A) && !pressStartButtonFlag)
	{
		pressStartButtonFlag = true;
		menuR[0].Init(basePos[0], "[play         ]");
		menuR[1].Init(basePos[1], "[exit         ]");
	}
}

void TitleScene::Update()
{
	//デバック用
	//eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle, 0.1f);
	//targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);

	gridR[0]->Update(-1.0f);

	selectMenu.Update();
	for (int i = 0; i < menuR.size(); ++i)
	{
		menuR[i].Update(selectMenu.GetNumber());
	}


	//ImGui::Begin("UI");
	//KazImGuiHelper::InputTransform2D("TitleLogo", &titleLogoR.data.transform);
	//KazImGuiHelper::InputTransform2D("LineLogo", &lineLogoR.data.transform);
	//KazImGuiHelper::InputTransform2D("StartButton", &startButtonR.data.transform);
	//KazImGuiHelper::InputVec2("MenuStart", &basePos[0]);
	//KazImGuiHelper::InputVec2("MenuExit", &basePos[1]);
	//ImGui::End();

	++buttonTimer;
	if (20 <= buttonTimer)
	{
		buttonTimer = 0;
		buttonFlashFlag = !buttonFlashFlag;
	}

	//ゲーム開始時の演出ーーーーーー
	if (!startGameFlag)
	{
		return;
	}

	++appearTimer;
	bool lEndFlag = KazMath::ConvertSecondToFlame(4) <= appearTimer;

	if (KazMath::ConvertSecondToFlame(2) <= appearTimer)
	{
		Rate(&gridTopRate, 0.01f, 1.0f);
		gridR[1]->Update(-200.0f + EasingMaker(Out, Cubic, gridTopRate) * 1000.0f, 1.0f <= gridTopRate);
	}
	if (KazMath::ConvertSecondToFlame(3) <= appearTimer)
	{
		KazMath::Larp({ 0.0f,3.0f,-8.0f + 15.0f }, &eyePos, 0.05f);
		KazMath::Larp({ 0.0f,3.0f,15.0f }, &targetPos, 0.05f);
	}

	for (int i = 0; i < playerAroundCircleR.size(); ++i)
	{
		playerAroundCircleR[i].data.transform.rotation.x += 2.0f;
		playerAroundCircleR[i].data.transform.rotation.y += 2.0f;
		playerAroundCircleR[i].data.transform.rotation.z += 2.0f;

		if (playerAroundCircleR[i].data.radius <= 5.0f && !lEndFlag)
		{
			playerAroundCircleR[i].data.radius += 0.1f;
		}
		else if (0.0f <= playerAroundCircleR[i].data.radius && lEndFlag)
		{
			playerAroundCircleR[i].data.radius += -0.1f;
		}
	}

	if (lEndFlag)
	{
		for (int i = 0; i < logWriteMaxTime.size(); ++i)
		{
			log[i].Finalize();
			logWindow[i].End();
		}
	}



	for (int i = 0; i < menuR.size(); ++i)
	{
		menuR[i].Finalize();
	}
	player.Update();
	for (int i = 0; i < log.size(); ++i)
	{
		log[i].Update();
	}

	++logAppearTimer;
	if (logAppearMaxTime <= logAppearTimer && logWindowIndex < logWindow.size())
	{
		logAppearMaxTime = KazMath::Rand(20, 10);
		logAppearTimer = 0;
		logWindow[logWindowIndex].Start();
		++logWindowIndex;
	}

	for (int i = 0; i < logWindow.size(); ++i)
	{
		if (logWindow[i].IsEnd())
		{
			continue;
		}

		++logWriteTimer[i];
		if (logWriteMaxTime[i] <= logWriteTimer[i])
		{
			logWriteMaxTime[i] = KazMath::Rand(20, 10);
			log[i].WriteLog("WriteTestLog", 1.0f);
			logWriteTimer[i] = 0;
		}
	}
	for (int i = 0; i < logWindow.size(); ++i)
	{
		logWindow[i].Update();
	}


	if (lEndFlag && playerAroundCircleR[0].data.radius <= 0.0f)
	{
		sceneNum = 1;
	}
}

void TitleScene::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(handles[0]);
	RenderTargetStatus::Instance()->ClearRenderTarget(handles[0]);

	if (startGameFlag)
	{
		for (int i = 0; i < playerAroundCircleR.size(); ++i)
		{
			playerAroundCircleR[i].Draw();
		}
		player.Draw();
		for (int i = 0; i < log.size(); ++i)
		{
			log[i].Draw();
		}

		for (int i = 0; i < logWindow.size(); ++i)
		{
			logWindow[i].Draw();
		}
	}

	for (int i = 0; i < gridR.size(); ++i)
	{
		gridR[i]->Draw();
	}

	RenderTargetStatus::Instance()->PrepareToChangeBarrier(addHandle, handles[0]);
	RenderTargetStatus::Instance()->ClearRenderTarget(addHandle);
	//輝度抽出
	luminaceTex.Draw();
	RenderTargetStatus::Instance()->PrepareToCloseBarrier(addHandle);
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	//ゲーム画面描画
	mainRenderTarget.Draw();

	addRenderTarget.data.handleData = addHandle;
	addRenderTarget.data.addHandle.handle[0] = buler[0]->BlurImage(addHandle);
	addRenderTarget.data.addHandle.handle[1] = buler[1]->BlurImage(addRenderTarget.data.addHandle.handle[0]);
	addRenderTarget.data.addHandle.handle[2] = buler[2]->BlurImage(addRenderTarget.data.addHandle.handle[1]);
	addRenderTarget.data.addHandle.handle[3] = buler[3]->BlurImage(addRenderTarget.data.addHandle.handle[2]);
	addRenderTarget.Draw();

	if (!startGameFlag)
	{
		titleLogoR.Draw();
		lineLogoR.Draw();
	}
	if (buttonFlashFlag && !pressStartButtonFlag)
	{
		startButtonR.Draw();
	}
	else if (pressStartButtonFlag)
	{
		for (int i = 0; i < menuR.size(); ++i)
		{
			menuR[i].Draw();
		}
	}
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