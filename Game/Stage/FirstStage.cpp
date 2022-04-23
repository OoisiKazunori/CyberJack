#include "FirstStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"

FirstStage::FirstStage()
{
	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		//stageDebugBox[i]->data.pipelineName = PIPELINE_NAME_FOG_COLOR;
	}
}

void FirstStage::Update()
{
	ImGui::Begin("Stage");
	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		string name = "Box" + std::to_string(i);
		if (ImGui::TreeNode(name.c_str()))
		{
			ImGui::InputFloat("X", &stageDebugBox[i].data.transform.pos.m128_f32[0]);
			ImGui::InputFloat("Y", &stageDebugBox[i].data.transform.pos.m128_f32[1]);
			ImGui::InputFloat("Z", &stageDebugBox[i].data.transform.pos.m128_f32[2]);
			//ImGui::InputFloat("Fog", );
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void FirstStage::Draw()
{
	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		stageDebugBox[i].Draw();
	}
}
