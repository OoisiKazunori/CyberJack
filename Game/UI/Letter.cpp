#include "Letter.h"
#include<assert.h>
#include"../KazLibrary/Helper/ResourceFilePass.h"

Letter::Letter()
{
	//graphHandle[CHARA_LARGE] = TextureResourceMgr::Instance()->LoadDivGraph("");
	//graphHandle[CHARA_SMALL] = TextureResourceMgr::Instance()->LoadDivGraph("");
	//graphHandle[CHARA_NUM] = TextureResourceMgr::Instance()->LoadDivGraph("");
	inputLogHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::UIPath + "inputLog.png");
	initFlag = false;
}

void Letter::Init(const KazMath::Vec2<float> &POS, const char &CHARACTER, float FONT_SIZE)
{
	basePos = POS;
	baseSize = { FONT_SIZE ,FONT_SIZE };

	const float L_ADD_SCALE = 2.5f;
	render.data.transform.pos = basePos + KazMath::Vec2<float>(25.0f, 25.0f);
	render.data.transform.scale = baseSize + L_ADD_SCALE;
	render.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::UIPath + "FontTest.png");
	//render.data.handleData = graphHandle[fontType];

	inputRender.data.transform.pos = basePos;
	inputRender.data.handleData = inputLogHandle;


	spaceFlag = false;

	//文字を
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
	else if (CHARACTER == 32)
	{
		spaceFlag = true;
	}
	else
	{
		//使えない文字が入力されました
		assert(0);
	}
	//render.data.animationHandle = fontNum;

	changeHandleFlag = false;
	initFlag = true;

	timer = 0;
	prevStringIndex = 0;
}

void Letter::Finalize()
{
	initFlag = false;
}

void Letter::Update(float Y_POS, int INDEX)
{
	if (!initFlag)return;

	//文のインデックスが変わったら文字の座標を変える
	if (INDEX != prevStringIndex)
	{
		/*
		基本座標と補間座標の差分を出す
		↓
		基本座標と補間座標を変更後の同じ場所に
		↓
		差分を補間座標に足す
		*/
		KazMath::Vec2<float>sub = basePos - render.data.transform.pos;
		basePos.y = Y_POS;
		render.data.transform.pos = basePos + sub;

		prevStringIndex = INDEX;
	}

	//補間をかける
	KazMath::Larp(basePos, &render.data.transform.pos, 0.3f);
	KazMath::Larp(baseSize, &render.data.transform.scale, 0.3f);

	++timer;
	if (5 <= timer)
	{
		changeHandleFlag = true;
	}
}

void Letter::Draw()
{
	if (!initFlag)return;

	if (!changeHandleFlag)
	{
		inputRender.Draw();
	}
	else if (!spaceFlag)
	{
		render.Draw();
	}
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

void String::Init(const KazMath::Vec2<float>POS, const std::string &STRING, float FONT_SIZE)
{
	logString = STRING;
	fontSize = FONT_SIZE;
	timer = 0;
	charaArrayNum = 0;

	basePos = POS;
	KazMath::Vec2<float>lPos = { basePos.x + static_cast<float>(charaArrayNum) * (16.0f * fontSize), basePos.y };
	letters[0].Init(lPos, logString[charaArrayNum], fontSize);
}

void String::Finalize()
{
	for (int i = 0; i < letters.size(); ++i)
	{
		letters[i].Finalize();
	}
}

void String::Update(int STRING_INDEX)
{
	//時間経過で文字を一文字ずつ出す処理ーーー
	++timer;
	if (4 <= timer && charaArrayNum < logString.size() - 1 && logString.size() != 0 && charaArrayNum < letters.size())
	{
		timer = 0;
		++charaArrayNum;
		KazMath::Vec2<float>lPos = { basePos.x + static_cast<float>(charaArrayNum) * (16.0f * fontSize), basePos.y + 20.0f * static_cast<float>(STRING_INDEX) };
		letters[charaArrayNum].Init(lPos, logString[charaArrayNum], fontSize);
	}
	//時間経過で文字を一文字ずつ出す処理ーーー


	for (int i = 0; i < letters.size(); ++i)
	{
		letters[i].Update(basePos.y + 20.0f * static_cast<float>(STRING_INDEX), STRING_INDEX);
	}
}

void String::Draw()
{
	for (int i = 0; i < letters.size(); ++i)
	{
		letters[i].Draw();
	}
}
