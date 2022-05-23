#include "GoalBoxPortalEffect.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

GoalBoxPortalEffect::GoalBoxPortalEffect()
{
	sprite = std::make_unique<Sprite3DRender>();
	sprite->data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "Test.png");
}

void GoalBoxPortalEffect::Init(const XMVECTOR &POS)
{
	startFlag = false;
	goBeforeFlag = false;
	sprite->data.transform.pos = POS;
	sprite->data.transform.scale = { 0.1f,0.1f };
	sprite->data.transform.rotation = { 0.0f,0.0f,0.0f };
	timer = 0;
	lerpRota = { 0.0f,0.0f,0.0f };
}

void GoalBoxPortalEffect::Update()
{
	if (startFlag)
	{
		if (120 <= timer)
		{
			goBeforeFlag = true;
		}
		if (goBeforeFlag)
		{
			sprite->data.transform.pos.m128_f32[2] -= 2.0f;
		}

		lerpRota.m128_f32[1] = 360.0f * 5.0f;
		{
			XMVECTOR distance = lerpRota - sprite->data.transform.rotation;
			sprite->data.transform.rotation += distance * 0.1f;
		}
		++timer;
	}


	sprite->data.transform.scale = { 0.18f,0.18f };
}

void GoalBoxPortalEffect::Draw()
{
	if (startFlag)
	{
		sprite->Draw();
	}
}

void GoalBoxPortalEffect::Start()
{
	startFlag = true;
}

bool GoalBoxPortalEffect::AllHidden()
{
	return 148 <= timer;
}
