#include "DebugStageScene.h"
#include"../Camera/CameraMgr.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Loader/FbxModelResourceMgr.h"
#include"../Helper/ResourceFilePass.h"
#include"../RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../Game/Stage/BlockParticleStage.h"
#include"../Game/Stage/FirstStage.h"
#include"../Game/Stage/RezStage.h"


#include"../KazLibrary/Input/ControllerInputManager.h"

DebugStageScene::DebugStageScene()
{
	//stages[0] = std::make_shared<BlockParticleStage>();
	stages[0] = std::make_shared<FirstStage>();
	stages[1] = std::make_shared<RezStage>();
	stages[2] = std::make_shared<BlockParticleStage>();

	renderTarget = std::make_unique<GameRenderTarget>(KazMath::Color(29, 19, 72, 255));
	mainRenderTarget.data.handleData = renderTarget->GetGameRenderTargetHandle();
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	stages[0]->startFlag = true;
	stages[1]->startFlag = true;
	CameraMgr::Instance()->CameraSetting(60.0f, 100000.0f, 0);
}

DebugStageScene::~DebugStageScene()
{
}

void DebugStageScene::Init()
{
	player.Init({ 0.0f,0.0f,15.0f }, false, false);
	cameraWork.Init();
	cursor.Init();
	//mapchipTool.Init();
}

void DebugStageScene::Finalize()
{
}

void DebugStageScene::Input()
{
	//KeyBoradInputManager *input = KeyBoradInputManager::Instance();
	ControllerInputManager *inputController = ControllerInputManager::Instance();

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

	//mapchipTool.Input(input->MouseInputTrigger(MOUSE_INPUT_LEFT), input->MouseInputTrigger(MOUSE_INPUT_RIGHT), input->GetMousePoint());


	ImGui::Begin("Stage");
	ImGui::InputInt("StageNum", &stageNum);
	ImGui::End();
}

void DebugStageScene::Update()
{
	eyePos = cameraWork.GetEyePos();
	targetPos = cameraWork.GetTargetPos();
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });

	player.Update();
	cursor.Update();
	cameraWork.Update(cursor.GetValue(), &player.pos, false);
	stages[stageNum]->Update();

	//mapchipTool.Update();
}

void DebugStageScene::Draw()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);
	//renderTarget->SetRenderTarget();
	player.Draw();
	stages[stageNum]->Draw();
	//mapchipTool.Draw();
	//renderTarget->Draw();

	//mainRenderTarget.Draw();
	cursor.Draw();
}

int DebugStageScene::SceneChange()
{
	return SCENE_NONE;
}
