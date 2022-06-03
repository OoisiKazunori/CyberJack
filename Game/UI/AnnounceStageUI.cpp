#include "AnnounceStageUI.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/KazHelper.h"

AnnounceStageUI::AnnounceStageUI()
{
	flameTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "Flame.png");
	areaTex[0].data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "A.png");
	areaTex[1].data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "R.png");
	areaTex[2].data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "E.png");
	areaTex[3].data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "A.png");


	numberHandle[0] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum0.png");
	numberHandle[1] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum1.png");
	numberHandle[2] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum2.png");
	numberHandle[3] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum3.png");
	numberHandle[4] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum4.png");
	numberHandle[5] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum5.png");
	numberHandle[6] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum6.png");
	numberHandle[7] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum7.png");
	numberHandle[8] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum8.png");
	numberHandle[9] = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "CursorNum9.png");

	basePos = { 640.0f,150.0f };
}

void AnnounceStageUI::Init()
{
	flameTex.data.transform.scale = { 0.0f,0.1f };
	larpScale = { 0.0f,0.1f };
	startFlag = false;
	endFlag = false;
	timer;
	areaNum = 0;

	for (int i = 0; i < areaTex.size(); ++i)
	{
		float sub = 30.0f;
		float size = 32.0f;
		areaTex[i].data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;
		areaTex[i].data.transform.scale = { 2.0f,2.0f };
		areaTex[i].data.transform.pos = { 590.0f - sub - 75.0f + i * (size + sub),153.0f };
		areaTex[i].data.alpha = 255.0f;
	}
}

void AnnounceStageUI::Update()
{
	if (startFlag)
	{
		flameTex.data.transform.pos = basePos;
		larpScale.m128_f32[0] = 4.0f;

		float mul = 0.3f;
		//ウィンドウを横に広げる
		KazMath::Larp(larpScale.m128_f32[0], &flameTex.data.transform.scale.m128_f32[0], mul);


		if (3.9f <= flameTex.data.transform.scale.m128_f32[0])
		{
			flameTex.data.transform.scale.m128_f32[0] = 4.0f;
		}
		//ウィンドウを縦に広げる
		if (static_cast<int>(larpScale.m128_f32[0]) == static_cast<int>(flameTex.data.transform.scale.m128_f32[0]))
		{
			larpScale.m128_f32[1] = 1.0f;
		}
		KazMath::Larp(larpScale.m128_f32[1], &flameTex.data.transform.scale.m128_f32[1], mul);



		++timer;
		if (120 <= timer)
		{
			timer = 0;
			startFlag = false;
			endFlag = true;
		}

		//文字を一文字ずつ描画する
		if (timer % 10 == 0 && areaNum <= areaTex.size() - 1)
		{
			++areaNum;
		}
	}

	//エリア文字をαで消す
	if (110 <= timer && 1.0f <= areaTex[0].data.alpha)
	{
		for (int i = 0; i < areaTex.size(); ++i)
		{
			areaTex[i].data.alpha = 255.0f / 30.0f;
		}
	}


	//しばらく描画したら登場演出と逆の事をやる
	if (endFlag)
	{
		flameTex.data.transform.pos = basePos;
		larpScale.m128_f32[1] = 0.1f;

		float mul = 0.3f;
		//ウィンドウの縦を狭める
		KazMath::Larp(larpScale.m128_f32[1], &flameTex.data.transform.scale.m128_f32[1], mul);


		if (flameTex.data.transform.scale.m128_f32[1] <= 0.2f)
		{
			//flameTex.data.transform.scale.m128_f32[1] = 0.1f;
			larpScale.m128_f32[0] = 0.0f;
		}
		//ウィンドウの横を無くす
		if (flameTex.data.transform.scale.m128_f32[0] <= 0.0f)
		{
			larpScale.m128_f32[1] = 0.0f;
			endFlag = false;
		}
		KazMath::Larp(larpScale.m128_f32[0], &flameTex.data.transform.scale.m128_f32[0], mul);
	}



	//ImGui::Begin("Layer");
	//ImGui::InputFloat("PosX", &areaTex.data.transform.pos.m128_f32[0]);
	//ImGui::InputFloat("PosY", &areaTex.data.transform.pos.m128_f32[1]);
	//ImGui::InputFloat("PosZ", &areaTex.data.transform.pos.m128_f32[2]);
	//ImGui::InputFloat("ScaleX", &areaTex.data.transform.scale.m128_f32[0]);
	//ImGui::InputFloat("ScaleY", &areaTex.data.transform.scale.m128_f32[1]);
	//ImGui::InputFloat("ScaleZ", &areaTex.data.transform.scale.m128_f32[2]);
	//ImGui::End();

}

void AnnounceStageUI::Draw()
{
	for (int i = 0; i < areaNum; ++i)
	{
		areaTex[i].Draw();
	}
	flameTex.Draw();
}

void AnnounceStageUI::AnnounceStage(int STAGE_NUM)
{
	stageNum = STAGE_NUM;
	startFlag = true;

	std::vector<int>num = KazHelper::CountNumber(stageNum, 2);

	float scale = 1.45f;
	areaTex[4].data.handle = numberHandle[num[0]];
	areaTex[4].data.transform.scale = { scale,scale };
	areaTex[5].data.handle = numberHandle[num[1]];
	areaTex[5].data.transform.scale = { scale,scale };
}
