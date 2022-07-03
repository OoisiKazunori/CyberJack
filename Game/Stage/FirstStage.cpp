#include "FirstStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary//Helper/KazHelper.h"

FirstStage::FirstStage()
{
	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		stageDebugBox[i].data.pipelineName = PIPELINE_NAME_FOG_COLOR_MULTITEX;
		constHandle[i] = stageDebugBox[i].CreateConstBuffer(sizeof(FogD), typeid(FogD).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		stageDebugBox[i].data.color = { 48,20,57,255 };
	}
	stageParamLoader.LoadFile(KazFilePathName::StageParamPath + "StageParamData.json");
	if (false)
	{
		std::array<std::vector<char>, 30> data;
		for (int i = 0; i < stageDebugBox.size(); ++i)
		{
			data[i] = KazHelper::CovertStringToChar("Box" + std::to_string(i));
		}

		if (false)
		{
			for (int i = 0; i < stageDebugBox.size(); ++i)
			{
				DirectX::XMVECTOR pos = stageDebugBox[i].data.transform.pos.ConvertXMVECTOR();
				DirectX::XMVECTOR scale = stageDebugBox[i].data.transform.scale.ConvertXMVECTOR();
				DirectX::XMVECTOR rota = stageDebugBox[i].data.transform.rotation.ConvertXMVECTOR();

				//Box毎のメンバ変数を追加
				rapidjson::Value posArray(rapidjson::kArrayType);
				rapidjson::Value scaleArray(rapidjson::kArrayType);
				rapidjson::Value rotaArray(rapidjson::kArrayType);
				for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
				{
					posArray.PushBack(rapidjson::Value(pos.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
					scaleArray.PushBack(rapidjson::Value(scale.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
					rotaArray.PushBack(rapidjson::Value(rota.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
				}

				//Boxオブジェクトにデータを追加
				rapidjson::Value object(rapidjson::kObjectType);
				object.AddMember("Pos", posArray, stageParamLoader.doc.GetAllocator());
				object.AddMember("Scale", scaleArray, stageParamLoader.doc.GetAllocator());
				object.AddMember("Rota", rotaArray, stageParamLoader.doc.GetAllocator());
				object.AddMember("Fog", 0.0f, stageParamLoader.doc.GetAllocator());

				stageParamLoader.doc.AddMember(rapidjson::GenericStringRef<char>(data[i].data()), object, stageParamLoader.doc.GetAllocator());
			}

			rapidjson::Value fogColor(rapidjson::kArrayType);
			//fogColor.PushBack(rapidjson::Value(fogData[0].fogdata.x), stageParamLoader.doc.GetAllocator());
			//fogColor.PushBack(rapidjson::Value(fogData[0].fogdata.y), stageParamLoader.doc.GetAllocator());
			//fogColor.PushBack(rapidjson::Value(fogData[0].fogdata.z), stageParamLoader.doc.GetAllocator());
			stageParamLoader.doc.AddMember("FogColor", fogColor, stageParamLoader.doc.GetAllocator());
			stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "StageParamData.json");
		}
	}

	//fogData[0].fogdata.x = BG_COLOR.x / 255.0f;
	//fogData[0].fogdata.y = BG_COLOR.y / 255.0f;
	//fogData[0].fogdata.z = BG_COLOR.z / 255.0f;

	{
		std::array<SpriteVertex, 4>vert;
		DirectX::XMVECTOR adjPos = { 50.0f,0.0f,0.0f };
		vert[0].pos = { -768.0f,407.0f,0.0f };
		vert[0].uv = { 0.0f,0.0f };
		vert[1].pos = { 768.0f,407.0f,0.0f };
		vert[1].uv = { 1.0f,0.0f };
		vert[2].pos = { -768.0f,-407.0f,0.0f };
		vert[2].uv = { 0.0f,1.0f };
		vert[3].pos = { 768.0f,-407.0f,0.0f };
		vert[3].uv = { 1.0f,1.0f };

		GradationData gradationData;
		gradationData.firstColor = DirectX::XMFLOAT4(0.93f, 0.65f, 0.53f, 1.0f);
		gradationData.endColor = DirectX::XMFLOAT4(0.24f, 0.09f, 0.62f, 1.0f);
		for (int i = 0; i < polygon.size(); ++i)
		{
			polygon[i] = std::make_unique<PolygonRender>(vert);
			RESOURCE_HANDLE handle = polygon[i]->CreateConstBuffer(sizeof(GradationData), typeid(GradationData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
			polygon[i]->TransData(&gradationData, handle, typeid(gradationData).name());
			polygon[i]->data.pipelineName = PIPELINE_NAME_SPRITE_GRADATION;
		}

		polygon[0]->data.transform.pos.z = 650.0f;
		polygon[1]->data.transform.pos.x = 550.0f;
		polygon[1]->data.transform.rotation.y = 100.0f;
		polygon[2]->data.transform.pos.x = -550.0f;
		polygon[2]->data.transform.rotation.y = 100.0f;
		polygon[3]->data.transform.pos.z = -650.0f;
	}

	{
		std::array<SpriteVertex, 4>vert;
		DirectX::XMVECTOR adjPos = { 50.0f,0.0f,0.0f };
		vert[0].pos = DirectX::XMFLOAT3(-800.0f, 0.0f, 700.0f);
		vert[0].uv = { 0.0f,0.0f };
		vert[1].pos = DirectX::XMFLOAT3(800.0f, 0.0f, 700.0f);
		vert[1].uv = { 1.0f,0.0f };
		vert[2].pos = DirectX::XMFLOAT3(-800.0f, 0.0f, -700.0f);
		vert[2].uv = { 0.0f,1.0f };
		vert[3].pos = DirectX::XMFLOAT3(800.0f, 0.0f, -700.0f);
		vert[3].uv = { 1.0f,1.0f };
		topPolygon = std::make_unique<PolygonRender>(vert);

		RESOURCE_HANDLE handle = topPolygon->CreateConstBuffer(sizeof(GradationData), typeid(GradationData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		topPolygon->data.pipelineName = PIPELINE_NAME_SPRITE_GRADATION;

		GradationData gradData;
		gradData.endColor = DirectX::XMFLOAT4(0.24f, 0.09f, 0.62f, 1.0f);
		gradData.firstColor = DirectX::XMFLOAT4(0.24f, 0.09f, 0.62f, 1.0f);
		topPolygon->TransData(&gradData, handle, typeid(gradData).name());

		topPolygon->data.transform.pos.y = 400.0f;
	}


	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		std::string name = "Box" + std::to_string(i);
		for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
		{
			switch (axisIndex)
			{
			case 0:
				stageDebugBox[i].data.transform.pos.x = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
				stageDebugBox[i].data.transform.scale.x = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
				stageDebugBox[i].data.transform.rotation.x = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();
				break;
			case 1:
				stageDebugBox[i].data.transform.pos.y = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
				stageDebugBox[i].data.transform.scale.y = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
				stageDebugBox[i].data.transform.rotation.y = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();
				break;
			case 2:
				stageDebugBox[i].data.transform.pos.z = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
				stageDebugBox[i].data.transform.scale.z = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
				stageDebugBox[i].data.transform.rotation.z = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();

				break;
			default:
				break;
			}


			stageYPos[i] = stageDebugBox[i].data.transform.pos;
		}
	}

	t = 0.0f;

}

void FirstStage::Update()
{
#pragma region ImGui
	bool exportFlag = false;
	bool importFlag = false;

	//ImGui::Begin("Stage");
	//for (int i = 0; i < stageDebugBox.size(); ++i)
	//{
	//	string name = "Box" + std::to_string(i);
	//	if (ImGui::TreeNode(name.c_str()))
	//	{
	//		ImGui::InputFloat("PosX", &stageDebugBox[i].data.transform.pos.x);
	//		ImGui::InputFloat("PosY", &stageDebugBox[i].data.transform.pos.y);
	//		ImGui::InputFloat("PosZ", &stageDebugBox[i].data.transform.pos.z);
	//		ImGui::InputFloat("ScaleX", &stageDebugBox[i].data.transform.scale.x);
	//		ImGui::InputFloat("ScaleY", &stageDebugBox[i].data.transform.scale.y);
	//		ImGui::InputFloat("ScaleZ", &stageDebugBox[i].data.transform.scale.z);
	//		ImGui::InputFloat("RotaX", &stageDebugBox[i].data.transform.rotation.x);
	//		ImGui::InputFloat("RotaY", &stageDebugBox[i].data.transform.rotation.y);
	//		ImGui::InputFloat("RotaZ", &stageDebugBox[i].data.transform.rotation.z);
	//		//ImGui::InputFloat("FogDistiny", &fogData[i].fogdata.w);
	//		ImGui::TreePop();
	//	}
	//}
	////ImGui::InputFloat("FogColorX", &fogData[0].fogdata.x);
	////ImGui::InputFloat("FogColorY", &fogData[0].fogdata.y);
	////ImGui::InputFloat("FogColorZ", &fogData[0].fogdata.z);
	//if (ImGui::Button("Import"))
	//{
	//	importFlag = true;
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Export"))
	//{
	//	exportFlag = true;
	//}
	//ImGui::End();

	for (int i = 1; i < stageDebugBox.size(); ++i)
	{
		//fogData[i].fogdata.x = fogData[0].fogdata.x;
		//fogData[i].fogdata.y = fogData[0].fogdata.y;
		//fogData[i].fogdata.z = fogData[0].fogdata.z;
	}

	//ファイル読み込み
	if (importFlag)
	{
		for (int i = 0; i < stageDebugBox.size(); ++i)
		{
			std::string name = "Box" + std::to_string(i);
			for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
			{
				switch (axisIndex)
				{
				case 0:
					stageDebugBox[i].data.transform.pos.x = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
					stageDebugBox[i].data.transform.scale.x = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
					stageDebugBox[i].data.transform.rotation.x = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();
					break;
				case 1:
					stageDebugBox[i].data.transform.pos.y = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
					stageDebugBox[i].data.transform.scale.y = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
					stageDebugBox[i].data.transform.rotation.y = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();
					break;
				case 2:
					stageDebugBox[i].data.transform.pos.z = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
					stageDebugBox[i].data.transform.scale.z = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
					stageDebugBox[i].data.transform.rotation.z = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();

					break;
				default:
					break;
				}
			}
		}
	}

	//ファイル書き込み
	if (exportFlag)
	{
		for (int i = 0; i < stageDebugBox.size(); ++i)
		{
			std::string name = "Box" + std::to_string(i);
			for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
			{
				switch (axisIndex)
				{
				case 0:
					stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].SetFloat(stageDebugBox[i].data.transform.pos.x);
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(stageDebugBox[i].data.transform.scale.y);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(stageDebugBox[i].data.transform.rotation.z);
					break;
				case 1:
					stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].SetFloat(stageDebugBox[i].data.transform.pos.x);
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(stageDebugBox[i].data.transform.scale.y);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(stageDebugBox[i].data.transform.rotation.z);
					break;
				case 2:
					stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].SetFloat(stageDebugBox[i].data.transform.pos.x);
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(stageDebugBox[i].data.transform.scale.y);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(stageDebugBox[i].data.transform.rotation.z);

					break;
				default:
					break;
				}
			}
			//stageParamLoader.doc[name.c_str()]["Fog"].SetFloat(fogData[i].fogdata.w);
		}


		for (int i = 0; i < stageDebugBox.size(); ++i)
		{
			//stageParamLoader.doc["FogColor"][0].SetFloat(fogData[i].fogdata.x);
			//stageParamLoader.doc["FogColor"][1].SetFloat(fogData[i].fogdata.y);
			//stageParamLoader.doc["FogColor"][2].SetFloat(fogData[i].fogdata.z);
		}
		stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "StageParamData.json");
	}
#pragma endregion


	if (startFlag)
	{
		if (t <= 1.0f)
		{
			t += 1.0f / 60.0f;
		}
		if (1.0f <= t)
		{
			t = 1.0f;
		}
	}
	else
	{
		t = 0.0f;
	}

	float mul = 800;
	easeY = EasingMaker(Out, Cubic, t);
	for (int i = 1; i < stageDebugBox.size(); ++i)
	{
		stageDebugBox[i].data.transform.pos.y = stageYPos[i].y + (-mul + easeY * mul);
	}


	const KazMath::Vec3<float> vel = { 0.0f,0.0f,-1.0f };
	//手前柱
	for (int i = 5; i < 16; ++i)
	{
		stageDebugBox[i].data.transform.pos += vel;
		if (stageDebugBox[i].data.transform.pos.z <= -200.0f)
		{
			stageDebugBox[i].data.transform.pos.z = 800.0f;
		}
	}
	for (int i = 16; i < 24; ++i)
	{
		stageDebugBox[i].data.transform.pos += vel;
		if (stageDebugBox[i].data.transform.pos.z <= -100.0f)
		{
			stageDebugBox[i].data.transform.pos.z = 800.0f;
		}
	}
	//手前柱

	//ImGui::Begin("Depth");
	//ImGui::SliderFloat("StartDepthFogX", &depthX.x, 0.0f, 500.0f);
	//ImGui::SliderFloat("EndDepthFogY", &depthX.y, 0.0f, 500.0f);
	//ImGui::End();

	for (int i = 0; i < fogData.size(); ++i)
	{
		fogData[i].rateAndFogLine.z = 100.0f;
		fogData[i].rateAndFogLine.w = 300.0f;

		fogData[i].rateAndFogLine.x = -401.0f;
		fogData[i].rateAndFogLine.y = 407.0f;

		fogData[i].depthX = depthX;

		fogData[i].depthX.z = 0;
		if (i == 2 || i == 4)
		{
			fogData[i].depthX.z = 1;
		}
		if (16 <= i && i <= 24)
		{
			fogData[i].depthX.z = 1;
		}



		DirectX::XMFLOAT3 first(0.93f, 0.65f, 0.53f);
		DirectX::XMFLOAT3 end(0.24f, 0.09f, 0.62f);
		DirectX::XMFLOAT3 result;
		result.x = first.x - end.x;
		result.y = first.y - end.y;
		result.z = first.z - end.z;
		fogData[i].endColor = end;
		fogData[i].subValue = result;
	}


	for (int i = 0; i < stageDebugBox.size(); ++i)
	{
		stageDebugBox[i].data.cameraIndex = cameraIndex;
		stageDebugBox[i].TransData(&fogData[i], constHandle[i], typeid(fogData[i]).name());
	}

}

void FirstStage::Draw()
{
	for (int i = 0; i < 24; ++i)
	{
		stageDebugBox[i].Draw();
	}

	for (int i = 0; i < 4; ++i)
	{
		polygon[i]->Draw();
	}
	topPolygon->Draw();
}
