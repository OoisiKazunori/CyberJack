#include "RezStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary/Helper/KazHelper.h"

RezStage::RezStage()
{
#pragma region Grid
	const float L_SPACE = 300.0f;
	const float L_Y = -150.0f;
	const float L_X = -10000.0f;
	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		float index = static_cast<float>(i);
		gridFloorZLinePos[i][0].pos = { L_X,L_Y,-700.0f + index * L_SPACE };
		gridFloorZLinePos[i][1] = gridFloorZLinePos[i][0];
		gridFloorZLinePos[i][1].pos.x *= -1;
	}

	for (int i = 0; i < gridFloorXLinePos.size(); ++i)
	{
		float index = static_cast<float>(i);
		gridFloorXLinePos[i][0].pos = { L_X + index * L_SPACE,L_Y,-5000.0f };
		gridFloorXLinePos[i][1] = gridFloorXLinePos[i][0];
		gridFloorXLinePos[i][1].pos.z *= -1;
	}


	int lLineRIndex = 0;
	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		gridLineRender[lLineRIndex].data.startPos = gridFloorZLinePos[i][0].pos;
		gridLineRender[lLineRIndex].data.endPos = gridFloorZLinePos[i][1].pos;
		++lLineRIndex;
	}
	for (int i = 0; i < gridFloorXLinePos.size(); ++i)
	{
		gridLineRender[lLineRIndex].data.startPos = gridFloorXLinePos[i][0].pos;
		gridLineRender[lLineRIndex].data.endPos = gridFloorXLinePos[i][1].pos;
		++lLineRIndex;
	}

	for (int i = 0; i < gridLineRender.size(); ++i)
	{
		gridLineRender[i].data.color = { 255,255,255,255 };
		gridLineRender[i].data.pipelineName = PIPELINE_NAME_FOG_LINE;

		FogData lData;
		RESOURCE_HANDLE lHandle = gridLineRender[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		KazMath::Color lColor(29, 19, 72, 255);
		DirectX::XMFLOAT3 colorRate = lColor.ConvertColorRateToXMFLOAT3();
		lData.fogdata = { colorRate.x,colorRate.y,colorRate.z,0.0006f };
		gridLineRender[i].TransData(&lData, lHandle, typeid(lData).name());
	}
#pragma endregion


	for (int i = 0; i < filePassChar.size(); ++i)
	{
		filePassChar[i][0] = 'n';
		filePassChar[i][1] = 'o';
		filePassChar[i][2] = '_';
		filePassChar[i][3] = 'p';
		filePassChar[i][4] = 'a';
		filePassChar[i][5] = 's';
		filePassChar[i][6] = 's';
		filePassChar[i][7] = '\0';
	}

	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		const float maxXPos = 4000.0f;
		const float minXPos = 500.0f;
		if (KazMath::Rand<int>(2, 0))
		{
			floorObjectRender[i].objRender[0].data.transform.pos.x = KazMath::Rand<float>(-maxXPos, -minXPos);
		}
		else
		{
			floorObjectRender[i].objRender[0].data.transform.pos.x = KazMath::Rand<float>(maxXPos, minXPos);
		}
		floorObjectRender[i].objRender[0].data.transform.pos.z = KazMath::Rand<float>(10000, -100);

		const float lScaleRate = abs(floorObjectRender[i].objRender[0].data.transform.pos.x) / abs(maxXPos);
		const float lScaleMin = 100.0f;
		const float lScaleMax = (lScaleRate * 200.0f) + lScaleMin;
		floorObjectRender[i].objRender[0].data.transform.scale =
		{
			KazMath::Rand<float>(lScaleMax,lScaleMin),
			KazMath::Rand<float>(lScaleMax,lScaleMin),
			KazMath::Rand<float>(lScaleMax,lScaleMin)
		};
		floorObjectRender[i].initScale = floorObjectRender[i].objRender[0].data.transform.scale;
		floorObjectRender[i].objRender[0].data.transform.pos.y = gridFloorZLinePos[0][0].pos.y + floorObjectRender[i].objRender[0].data.transform.scale.y;


		floorObjectRender[i].objRender[0].data.color = { 255,255,255,255 };
		floorObjectRender[i].objRender[0].data.pipelineName = PIPELINE_NAME_FOG_COLOR;

		floorObjectRender[i].objRender[1].data.transform.pos = floorObjectRender[i].objRender[0].data.transform.pos + KazMath::Vec3<float>(0.0f, -(floorObjectRender[i].objRender[0].data.transform.scale.y * 2), 0.0f);
		floorObjectRender[i].objRender[1].data.transform.scale = floorObjectRender[i].objRender[0].data.transform.scale;
		floorObjectRender[i].objRender[1].data.color = { 255,0,0,255 };
		floorObjectRender[i].objRender[1].data.pipelineName = PIPELINE_NAME_FOG_COLOR;

		RESOURCE_HANDLE lHandle = floorObjectRender[i].objRender[0].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		RESOURCE_HANDLE lHandle2 = floorObjectRender[i].objRender[1].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		FogData lData;

		KazMath::Color lColor(29, 19, 72, 255);
		DirectX::XMFLOAT3 colorRate = lColor.ConvertColorRateToXMFLOAT3();
		lData.fogdata = { colorRate.x,colorRate.y,colorRate.z,0.0006f };

		floorObjectRender[i].objRender[0].TransData(&lData, lHandle, typeid(lData).name());
		floorObjectRender[i].objRender[1].TransData(&lData, lHandle2, typeid(lData).name());
	}


	vaporWaveSunRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::RelativeResourcePath + "Stage/" + "vaporWaveSun.png");
	vaporWaveSunRender.data.transform.pos = { 0.0f,2000.0f,5000.0f };
	vaporWaveSunRender.data.transform.scale = { 3.0f,3.0f,1.0f };
	vaporWaveSunRender.data.colorData = { 255,0,0,255 };
	vaporWaveSunRender.data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;


	maxTimer = 60;



	//model.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "boneTest.fbx");
	model.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Gunner_Switch_anim_v02.fbx");
	//objModel.data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "Gunner_Switch_anim.obj");



	stageParamLoader.LoadFile(KazFilePathName::StageParamPath + "RezStageParamData.json");
	if (false)
	{
		std::array<std::vector<char>, 50> data;
		for (int i = 0; i < floorObjectRender.size(); ++i)
		{
			data[i] = KazHelper::CovertStringToChar("FloorObj" + std::to_string(i));
		}

		if (true)
		{
			for (int i = 0; i < floorObjectRender.size(); ++i)
			{
				DirectX::XMVECTOR pos = KazMath::Transform3D().pos.ConvertXMVECTOR();
				DirectX::XMVECTOR scale = KazMath::Transform3D().scale.ConvertXMVECTOR();
				DirectX::XMVECTOR rota = KazMath::Transform3D().rotation.ConvertXMVECTOR();

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
				object.AddMember("FilePass", "no_pass", stageParamLoader.doc.GetAllocator());

				stageParamLoader.doc.AddMember(rapidjson::GenericStringRef<char>(data[i].data()), object, stageParamLoader.doc.GetAllocator());
			}
			stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "RezStageParamData.json");
		}
	}
	selectingR.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	selectingR.data.color = { 255,0,0,255 };

}

void RezStage::Update()
{
#pragma region Grid
	const float lVelZ = -5.0f;

	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		gridFloorZLinePos[i][0].pos.z += lVelZ;
		gridFloorZLinePos[i][1].pos.z += lVelZ;

		if (gridFloorZLinePos[i][0].pos.z <= -700.0f)
		{
			gridFloorZLinePos[i][0].pos.z = -700.0f + static_cast<float>(gridFloorZLinePos.size()) * 300.0f;
			gridFloorZLinePos[i][1].pos.z = gridFloorZLinePos[i][0].pos.z;
		}
	}


	int lineRIndex = 0;
	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		gridLineRender[lineRIndex].data.startPos = gridFloorZLinePos[i][0].pos;
		gridLineRender[lineRIndex].data.endPos = gridFloorZLinePos[i][1].pos;
		++lineRIndex;
	}
#pragma endregion


#pragma region ImGuiObj
	bool lImportFlag = false;
	bool lExportFlag = false;
	ImGui::Begin("FloorObject");
	lImportFlag = ImGui::Button("Import");
	ImGui::SameLine();
	lExportFlag = ImGui::Button("Export");
	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		std::string name = "FloorObj" + std::to_string(i);
		if (ImGui::TreeNode(name.c_str()))
		{
			ImGui::DragFloat("POS_X", &floorObjectRender[i].objRender[0].data.transform.pos.x);
			ImGui::DragFloat("POS_Y", &floorObjectRender[i].objRender[0].data.transform.pos.y);
			ImGui::DragFloat("POS_Z", &floorObjectRender[i].objRender[0].data.transform.pos.z);
			ImGui::DragFloat("SCALE_X", &floorObjectRender[i].objRender[0].data.transform.scale.x);
			ImGui::DragFloat("SCALE_Y", &floorObjectRender[i].objRender[0].data.transform.scale.y);
			ImGui::DragFloat("SCALE_Z", &floorObjectRender[i].objRender[0].data.transform.scale.z);
			ImGui::DragFloat("ROTA_X", &floorObjectRender[i].objRender[0].data.transform.rotation.x);
			ImGui::DragFloat("ROTA_Y", &floorObjectRender[i].objRender[0].data.transform.rotation.y);
			ImGui::DragFloat("ROTA_Z", &floorObjectRender[i].objRender[0].data.transform.rotation.z);
			ImGui::InputText("FilePass", filePassChar[i].data(), sizeof(char) * 100);
			std::string lFilePass = filePassChar[i].data();
			//floorObjectRender[i].objRender[0].data.handle = ObjResourceMgr::Instance()->LoadModel(lFilePass);
			ImGui::TreePop();
		}

		if (floorObjectRender[i].objRender[0].data.transform.Dirty())
		{
			selectingR.data.transform = floorObjectRender[i].objRender[0].data.transform;
			selectingR.data.transform.scale += KazMath::Vec3<float>(1.0f, 1.0f, 1.0f);
		}
	}
	ImGui::End();

	//ファイル読み込み
	if (lImportFlag)
	{
		for (int i = 0; i < floorObjectRender.size(); ++i)
		{
			std::string name = "FloorObj" + std::to_string(i);
			for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
			{
				switch (axisIndex)
				{
				case 0:
					floorObjectRender[i].objRender[0].data.transform.pos.x = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
					floorObjectRender[i].objRender[0].data.transform.scale.x = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
					floorObjectRender[i].objRender[0].data.transform.rotation.x = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();
					break;
				case 1:
					floorObjectRender[i].objRender[0].data.transform.pos.y = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
					floorObjectRender[i].objRender[0].data.transform.scale.y = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
					floorObjectRender[i].objRender[0].data.transform.rotation.y = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();
					break;
				case 2:
					floorObjectRender[i].objRender[0].data.transform.pos.z = stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].GetFloat();
					floorObjectRender[i].objRender[0].data.transform.scale.z = stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].GetFloat();
					floorObjectRender[i].objRender[0].data.transform.rotation.z = stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].GetFloat();
					break;
				default:
					break;
				}
			}
			const char *lTmp = stageParamLoader.doc[name.c_str()]["FilePass"].GetString();

			for (int charIndex = 0; charIndex < filePassChar[i].size(); ++charIndex)
			{
				if (lTmp[charIndex] != '\0')
				{
					filePassChar[i][charIndex] = lTmp[charIndex];
				}
				else
				{
					break;
				}
			}
		}
	}

	//ファイル書き込み
	if (lExportFlag)
	{
		for (int i = 0; i < floorObjectRender.size(); ++i)
		{
			std::string name = "FloorObj" + std::to_string(i);
			for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
			{
				switch (axisIndex)
				{
				case 0:
					stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.pos.x);
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.scale.y);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.rotation.z);
					break;
				case 1:
					stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.pos.x);
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.scale.y);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.rotation.z);
					break;
				case 2:
					stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.pos.x);
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.scale.y);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.rotation.z);

					break;
				default:
					break;
				}
			}
			stageParamLoader.doc[name.c_str()]["FilePass"].SetString(rapidjson::GenericStringRef<char>(filePassChar[i].data()));
		}
		stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "RezStageParamData.json");
	}

#pragma endregion



#pragma region Obj
	if (reversValueFlag)
	{
		scaleRate += 1.0f / 30.0f;
	}
	else
	{
		scaleRate += -(1.0f / 10.0f);
	}

	if (scaleRate <= 0.0f)
	{
		scaleRate = 0.0f;
		reversValueFlag = true;
	}
	else if (1.0f <= scaleRate)
	{
		scaleRate = 1.0f;
		reversValueFlag = false;
	}


	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		floorObjectRender[i].objRender[0].data.transform.pos.z += 0.0f;
		bool limitZLineFlag = floorObjectRender[i].objRender[0].data.transform.pos.z <= -100.0f;

		floorObjectRender[i].objRender[1].data.transform = floorObjectRender[i].objRender[0].data.transform;
		//floorObjectRender[i].objRender[0].data.transform.scale.y = floorObjectRender[i].initScale.y + EasingMaker(Out, Cubic, scaleRate) * 50.0f;
		floorObjectRender[i].objRender[0].data.transform.pos.y = -150.0f + floorObjectRender[i].objRender[0].data.transform.scale.y;

		floorObjectRender[i].objRender[1].data.transform.scale.y = floorObjectRender[i].objRender[0].data.transform.scale.y;
		floorObjectRender[i].objRender[1].data.transform.pos = floorObjectRender[i].objRender[0].data.transform.pos + KazMath::Vec3<float>(0.0f, -(floorObjectRender[i].objRender[0].data.transform.scale.y * 2), 0.0f);


		if (limitZLineFlag)
		{
			floorObjectRender[i].objRender[0].data.transform.pos.z = 8000.0f;
			floorObjectRender[i].objRender[1].data.transform.pos = floorObjectRender[i].objRender[0].data.transform.pos + KazMath::Vec3<float>(0.0f, -(floorObjectRender[i].objRender[0].data.transform.scale.y * 2), 0.0f);
		}
	}
#pragma endregion


#pragma region LightEffect

	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		for (int limitPointIndex = 0; limitPointIndex < lightEffectGridFloorLinePos[i].size(); limitPointIndex++)
		{
			lightEffectGridFloorLinePos[i][limitPointIndex] = gridFloorZLinePos[i][0].pos + KazMath::Vec3<float>(static_cast<float>(limitPointIndex) * -10.0f, 0.0f, 0.0f);
		}
	}
	for (int i = static_cast<int>(gridFloorZLinePos.size()); i < gridFloorZLinePos.size() + gridFloorXLinePos.size(); ++i)
	{
		for (int limitPointIndex = 0; limitPointIndex < lightEffectGridFloorLinePos[i].size(); limitPointIndex++)
		{
			lightEffectGridFloorLinePos[i][limitPointIndex] = gridFloorXLinePos[i - 50][0].pos + KazMath::Vec3<float>(0.0f, 0.0f, static_cast<float>(limitPointIndex) * -10.0f);
		}
	}






	if (maxTimer <= timer)
	{
		maxTimer = KazMath::Rand<int>(60 * 2, 60);
		int countTrue = 0;
		while (countTrue < lightEffectArray.size())
		{
			lightEffectIndex = KazMath::Rand<int>(static_cast<int>(lightEffectArray.size() - 1), 0);
			if (lightEffectArray[lightEffectIndex][0].IsFinish() && lightEffectInitFlagArray[lightEffectIndex])
			{
				break;
			}
			lightEffectInitFlagArray[lightEffectIndex] = true;
			++countTrue;
		}
		timer = 0;
	}
	else
	{
		++timer;
	}

	for (int squareIndex = 0; squareIndex < lightEffectArray[lightEffectIndex].size(); ++squareIndex)
	{
		if (lightEffectArray[lightEffectIndex][squareIndex].IsFinish())
		{
			std::vector<KazMath::Vec3<float> *>lPosArray;
			lPosArray.push_back(&lightEffectGridFloorLinePos[lightEffectIndex][squareIndex]);
			lPosArray.push_back(&gridLineRender[lightEffectIndex].data.endPos);

			lightEffectArray[lightEffectIndex][squareIndex].Init(lPosArray, 60 * 10);
		}
	}

	for (int i = 0; i < lightEffectArray.size(); ++i)
	{
		for (int squareIndex = 0; squareIndex < lightEffectArray[i].size(); ++squareIndex)
		{
			if (lightEffectArray[i][squareIndex].IsFinish() && lightEffectInitFlagArray[i])
			{
				std::vector<KazMath::Vec3<float> *>lPosArray;
				lPosArray.push_back(&lightEffectGridFloorLinePos[i][squareIndex]);
				lPosArray.push_back(&gridLineRender[i].data.endPos);
				lightEffectArray[i][squareIndex].Init(lPosArray, 60 * 10);
			}
		}
	}


	for (int i = 0; i < lightEffectArray.size(); ++i)
	{
		for (int squareIndex = 0; squareIndex < lightEffectArray[i].size(); ++squareIndex)
		{
			lightEffectArray[i][squareIndex].Update();
		}
	}

#pragma endregion
}

void RezStage::Draw()
{

	for (int i = 0; i < gridLineRender.size(); ++i)
	{
		KazMath::Color baseColor(115, 85, 140, 255);
		KazMath::Color flashColor(213, 5, 228, 255);
		KazMath::Color nowColor(0, 0, 0, 255);
		nowColor.color = (flashColor.color - baseColor.color);
		KazMath::Vec4<float> tmpFloatColor = baseColor.color.Float() + nowColor.color.Float() * scaleRate;
		gridLineRender[i].data.color.color = tmpFloatColor.Int();

		gridLineRender[i].Draw();
	}

	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		for (int objIndex = 0; objIndex < floorObjectRender[i].objRender.size(); ++objIndex)
		{
			floorObjectRender[i].objRender[objIndex].Draw();
		}
	}

	for (int i = 0; i < lightEffectArray.size(); ++i)
	{
		for (int squareIndex = 0; squareIndex < lightEffectArray[i].size(); ++squareIndex)
		{
			lightEffectArray[i][squareIndex].Draw();
		}
	}

	selectingR.Draw();

	vaporWaveSunRender.Draw();


	ImGui::Begin("Model");
	ImGui::DragFloat("POS_X", &model.data.transform.pos.x);
	ImGui::DragFloat("POS_Y", &model.data.transform.pos.y);
	ImGui::DragFloat("POS_Z", &model.data.transform.pos.z);
	ImGui::DragFloat("SCALE_X", &model.data.transform.scale.x);
	ImGui::DragFloat("SCALE_Y", &model.data.transform.scale.y);
	ImGui::DragFloat("SCALE_Z", &model.data.transform.scale.z);
	ImGui::End();

	//model.Draw();
	//DrawBackGround();


}