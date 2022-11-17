#include "TutorialMovie.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"

TutorialMovie::TutorialMovie()
{
	moviePlayer = std::make_unique<DirectX12MoviePlayer>();
	renderTargetHandle = RenderTargetStatus::Instance()->CreateRenderTarget(KazMath::Vec2<UINT>(WIN_X, WIN_Y), { 0,0,0 }, DXGI_FORMAT_R8G8B8A8_UNORM);

	movieRender.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
}

void TutorialMovie::Init()
{
	moviePlayer->SetMediaSource(KazFilePathName::TestPath + "test1m.mp4");

	seedHandle = movieRender.CreateConstBuffer(sizeof(float), typeid(float).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	seedNum = 0;

	startMovieFlag = false;
}

void TutorialMovie::Update()
{
	//VHS‚Ì“®‰æ•`‰æ
	if (startMovieFlag)
	{
		moviePlayer->Play();
		moviePlayer->TranferFrame();
		movieRender.data.pipelineName = PIPELINE_NAME_SPRITE_VHS;
		movieRender.data.buff = moviePlayer->GetBuffer();
		movieRender.data.handleData = moviePlayer->GetDescriptorHeapHandle();

		movieRender.TransData(&seedNum, seedHandle, typeid(float).name());
	}

	seedNum += 1.0f;
	movieRender.data.transform.scale = { 0.67f,0.7f };
	ImGui::Begin("Size");
	KazImGuiHelper::InputVec2("Size", &movieRender.data.transform.scale);
	ImGui::End();

}

void TutorialMovie::Draw()
{
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(renderTargetHandle);
	if (startMovieFlag)
	{
		movieRender.Draw();
	}
	RenderTargetStatus::Instance()->PrepareToCloseBarrier(renderTargetHandle);
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
}

void TutorialMovie::Play()
{
	startMovieFlag = true;
}
