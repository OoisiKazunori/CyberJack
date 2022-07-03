#include"WindowFlame.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/KazHelper.h"

WindowFlame::WindowFlame()
{
	flameTex.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StageUIPath + "Flame.png");
	flameTex.data.transform.scale = { 0.0f,0.0f,0.0f };
}

void WindowFlame::Init(const KazMath::Transform3D &POS, const  KazMath::Vec2<float> &SCALE, bool APPEAR_WINDOW)
{
	KazMath::Transform3D init;
	init.pos = { 0.0f,0.0f,0.0f };
	init.scale = { 0.0f,0.1f,1.0f };
	init.rotation = { 0.0f,0.0f,0.0f };
	flameTex.data.transform = init;

	basePos = POS.pos;
	lerpMaxScale = SCALE;
	flameTex.data.transform.rotation = POS.rotation;

	if (APPEAR_WINDOW)
	{
		flameTex.data.transform.scale = { SCALE.x,SCALE.y,1.0f };
	}
	larpScale = { 0.0f,0.1f,1.0f };
	startFlag = false;
	endFlag = false;
}

void WindowFlame::Update()
{
	if (startFlag)
	{
		flameTex.data.transform.pos = basePos;
		larpScale.x = lerpMaxScale.x;

		float mul = 0.3f;
		//ウィンドウを横に広げる
		KazMath::Larp(larpScale.x, &flameTex.data.transform.scale.x, mul);


		if (lerpMaxScale.x - 0.1f <= flameTex.data.transform.scale.x)
		{
			flameTex.data.transform.scale.x = lerpMaxScale.x;
		}
		//ウィンドウを縦に広げる
		if (larpScale.x <= flameTex.data.transform.scale.x)
		{
			larpScale.y = lerpMaxScale.y;
		}
		KazMath::Larp(larpScale.y, &flameTex.data.transform.scale.y, mul);

		//縦に広がった
		if (lerpMaxScale.y - 0.1f <= flameTex.data.transform.scale.y)
		{
			startFlag = false;
		}
	}


	//しばらく描画したら登場演出と逆の事をやる
	if (endFlag)
	{
		flameTex.data.transform.pos = basePos;
		larpScale.y = 0.1f;

		float mul = 0.3f;
		//ウィンドウの縦を狭める
		KazMath::Larp(larpScale.y, &flameTex.data.transform.scale.y, mul);


		if (flameTex.data.transform.scale.y <= 0.11f)
		{
			//flameTex.data.transform.scale.y = 0.1f;
			larpScale.x = 0.0f;
		}
		//ウィンドウの横を無くす
		if (flameTex.data.transform.scale.x <= 0.0f)
		{
			larpScale.y = 0.0f;
			endFlag = false;
		}
		KazMath::Larp(larpScale.x, &flameTex.data.transform.scale.x, mul);
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
