#include "AnnounceStageUI.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

AnnounceStageUI::AnnounceStageUI()
{
	flameTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "Flame.png");
	areaTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "Area.png");

	numberHandle[0] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum0.png");
	numberHandle[1] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum1.png");
	numberHandle[2] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum2.png");
	numberHandle[3] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum3.png");
	numberHandle[4] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum4.png");
	numberHandle[5] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum5.png");
	numberHandle[6] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum6.png");
	numberHandle[7] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum7.png");
	numberHandle[8] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum8.png");
	numberHandle[9] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorNum9.png");

	numberTex.data.handle = numberHandle[0];

	numberTex.data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;
	areaTex.data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;

	basePos = { 640.0f,150.0f };
	areaTex.data.transform.scale = { 2.0f,2.0f };
	areaTex.data.transform.pos = { 590.0f,153.0f };
}

void AnnounceStageUI::Init()
{
	flameTex.data.transform.scale = { 0.0f,0.1f };
	larpScale = { 0.0f,0.1f };
	startFlag = false;
}

void AnnounceStageUI::Update()
{
	if (startFlag)
	{
		flameTex.data.transform.pos = basePos;
		larpScale.m128_f32[0] = 4.0f;

		float mul = 0.3f;
		//ƒEƒBƒ“ƒhƒE‚ğ‰¡‚ÉL‚°‚é
		KazMath::Larp(larpScale.m128_f32[0], &flameTex.data.transform.scale.m128_f32[0], mul);


		if (3.9f <= flameTex.data.transform.scale.m128_f32[0])
		{
			flameTex.data.transform.scale.m128_f32[0] = 4.0f;
		}
		//ƒEƒBƒ“ƒhƒE‚ğc‚ÉL‚°‚é
		if (static_cast<int>(larpScale.m128_f32[0]) == static_cast<int>(flameTex.data.transform.scale.m128_f32[0]))
		{
			larpScale.m128_f32[1] = 1.0f;
		}
		KazMath::Larp(larpScale.m128_f32[1], &flameTex.data.transform.scale.m128_f32[1], mul);


		//•¶š‚ğˆê•¶š‚¸‚Â•`‰æ‚·‚é
		areaNum;


	}



	//‚µ‚Î‚ç‚­•`‰æ‚µ‚½‚ç“oê‰‰o‚Æ‹t‚Ì–‚ğ‚â‚é


	ImGui::Begin("Layer");
	ImGui::InputFloat("PosX", &areaTex.data.transform.pos.m128_f32[0]);
	ImGui::InputFloat("PosY", &areaTex.data.transform.pos.m128_f32[1]);
	ImGui::InputFloat("PosZ", &areaTex.data.transform.pos.m128_f32[2]);
	ImGui::InputFloat("ScaleX", &areaTex.data.transform.scale.m128_f32[0]);
	ImGui::InputFloat("ScaleY", &areaTex.data.transform.scale.m128_f32[1]);
	ImGui::InputFloat("ScaleZ", &areaTex.data.transform.scale.m128_f32[2]);
	ImGui::End();

}

void AnnounceStageUI::Draw()
{
	numberTex.Draw();
	areaTex.Draw();
	flameTex.Draw();
}

void AnnounceStageUI::AnnounceStage(int STAGE_NUM)
{
	stageNum = STAGE_NUM;
	startFlag = true;
}
