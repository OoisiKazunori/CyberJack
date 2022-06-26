#include "PortalScene.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Input/ControllerInputManager.h"

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
	mainRenderTarget.data.handle = multipassHandle[0];
	mainRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	mainRenderTarget.data.pipelineName = PIPELINE_NAME_SPRITE_NOBLEND;


	addHandle = RenderTargetStatus::Instance()->CreateRenderTarget({ WIN_X,WIN_Y }, XMFLOAT3(0.0f, 0.0f, 0.0f), DXGI_FORMAT_R8G8B8A8_UNORM);
	addRenderTarget.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	addRenderTarget.data.pipelineName = PIPELINE_NAME_ADDBLEND;


	luminaceTex.data.pipelineName = PIPELINE_NAME_SPRITE_LUMI;
	luminaceTex.data.handle = multipassHandle[0];
	luminaceTex.data.addHandle.handle[0] = multipassHandle[1];
	luminaceTex.data.addHandle.paramType[0] = GRAPHICS_PRAMTYPE_TEX2;
	luminaceTex.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };

	buler = std::make_unique<GaussianBuler>(KazMath::Vec2<UINT>(WIN_X, WIN_Y));

}

PortalScene::~PortalScene()
{
}

void PortalScene::Init()
{
	initPos = { 0.0f,10.0f,30.0f };

	portal.Init(initPos);
	stringEffect.Init(initPos);

	portalFlame.Init(initPos, KazMath::Vec2<float>(60.0f, 30.0f));
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
#pragma endregion


	if (input->InputTrigger(DIK_SPACE))
	{
		portal.Start();
	}
	if (input->InputTrigger(DIK_R))
	{
		portal.Init(initPos);
	}

	if (input->InputTrigger(DIK_T))
	{
		stringEffect.Init(initPos);
	}
	if (input->InputTrigger(DIK_B))
	{
		stringEffect.Start();
	}
}

void PortalScene::Update()
{
	//デバック用
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f }, 0);




	portal.Update();
	stringEffect.Update();
	portalFlame.Update();
}

void PortalScene::Draw()
{
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(multipassHandle[0]);
	RenderTargetStatus::Instance()->ClearRenderTarget(multipassHandle[0]);
	bg.Draw();
	portal.Draw();
	stringEffect.Draw();
	portalFlame.Draw();
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(addHandle, multipassHandle[0]);
	RenderTargetStatus::Instance()->ClearRenderTarget(addHandle);
	luminaceTex.Draw();
	RenderTargetStatus::Instance()->PrepareToCloseBarrier(addHandle);
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();


	mainRenderTarget.Draw();
	addRenderTarget.data.handle = buler->BlurImage(addHandle);
	addRenderTarget.Draw();

}

int PortalScene::SceneChange()
{
	return 0;
}
