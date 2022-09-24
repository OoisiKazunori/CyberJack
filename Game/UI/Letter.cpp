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

	//������
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
		//�g���Ȃ����������͂���܂���
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

	//���̃C���f�b�N�X���ς�����當���̍��W��ς���
	if (INDEX != prevStringIndex)
	{
		/*
		��{���W�ƕ�ԍ��W�̍������o��
		��
		��{���W�ƕ�ԍ��W��ύX��̓����ꏊ��
		��
		�������ԍ��W�ɑ���
		*/
		KazMath::Vec2<float>sub = basePos - render.data.transform.pos;
		basePos.y = Y_POS;
		render.data.transform.pos = basePos + sub;

		prevStringIndex = INDEX;
	}

	//��Ԃ�������
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
	//���Ԍo�߂ŕ������ꕶ�����o�������[�[�[
	++timer;
	if (4 <= timer && charaArrayNum < logString.size() - 1 && logString.size() != 0 && charaArrayNum < letters.size())
	{
		timer = 0;
		++charaArrayNum;
		KazMath::Vec2<float>lPos = { basePos.x + static_cast<float>(charaArrayNum) * (16.0f * fontSize), basePos.y + 20.0f * static_cast<float>(STRING_INDEX) };
		letters[charaArrayNum].Init(lPos, logString[charaArrayNum], fontSize);
	}
	//���Ԍo�߂ŕ������ꕶ�����o�������[�[�[


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
