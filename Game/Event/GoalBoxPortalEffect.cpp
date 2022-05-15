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
	sprite->data.transform.scale = { 0.5f,0.5f };
	timer = 0;
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

		++timer;
	}
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
