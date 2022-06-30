#include "GoalBoxPortalEffect.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

GoalBoxPortalEffect::GoalBoxPortalEffect()
{
	sprite = std::make_unique<Sprite3DRender>();
	sprite->data.handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "Test.png");
}

void GoalBoxPortalEffect::Init(const KazMath::Vec3<float> &POS)
{
	startFlag = false;
	sprite->data.transform.pos = POS;
	sprite->data.transform.scale = { 0.0f,0.18f,0.0f };
	lerpScale = sprite->data.transform.scale;
	sprite->data.transform.rotation = { 0.0f,0.0f,0.0f };
	timer = 0;
}

void GoalBoxPortalEffect::Update()
{
	if (startFlag)
	{
		sprite->data.transform.pos.z -= 0.5f;
		if (sprite->data.transform.pos.z <= 12.2f)
		{
			sprite->data.transform.pos.z = 12.2f;
		}


		lerpScale.x = 0.18f;
		{
			KazMath::Vec3<float> distance = lerpScale - sprite->data.transform.scale;
			sprite->data.transform.scale += distance * 0.1f;
		}
		++timer;
	}

	//�J�ڒ����p
	//ImGui::Begin("Sprite");
	//ImGui::InputFloat("PosZ", &sprite->data.transform.pos.z);
	//ImGui::InputFloat("ScaleX", &sprite->data.transform.scale.x);
	//ImGui::InputFloat("ScaleY", &sprite->data.transform.scale.y);
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
