#include "RocketEffect.h"
#include "../KazLibrary/Helper/ResourceFilePass.h"

RocketEffect::RocketEffect()
{
	fireTexPos = { 0.0f,0.0f,45.0f };
	bloomTexPos = { 0.0f,0.0f,20.0f };

	fireEffectRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::EffectFirePath + "Fire.png");
	fireEffectRender.data.billBoardFlag = false;

	fireTexPos = { 0.0f,0.0f,45.0f };
	fireEffectRender.data.transform.scale = { 0.5f,1.5f,1.0f };
	fireEffectRender.data.transform.rotation = { 0.0f,90.0f,90.0f };

	bloomTexPos = { 0.0f,0.0f,20.0f };
	lightEffectRender.data.change3DFlag = true;
	lightEffectRender.data.billBoardFlag = true;
	lightEffectRender.data.radius = 10.0f;
	initFlag = false;
}

void RocketEffect::Init(KazMath::Vec3<float> *BASE_POS_PTR)
{
	pos = BASE_POS_PTR;
	flashTimer = 0;

	fireEffectRender.data.colorData.color.a = 0;
	lightEffectRender.data.colorData.color.a = 0;

	initFlag = true;
}

void RocketEffect::Update()
{
	if (initFlag)
	{
		if (fireEffectRender.data.colorData.color.a < 255)
		{
			fireEffectRender.data.colorData.color.a += 5;
			lightEffectRender.data.colorData.color.a += 5;
		}
		else
		{
			fireEffectRender.data.colorData.color.a = 255;
			lightEffectRender.data.colorData.color.a = 255;
		}


		if (3 <= flashTimer)
		{
			flashTimer = 0;
			lightEffectRender.data.radius = 8.0f;
		}
		else
		{
			lightEffectRender.data.radius = 10.0f;
			++flashTimer;
		}

		lightEffectRender.data.transform.pos = {};
		lightEffectRender.data.transform.pos = *pos + bloomTexPos;
		fireEffectRender.data.transform.pos = {};
		fireEffectRender.data.transform.pos = *pos + fireTexPos;
	}
}

void RocketEffect::Draw()
{
	if (initFlag)
	{
		lightEffectRender.Draw();
		fireEffectRender.Draw();
	}
}
