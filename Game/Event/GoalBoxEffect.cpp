#include "GoalBoxEffect.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

GoalBoxEffect::GoalBoxEffect()
{
	int handle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::GoalPath + "GoalEffect.png");
	for (int i = 0; i < lightRender.size(); ++i)
	{
		lightRender[i] = std::make_unique<Sprite3DRender>(XMFLOAT2(0.0f, 0.0f));
		lightRender[i]->data.handle = handle;
		lightRender[i]->data.pipelineName = PIPELINE_NAME_SPRITE_GOAL_EFFECT_MULTITEX;
		uvHandle[i] = lightRender[i]->CreateConstBuffer(sizeof(GoalLightData), typeid(GoalLightData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	}


	lightRender[0]->data.transform.pos = { 0,-0.5,0 };
	lightRender[1]->data.transform.pos = { -0.5,0,0 };
	lightRender[2]->data.transform.pos = { 0,0.5,0 };
	lightRender[3]->data.transform.pos = { 0.5,0,0 };
	lightRender[4]->data.transform.pos = { 0.5,0,0, };
	lightRender[5]->data.transform.pos = { 0,-0.2,0, };
	lightRender[6]->data.transform.pos = { -0.5,0,0 };
	lightRender[7]->data.transform.pos = { 0,0,0 };

	lightRender[0]->data.transform.rotation = { 0,-40,90 };
	lightRender[1]->data.transform.rotation = { -70,0,0 };
	lightRender[2]->data.transform.rotation = { 0,40,270 };
	lightRender[3]->data.transform.rotation = { -50,0,180 };
	lightRender[4]->data.transform.rotation = { 0,180,10 };
	lightRender[5]->data.transform.rotation = { -10,0,130 };
	lightRender[6]->data.transform.rotation = { 10,40,50 };
	lightRender[7]->data.transform.rotation = { 0,40,230 };
}

void GoalBoxEffect::Init(XMMATRIX *ADRESS)
{
	motherPtr = ADRESS;
	uvData.light = { 0.0f,0.0f,0.0f,0.0f };
	drawHandle = 0;

	for (int i = 0; i < lightRender.size(); ++i)
	{
		lightRender[i]->data.alpha = 0.0f;
	}
}

void GoalBoxEffect::Update()
{
	//êeéqä÷åW
	for (int i = 0; i < lightRender.size(); ++i)
	{
		lightRender[i]->data.motherMat = *motherPtr;
	}


	uvData.light.y += 0.01f;
	for (int i = 0; i < lightRender.size(); ++i)
	{
		lightRender[i]->TransData(&uvData, uvHandle[i], typeid(GoalLightData).name());
	}
	//ìoèÍÇ≥ÇπÇÈ
	for (int i = 0; i < drawHandle; ++i)
	{
		if (lightRender[i]->data.alpha < 255.0f)
		{
			lightRender[i]->data.alpha += 5.0f;
		}
		if (255.0f <= lightRender[i]->data.alpha)
		{
			lightRender[i]->data.alpha = 255.0f;
		}
	}


	//ImGui::Begin("Effect");
	//for (int i = 0; i < lightRender.size(); ++i)
	//{
	//	{
	//		std::string name = "X" + std::to_string(i);
	//		ImGui::InputFloat(name.c_str(), &lightRender[i]->data.transform.pos.m128_f32[0]);
	//	}
	//	{
	//		std::string name = "Y" + std::to_string(i);
	//		ImGui::InputFloat(name.c_str(), &lightRender[i]->data.transform.pos.m128_f32[1]);
	//	}
	//	{
	//		std::string name = "Z" + std::to_string(i);
	//		ImGui::InputFloat(name.c_str(), &lightRender[i]->data.transform.pos.m128_f32[2]);
	//	}
	//}
	//ImGui::End();
}

void GoalBoxEffect::Draw()
{
	for (int i = 0; i < lightRender.size(); ++i)
	{
		lightRender[i]->Draw();
	}
}

void GoalBoxEffect::Appear()
{
	++drawHandle;

	if (8 <= drawHandle)
	{
		drawHandle = 8;
	}
}
