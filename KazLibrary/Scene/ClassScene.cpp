#include "ClassScene.h"
#include"../Camera/CameraMgr.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Loader/FbxModelResourceMgr.h"
#include"../Helper/ResourceFilePass.h"
#include"../RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Imgui/MyImgui.h"

ClassScene::ClassScene()
{
	modelRender = std::make_unique<FbxModelRender>();

	gameRenderTarget = std::make_unique<Sprite2DRender>();
	mainRenderTarget = std::make_unique<Sprite2DRender>();

	gameRenderTarget->data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	mainRenderTarget->data.transform.pos = { WIN_X / 2.0f ,WIN_Y / 2.0f };
	mainRenderTarget->data.pipelineName = PIPELINE_NAME_SPRITE_VHS;


	handle = mainRenderTarget->CreateConstBuffer(sizeof(constBufferVhsData), typeid(constBufferVhsData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);


	gameRenderTarget->data.handle =
		RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, BG_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM);

	mainRenderTarget->data.handle =
		RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, BG_COLOR, DXGI_FORMAT_R8G8B8A8_UNORM);

	std::vector<MultiRenderTargetData>data;
	data.push_back(MultiRenderTargetData());
	data.push_back(MultiRenderTargetData());

	data[0].graphSize = { WIN_X,WIN_Y };
	data[0].backGroundColor = BG_COLOR;
	data[1].graphSize = { WIN_X,WIN_Y };
	data[1].backGroundColor = { 255.0f,0.0f,0.0f };
	multiHandle = RenderTargetStatus::Instance()->CreateMultiRenderTarget(data, DXGI_FORMAT_R8G8B8A8_UNORM);


	//circle.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "circle.png");
	//circle.data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;
}

ClassScene::~ClassScene()
{
}

void ClassScene::Init()
{
	modelRender->data.handle =
		FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "boneTest.fbx");

	modelRender->data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
}

void ClassScene::Finalize()
{
}

void ClassScene::Input()
{

	KeyBoradInputManager *input = KeyBoradInputManager::Instance();
	//	ControllerInputManager *inputController = ControllerInputManager::Instance();

#pragma region カメラ操作
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 1;
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
}

void ClassScene::Update()
{
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });

	data.time += 0.01f;
	mainRenderTarget->TransData(&data, handle, typeid(data).name());

	modelRender->data.isPlay = true;


	hitFlag = KazMath::CheckRayAndCircle(rayStartPos, rayEndPos, circleCentralPos, radius);

	ImGui::Begin("HitBox");
	ImGui::SliderFloat("rayStartPosX", &rayStartPos.m128_f32[0], 0.0f, WIN_X);
	ImGui::SliderFloat("rayStartPosY", &rayStartPos.m128_f32[1], 0.0f, WIN_Y);
	ImGui::SliderFloat("rayEndPosX", &rayEndPos.m128_f32[0], 0.0f, WIN_X);
	ImGui::SliderFloat("rayEndPosY", &rayEndPos.m128_f32[1], 0.0f, WIN_Y);
	ImGui::SliderFloat("circleCentralPosX", &circleCentralPos.m128_f32[0], 0.0f, WIN_X);
	ImGui::SliderFloat("circleCentralPosY", &circleCentralPos.m128_f32[1], 0.0f, WIN_Y);
	ImGui::InputFloat("radius", &radius);
	if (hitFlag)
	{
		ImGui::Text("Hit");
		circle.data.color = { 255.0f,0.0f,0.0f,255.0f };
	}
	else
	{
		ImGui::Text("NoHit");
		circle.data.color = { 255.0f,255.0f,255.0f,255.0f };
	}
	ImGui::End();

	circle.data.transform.pos = circleCentralPos;
	circle.data.radius = radius;
	line.data.startPos = rayStartPos;
	line.data.endPos = rayEndPos;
}

void ClassScene::Draw()
{
	circle.Draw();
	line.Draw();
}

int ClassScene::SceneChange()
{
	return SCENE_NONE;
}
