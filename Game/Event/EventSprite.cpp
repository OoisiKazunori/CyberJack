#include "EventSprite.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

EventSprite::EventSprite()
{
	hitBox.center = &spriteRender.data.transform.pos;
	hitBox.radius = 40.0f;

	selectHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TitlePath + "ChoseStart.png");
	hitHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TitlePath + "Start_Done.png");

}

void EventSprite::Init(const XMVECTOR &POS, int HANDLE)
{
	noHitHandle = HANDLE;
	spriteRender.data.handle = noHitHandle;
	spriteRender.data.transform.pos = POS;
	hitFlag = false;
	deadFlag = false;
	timer = 0;
	iOperationData.Init(1);

	selectingFlag = false;

	spriteRender.data.transform.scale = { 0.5f,0.5f };
	spriteRender.data.alpha = 255.0f;
}

void EventSprite::Update()
{
	if (selectingFlag)
	{
		spriteRender.data.handle = selectHandle;
	}
	else
	{
		spriteRender.data.handle = noHitHandle;
	}
	if (!IsAlive())
	{
		hitFlag = true;
		spriteRender.data.handle = hitHandle;
	}
	if (hitFlag)
	{
		++timer;
	}
	if (60 <= timer)
	{
		spriteRender.data.alpha -= 5.0f;
	}

	if (spriteRender.data.alpha <= 0.0f)
	{
		deadFlag = true;
	}

	spriteRender.data.transform.pos.m128_f32[1] = 10.0f;
	spriteRender.data.transform.pos.m128_f32[2] = 200.0f;
}

void EventSprite::Draw()
{
	if (!deadFlag)
	{
		spriteRender.Draw();
	}
}
