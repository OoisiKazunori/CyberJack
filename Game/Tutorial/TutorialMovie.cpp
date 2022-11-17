#include "TutorialMovie.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

TutorialMovie::TutorialMovie()
{
	renderTarget = std::make_unique<GameRenderTarget>(KazMath::Color(0, 0, 0, 255));
	moviePlayer = std::make_unique<DirectX12MoviePlayer>();
}

void TutorialMovie::Init()
{

	moviePlayer->SetMediaSource(KazFilePathName::TestPath + "test1m.mp4");

	seedHandle = render.CreateConstBuffer(sizeof(float), typeid(float).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	seedNum = 0;

	startMovieFlag = false;
	startNoiseFlag = false;

	greenBackHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "");
}

void TutorialMovie::Update()
{
	//何も表示されていない状態
	render.data.buff = TextureResourceMgr::Instance()->buffers->GetBufferData(greenBackHandle);

	//VHSの動画描画
	if (startMovieFlag)
	{
		moviePlayer->Play();
		moviePlayer->TranferFrame();
		render.data.pipelineName = PIPELINE_NAME_SPRITE_VHS;
		render.data.buff = moviePlayer->GetBuffer();
		render.data.handleData = moviePlayer->GetDescriptorHeapHandle();
	}

	//ノイズの描画
	if (startNoiseFlag)
	{
		render.data.pipelineName = PIPELINE_NAME_SPRITE_NOISE;
	}

	seedNum += 1.0f;
	render.TransData(&seedNum, seedHandle, typeid(float).name());

	ImGui::Begin("Size");
	KazImGuiHelper::InputVec2("Size", &render.data.transform.scale);
	ImGui::End();
}

void TutorialMovie::Draw()
{
	renderTarget->SetRenderTarget();
	render.Draw();
	renderTarget->Draw();
}

void TutorialMovie::Noise()
{
	startMovieFlag = false;
	startNoiseFlag = true;
}

void TutorialMovie::Play()
{
	startMovieFlag = true;
	startNoiseFlag = false;
}
