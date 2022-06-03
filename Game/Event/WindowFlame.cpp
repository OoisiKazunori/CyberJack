#include"WindowFlame.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/KazHelper.h"

WindowFlame::WindowFlame()
{
	flameTex.data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "Flame.png");
	flameTex.data.transform.scale = { 0.0f,0.0f,0.0f };
}

void WindowFlame::Init(const KazMath::Transform3D &POS, const XMVECTOR &SCALE, bool APPEAR_WINDOW)
{
	KazMath::Transform3D init;
	init.pos = { 0.0f,0.0f,0.0f };
	init.scale = { 0.0f,0.1f };
	init.rotation = { 0.0f,0.0f,0.0f };
	flameTex.data.transform = init;

	basePos = POS.pos;
	lerpMaxScale = SCALE;
	flameTex.data.transform.rotation = POS.rotation;

	if (APPEAR_WINDOW)
	{
		flameTex.data.transform.scale = SCALE;
	}

	larpScale = { 0.0f,0.1f };
	startFlag = false;
	endFlag = false;
}

void WindowFlame::Update()
{
	if (startFlag)
	{
		flameTex.data.transform.pos = basePos;
		larpScale.m128_f32[0] = lerpMaxScale.m128_f32[0];

		float mul = 0.3f;
		//ウィンドウを横に広げる
		KazMath::Larp(larpScale.m128_f32[0], &flameTex.data.transform.scale.m128_f32[0], mul);


		if (lerpMaxScale.m128_f32[0] - 0.1f <= flameTex.data.transform.scale.m128_f32[0])
		{
			flameTex.data.transform.scale.m128_f32[0] = lerpMaxScale.m128_f32[0];;
		}
		//ウィンドウを縦に広げる
		if (larpScale.m128_f32[0] <= flameTex.data.transform.scale.m128_f32[0])
		{
			larpScale.m128_f32[1] = lerpMaxScale.m128_f32[1];
		}
		KazMath::Larp(larpScale.m128_f32[1], &flameTex.data.transform.scale.m128_f32[1], mul);

		//縦に広がった
		if (lerpMaxScale.m128_f32[1] - 0.1f <= flameTex.data.transform.scale.m128_f32[1])
		{
			startFlag = false;
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
}

void WindowFlame::Draw()
{
	flameTex.Draw();
}

void WindowFlame::Start()
{
	startFlag = true;
}

void WindowFlame::End()
{
	endFlag = true;
}
