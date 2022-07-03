#include "GoalBoxPortalEffect.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

GoalBoxPortalEffect::GoalBoxPortalEffect()
{
	sprite = std::make_unique<Sprite3DRender>();
	constBuffHandle = sprite->CreateConstBuffer(sizeof(float), typeid(float).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);

	sprite->data.pipelineName = PIPELINE_NAME_SPRITE_NOISE;
	sprite->data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "Test.png");
}

void GoalBoxPortalEffect::Init(const KazMath::Vec3<float> &POS)
{
	startFlag = false;
	sprite->data.transform.pos = POS;
	sprite->data.transform.scale = { 0.0f,0.18f,0.0f };
	lerpScale = sprite->data.transform.scale;
	sprite->data.transform.rotation = { 0.0f,0.0f,0.0f };
	timer = 0;
	appearNextStageFlag = false;
}

void GoalBoxPortalEffect::Update()
{
	if (startFlag)
	{
		if (sprite->data.transform.pos.z <= 12.2f)
		{
			sprite->data.transform.pos.z = 12.2f;
		}
		if (45 <= timer)
		{
			appearNextStageFlag = true;
		}

		float lSeed = static_cast<float>(timer);
		sprite->TransData(&lSeed, constBuffHandle, typeid(float).name());



		lerpScale.x = 0.18f;
		{
			KazMath::Vec3<float> distance = lerpScale - sprite->data.transform.scale;
			sprite->data.transform.scale += distance * 0.1f;
		}
		if (lerpScale.x <= sprite->data.transform.scale.x)
		{
			sprite->data.transform.scale.x = lerpScale.x;
		}
		if (appearNextStageFlag)
		{
			timer = -1;
		}
		else
		{
			++timer;
		}
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
	return sprite->data.transform.pos.z <= 12.2f;
}
