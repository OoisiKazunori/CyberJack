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
		lightRender[i]->data.pipelineName = PIPELINE_NAME_SPRITE_GOAL_EFFECT;
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
}

void GoalBoxEffect::Update()
{
	//êeéqä÷åW
	for (int i = 0; i < lightRender.size(); ++i)
	{
		lightRender[i]->data.motherMat = *motherPtr;
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
