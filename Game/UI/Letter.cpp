#include "Letter.h"
#include<assert.h>
#include"../KazLibrary/Helper/ResourceFilePass.h"

Letter::Letter()
{
	//graphHandle[CHARA_LARGE] = TextureResourceMgr::Instance()->LoadDivGraph("");
	//graphHandle[CHARA_SMALL] = TextureResourceMgr::Instance()->LoadDivGraph("");
	//graphHandle[CHARA_NUM] = TextureResourceMgr::Instance()->LoadDivGraph("");
	initFlag = false;
}

void Letter::Init(const KazMath::Vec2<float> &POS, const char &CHARACTER, float FONT_SIZE)
{
	basePos = POS;
	baseSize = { FONT_SIZE ,FONT_SIZE };

	const float L_ADD_SCALE = 2.5f;
	render.data.transform.pos = basePos + KazMath::Vec2<float>(25.0f, 25.0f);
	render.data.transform.scale = baseSize + L_ADD_SCALE;



	//•¶Žš‚ð
	if (CheckFontType(CHARACTER, 65, 90))
	{
		fontType = CHARA_LARGE;
		fontNum = CHARACTER - 65;
	}
	else if (CheckFontType(CHARACTER, 97, 122))
	{
		fontType = CHARA_SMALL;
		fontNum = CHARACTER - 97;
	}
	else if (CheckFontType(CHARACTER, 49, 57))
	{
		fontType = CHARA_NUM;
		fontNum = CHARACTER - 49;
	}
	else
	{
		//Žg‚¦‚È‚¢•¶Žš‚ª“ü—Í‚³‚ê‚Ü‚µ‚½
		assert(0);
	}

	//render.data.handleData = graphHandle[fontType];
	//render.data.animationHandle = fontNum;
	render.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::UIPath + "FontTest.png");
	initFlag = true;
}

void Letter::Finalize()
{
	initFlag = false;
}

void Letter::Update()
{
	if (!initFlag)return;
	//•âŠÔ‚ð‚©‚¯‚é
	KazMath::Larp(basePos, &render.data.transform.pos, 0.3f);
	KazMath::Larp(baseSize, &render.data.transform.scale, 0.3f);
}

void Letter::Draw()
{
	if (!initFlag)return;
	render.Draw();
}

bool Letter::CheckFontType(const char &CHARACTER, int MIN_NUM, int MAX_NUM)
{
	for (int i = MIN_NUM; i < MAX_NUM; ++i)
	{
		if (i == CHARACTER)
		{
			return true;
		}
	}
	return false;
}


String::String()
{
}

void String::Init(const std::string &STRING, float FONT_SIZE)
{
	for (int i = 0; i < STRING.size() && i < letters.size(); ++i)
	{
		KazMath::Vec2<float> lPos(10.0f + static_cast<float>(i) * (16.0f * FONT_SIZE), 300.0f);
		letters[i].Init(lPos, STRING[i], FONT_SIZE);
	}
}

void String::Finalize()
{
	for (int i = 0; i < letters.size(); ++i)
	{
		letters[i].Finalize();
	}
}

void String::Update()
{
	for (int i = 0; i < letters.size(); ++i)
	{
		letters[i].Update();
	}
}

void String::Draw()
{
	for (int i = 0; i < letters.size(); ++i)
	{
		letters[i].Draw();
	}
}
