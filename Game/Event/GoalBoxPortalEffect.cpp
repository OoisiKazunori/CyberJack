#include "GoalBoxPortalEffect.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

GoalBoxPortalEffect::GoalBoxPortalEffect()
{
	sprite = std::make_unique<Sprite3DRender>();
	sprite->data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "Test.png");
}

void GoalBoxPortalEffect::Init(const  KazMath::Vec3<float> &POS)
{
	startFlag = false;
	goBeforeFlag = false;
	sprite->data.transform.pos = POS;
	sprite->data.transform.scale = { 0.18f,0.18f,0.0f };
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
			sprite->data.transform.pos.z -= 1.0f;
		}
		if (sprite->data.transform.pos.z <= 12.2f)
		{
			sprite->data.transform.pos.z = 12.2f;
		}


		lerpRota.y = 360.0f * 5.0f;
		{
			KazMath::Vec3<float> distance = lerpRota - sprite->data.transform.rotation;
			sprite->data.transform.rotation += distance * 0.1f;
		}
		++timer;
	}

	//‘JˆÚ’²®—p
	//ImGui::Begin("Sprite");
	//ImGui::InputFloat("PosZ", &sprite->data.transform.pos.m128_f32[2]);
	//ImGui::InputFloat("ScaleX", &sprite->data.transform.scale.m128_f32[0]);
	//ImGui::InputFloat("ScaleY", &sprite->data.transform.scale.m128_f32[1]);
	//ImGui::End();

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
