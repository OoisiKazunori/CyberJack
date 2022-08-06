#include "RezStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary/Helper/KazHelper.h"

RezStage::RezStage()
{

	floorGrid.Init(true, 300.0f);

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
		floorObjectRender[i].objRender[0].data.transform.pos.y = -150.0f + floorObjectRender[i].objRender[0].data.transform.scale.y;


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
				DirectX::XMVECTOR lPos = KazMath::Transform3D().pos.ConvertXMVECTOR();
				DirectX::XMVECTOR lScale = KazMath::Transform3D().scale.ConvertXMVECTOR();
				DirectX::XMVECTOR lRota = KazMath::Transform3D().rotation.ConvertXMVECTOR();

				//Box毎のメンバ変数を追加
				rapidjson::Value posArray(rapidjson::kArrayType);
				rapidjson::Value scaleArray(rapidjson::kArrayType);
				rapidjson::Value rotaArray(rapidjson::kArrayType);
				for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
				{
					posArray.PushBack(rapidjson::Value(lPos.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
					scaleArray.PushBack(rapidjson::Value(lScale.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
					rotaArray.PushBack(rapidjson::Value(lRota.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
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


	ray = std::make_unique<LineRender>();
}

void RezStage::Update()
{
	floorGrid.Update();

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
		//floorObjectRender[i].objRender[0].data.transform.pos.z += 0.0f;
		bool limitZLineFlag = floorObjectRender[i].objRender[0].data.transform.pos.z <= -100.0f;

		floorObjectRender[i].objRender[1].data.transform = floorObjectRender[i].objRender[0].data.transform;
		floorObjectRender[i].objRender[0].data.transform.scale.y = floorObjectRender[i].initScale.y + EasingMaker(Out, Cubic, scaleRate) * 50.0f;
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



	ImGui::Begin("C");
	ImGui::DragFloat("CirclePosX", &pos.x);
	ImGui::DragFloat("CircleposY", &pos.y);
	ImGui::DragFloat("CircleposZ", &pos.z);
	ImGui::DragFloat("radius", &sphere.radius);
	ImGui::DragFloat("RayPosX1", &ray->data.startPos.x);
	ImGui::DragFloat("RayposY1", &ray->data.startPos.y);
	ImGui::DragFloat("RayposZ1", &ray->data.startPos.z);
	ImGui::DragFloat("RayPosX2", &ray->data.endPos.x);
	ImGui::DragFloat("RayposY2", &ray->data.endPos.y);
	ImGui::DragFloat("RayposZ2", &ray->data.endPos.z);
	ImGui::End();



	sphere.center = &pos;
	std::array<KazMath::Vec2<float>, 2>lTmp =
		CollisionManager::Instance()->CheckCircleAndRay(sphere, ray->data.startPos.ConvertVec2(), ray->data.endPos.ConvertVec2());

	c1.data.transform.pos.x = lTmp[0].x;
	c1.data.transform.pos.y = lTmp[0].y;
	c2.data.transform.pos.x = lTmp[1].x;
	c2.data.transform.pos.y = lTmp[1].y;
}

void RezStage::Draw()
{

	floorGrid.Draw();

	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		for (int objIndex = 0; objIndex < floorObjectRender[i].objRender.size(); ++objIndex)
		{
			floorObjectRender[i].objRender[objIndex].Draw();
		}
	}

	selectingR.Draw();

	vaporWaveSunRender.Draw();

	ray->Draw();
	c1.Draw();
	c2.Draw();

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