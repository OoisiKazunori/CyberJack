#include "TutorialMovie.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"

TutorialMovie::TutorialMovie() :
	gauge(TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TutorialPath + "Flame.png"),
		TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TutorialPath + "Gauge.png"))
{
	moviePlayer = std::make_unique<DirectX12MoviePlayer>();
	renderTargetHandle = RenderTargetStatus::Instance()->CreateRenderTarget(KazMath::Vec2<UINT>(WIN_X, WIN_Y), { 0,0,0 }, DXGI_FORMAT_R8G8B8A8_UNORM);
	outputRenderTargetHandle = RenderTargetStatus::Instance()->CreateRenderTarget(KazMath::Vec2<UINT>(WIN_X, WIN_Y), { 0,0,0 }, DXGI_FORMAT_R8G8B8A8_UNORM);

	movieRender.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	outputRender.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	normalRender.data.transform.pos = { WIN_X / 2.0f,WIN_Y / 2.0f };
	movieRender.data.transform.scale = { 0.67f,0.7f };


	vhsSeedHandle = outputRender.CreateConstBuffer(sizeof(float), typeid(float).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	outputRender.data.pipelineName = PIPELINE_NAME_SPRITE_VHS;
	outputRender.data.handleData = renderTargetHandle;

	noiseSeedHandle = normalRender.CreateConstBuffer(sizeof(float), typeid(float).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	normalRender.data.pipelineName = PIPELINE_NAME_SPRITE_NOISE;
	normalRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "NormalMovie.png");


	moviePlayer->SetMediaSource(KazFilePathName::TestPath + "test.mp4");
	tutorialText.Init("L Stick\nA Button", {});

}

void TutorialMovie::Init()
{
	seedNum = 0;
	startMovieFlag = false;
	gauge.Init(100);
}

void TutorialMovie::Update()
{
	//VHSの動画描画
	if (startMovieFlag)
	{
		moviePlayer->Play();
		moviePlayer->TranferFrame();

		movieRender.data.buff = moviePlayer->GetBuffer();
		movieRender.data.handleData = moviePlayer->GetDescriptorHeapHandle();
	}
	else if (startNoiseFlag)
	{
		normalRender.TransData(&seedNum, noiseSeedHandle, typeid(float).name());
	}
	else
	{
		float lstopNoiseNum = -1;
		normalRender.TransData(&lstopNoiseNum, noiseSeedHandle, typeid(float).name());
	}

	seedNum += 1.0f;
	outputRender.TransData(&seedNum, vhsSeedHandle, typeid(float).name());


	if (gauge.IsMax())
	{
		tutorialText.Succeed();
	}

	ImGui::Begin("Gauge");
	if (ImGui::Button("Add"))
	{
		gauge.Add(10);
	}
	ImGui::End();


	tutorialText.Update();
	gauge.Update();

}

void TutorialMovie::Draw()
{
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(renderTargetHandle);
	RenderTargetStatus::Instance()->ClearRenderTarget(renderTargetHandle);
	if (startMovieFlag)
	{
		tutorialText.Draw();
		gauge.Draw();
		movieRender.Draw();
	}
	else
	{
		normalRender.Draw();
	}
	RenderTargetStatus::Instance()->PrepareToChangeBarrier(outputRenderTargetHandle, renderTargetHandle);
	RenderTargetStatus::Instance()->ClearRenderTarget(outputRenderTargetHandle);
	outputRender.Draw();
	RenderTargetStatus::Instance()->PrepareToCloseBarrier(outputRenderTargetHandle);
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
}

void TutorialMovie::Play()
{
	startMovieFlag = true;
	startNoiseFlag = false;
	stopFlag = false;
}

void TutorialMovie::Noise()
{
	startMovieFlag = false;
	startNoiseFlag = true;
	stopFlag = false;
}

void TutorialMovie::Stop()
{
	startMovieFlag = false;
	startNoiseFlag = false;
	stopFlag = true;
}

bool TutorialMovie::End()
{
	//動画時間まで到達したら、最初に巻き戻しトリガー判定でtrueを返す
	bool lIsEndFlag = moviePlayer->IsEnd();
	if (lIsEndFlag)
	{
		moviePlayer->Restart();
	}
	return lIsEndFlag;
}
