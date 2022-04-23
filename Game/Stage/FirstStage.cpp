#include "FirstStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"

FirstStage::FirstStage()
{
	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		stageDebugBox[i].data.pipelineName = PIPELINE_NAME_FOG_COLOR;
		constHandle[i] = stageDebugBox[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);

		fogData[i].fogdata = { 0.0f,0.0f,0.0f,0.0f };
	}

	fogData[0].fogdata.x = 255.0f;
	fogData[0].fogdata.y = 0.0f;
	fogData[0].fogdata.z = 0.0f;

	stageDebugBox[0].data.transform.pos = { 0.0f,-25.0f,0.0f };
	stageDebugBox[0].data.transform.scale = { 60.0f,0.0f,2000.0f };
	stageDebugBox[0].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[1].data.transform.pos = { 140.0f,100.0f,0.0f };
	stageDebugBox[1].data.transform.scale = { 20.0f,30.0f,2000.0f };
	stageDebugBox[1].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[2].data.transform.pos = { -140.0f,100.0f,0.0f };
	stageDebugBox[2].data.transform.scale = { 20.0f,30.0f,2000.0f };
	stageDebugBox[2].data.transform.rotation = { 0.0f,0.0f,0.0f };



	stageDebugBox[2].data.transform.pos = { -140.0f,-100.0f,400.0f };
	stageDebugBox[2].data.transform.scale = { 10.0f,200.0f,10.0f };
	stageDebugBox[2].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[3].data.transform.pos = { 140.0f,-100.0f,400.0f };
	stageDebugBox[3].data.transform.scale = { 10.0f,200.0f,10.0f };
	stageDebugBox[3].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[4].data.transform.pos = { 140.0f,-100.0f,200.0f };
	stageDebugBox[4].data.transform.scale = { 10.0f,200.0f,10.0f };
	stageDebugBox[4].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[5].data.transform.pos = { -140.0f,-100.0f,200.0f };
	stageDebugBox[5].data.transform.scale = { 10.0f,200.0f,10.0f };
	stageDebugBox[5].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[6].data.transform.pos = { 140.0f,-100.0f,0.0f };
	stageDebugBox[6].data.transform.scale = { 10.0f,200.0f,10.0f };
	stageDebugBox[6].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[7].data.transform.pos = { -140.0f,-100.0f,0.0f };
	stageDebugBox[7].data.transform.scale = { 10.0f,200.0f,10.0f };
	stageDebugBox[7].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[8].data.transform.pos = { 140.0f,-100.0f,0.0f };
	stageDebugBox[8].data.transform.scale = { 10.0f,200.0f,10.0f };
	stageDebugBox[8].data.transform.rotation = { 0.0f,0.0f,0.0f };

	stageDebugBox[9].data.transform.pos = { -140.0f,100.0f,0.0f };
	stageDebugBox[9].data.transform.scale = { 20.0f,30.0f,2000.0f };
	stageDebugBox[9].data.transform.rotation = { 0.0f,0.0f,0.0f };
}

void FirstStage::Update()
{
	ImGui::Begin("Stage");
	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		string name = "Box" + std::to_string(i);
		if (ImGui::TreeNode(name.c_str()))
		{
			ImGui::InputFloat("PosX", &stageDebugBox[i].data.transform.pos.m128_f32[0]);
			ImGui::InputFloat("PosY", &stageDebugBox[i].data.transform.pos.m128_f32[1]);
			ImGui::InputFloat("PosZ", &stageDebugBox[i].data.transform.pos.m128_f32[2]);
			ImGui::InputFloat("ScaleX", &stageDebugBox[i].data.transform.scale.m128_f32[0]);
			ImGui::InputFloat("ScaleY", &stageDebugBox[i].data.transform.scale.m128_f32[1]);
			ImGui::InputFloat("ScaleZ", &stageDebugBox[i].data.transform.scale.m128_f32[2]);
			ImGui::InputFloat("RotaX", &stageDebugBox[i].data.transform.rotation.m128_f32[0]);
			ImGui::InputFloat("RotaY", &stageDebugBox[i].data.transform.rotation.m128_f32[1]);
			ImGui::InputFloat("RotaZ", &stageDebugBox[i].data.transform.rotation.m128_f32[2]);
			ImGui::InputFloat("FogDistiny", &fogData[i].fogdata.w);
			ImGui::TreePop();
		}
	}
	ImGui::InputFloat("FogColorX", &fogData[0].fogdata.x);
	ImGui::InputFloat("FogColorY", &fogData[0].fogdata.y);
	ImGui::InputFloat("FogColorZ", &fogData[0].fogdata.z);
	ImGui::End();


	for (int i = 1; i < stageDebugBox.size(); ++i)
	{
		fogData[i].fogdata.x = fogData[0].fogdata.x;
		fogData[i].fogdata.y = fogData[0].fogdata.y;
		fogData[i].fogdata.z = fogData[0].fogdata.z;
	}

	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		stageDebugBox[i].TransData(&fogData[i], constHandle[i], typeid(fogData[i]).name());
	}
}

void FirstStage::Draw()
{
	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		stageDebugBox[i].Draw();
	}
}
