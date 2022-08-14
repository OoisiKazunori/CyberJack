#include "RezStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary/Helper/KazHelper.h"

RezStage::RezStage()
{
	poly = std::make_unique<BoxPolygonRender>(true, 400);
	poly->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR;
	lineDrawHandle = poly->CreateConstBuffer(sizeof(MatData) * 500, "MatData", GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DRAW);


	gridRender[0].Init(true, 300.0f, -150.0f);
	gridRender[1].Init(true, 300.0f, 3000.0f);
	gridRender[2].Init(false, 300.0f, -3000.0f);
	gridRender[3].Init(false, 300.0f, 3000.0f);


	for (int i = 0; i < filePassNum.size(); ++i)
	{
		filePassNum[i] = 2;
	}

	stageModelhandle[0] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain01_Model.obj");
	stageModelhandle[1] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain02_Model.obj");
	stageModelhandle[2] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain03_Model.obj");

	RESOURCE_HANDLE lR = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain03_Model.obj");
	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		const float maxXPos = 4000.0f;
		const float minXPos = 500.0f;

		floorObjectRender[i].objRender[0].data.handle = lR;
		floorObjectRender[i].objRender[1].data.handle = lR;

		if (KazMath::Rand<int>(2, 0))
		{
			floorObjectRender[i].objRender[0].data.transform.pos.x = KazMath::Rand<float>(-maxXPos, -minXPos);
		}
		else
		{
			floorObjectRender[i].objRender[0].data.transform.pos.x = KazMath::Rand<float>(maxXPos, minXPos);
		}
		floorObjectRender[i].objRender[0].data.transform.pos.z = KazMath::Rand<float>(10000, -100);
		floorObjectRender[i].initScale = floorObjectRender[i].objRender[0].data.transform.scale;
		floorObjectRender[i].objRender[0].data.transform.pos.y = -150.0f + floorObjectRender[i].objRender[0].data.transform.scale.y;
		floorObjectRender[i].objRender[0].data.colorData = KazMath::Color(213, 5, 228, 255);
		floorObjectRender[i].objRender[0].data.pipelineName = PIPELINE_NAME_OBJ_WIREFLAME_FOG;
		floorObjectRender[i].objRender[0].data.removeMaterialFlag = true;

		floorObjectRender[i].objRender[1].data.transform.pos = floorObjectRender[i].objRender[0].data.transform.pos + KazMath::Vec3<float>(0.0f, -50.0f, 0.0f);
		floorObjectRender[i].objRender[1].data.transform.scale = floorObjectRender[i].objRender[0].data.transform.scale;
		floorObjectRender[i].objRender[1].data.transform.rotation = { 180.0f,0.0f,0.0f };
		floorObjectRender[i].objRender[1].data.colorData = KazMath::Color(248, 58, 16, 255);
		floorObjectRender[i].objRender[1].data.pipelineName = PIPELINE_NAME_OBJ_WIREFLAME_FOG;

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

				//Box���̃����o�ϐ���ǉ�
				rapidjson::Value posArray(rapidjson::kArrayType);
				rapidjson::Value scaleArray(rapidjson::kArrayType);
				rapidjson::Value rotaArray(rapidjson::kArrayType);
				for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
				{
					posArray.PushBack(rapidjson::Value(lPos.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
					scaleArray.PushBack(rapidjson::Value(lScale.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
					rotaArray.PushBack(rapidjson::Value(lRota.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
				}

				//Box�I�u�W�F�N�g�Ƀf�[�^��ǉ�
				rapidjson::Value object(rapidjson::kObjectType);
				object.AddMember("Pos", posArray, stageParamLoader.doc.GetAllocator());
				object.AddMember("Scale", scaleArray, stageParamLoader.doc.GetAllocator());
				object.AddMember("Rota", rotaArray, stageParamLoader.doc.GetAllocator());
				object.AddMember("FilePass", 0, stageParamLoader.doc.GetAllocator());

				stageParamLoader.doc.AddMember(rapidjson::GenericStringRef<char>(data[i].data()), object, stageParamLoader.doc.GetAllocator());
			}
			stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "RezStageParamData.json");
		}
	}
	selectingR.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	selectingR.data.color = { 255,0,0,255 };


	ray = std::make_unique<LineRender>();




	//�p�����[�^���画�f
	for (int i = 0; i < 40; ++i)
	{
		std::string name = "FloorObj" + std::to_string(i);
		KazMath::Transform3D initTrans;
		initTrans.pos.x = stageParamLoader.doc[name.c_str()]["Pos"][0].GetFloat();
		initTrans.pos.y = stageParamLoader.doc[name.c_str()]["Pos"][1].GetFloat();
		initTrans.pos.z = stageParamLoader.doc[name.c_str()]["Pos"][2].GetFloat();
		
		initTrans.scale.x = stageParamLoader.doc[name.c_str()]["Scale"][0].GetFloat();
		initTrans.scale.y = stageParamLoader.doc[name.c_str()]["Scale"][1].GetFloat();
		initTrans.scale.z = stageParamLoader.doc[name.c_str()]["Scale"][2].GetFloat();

		initTrans.rotation.x = stageParamLoader.doc[name.c_str()]["Rota"][0].GetFloat();
		initTrans.rotation.y = stageParamLoader.doc[name.c_str()]["Rota"][1].GetFloat();
		initTrans.rotation.z = stageParamLoader.doc[name.c_str()]["Rota"][2].GetFloat();

		int lNum = stageParamLoader.doc[name.c_str()]["FilePass"].GetInt();
		floorObjectRender2[i].Init(initTrans, stageModelhandle[lNum]);
	}
}

void RezStage::Update()
{
	for (int i = 0; i < gridRender.size(); ++i)
	{
		gridRender[i].Update();
	}

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
			ImGui::InputInt("FilePass", &filePassNum[i]);
			ImGui::TreePop();
		}

		if (floorObjectRender[i].objRender[0].data.transform.Dirty())
		{
			selectingR.data.transform = floorObjectRender[i].objRender[0].data.transform;
			selectingR.data.transform.scale += KazMath::Vec3<float>(1.0f, 1.0f, 1.0f);
		}


		if (stageModelhandle.size() <= filePassNum[i])
		{
			filePassNum[i] = static_cast<int>(stageModelhandle.size() - 1);
		}
		if (filePassNum[i] <= -1)
		{
			filePassNum[i] = 0;
		}
		floorObjectRender[i].objRender[0].data.handle = stageModelhandle[filePassNum[i]];
		floorObjectRender[i].objRender[1].data.handle = stageModelhandle[filePassNum[i]];
	}
	ImGui::End();



	//�t�@�C���ǂݍ���
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

			floorObjectRender[i].initScale.y = floorObjectRender[i].objRender[0].data.transform.scale.y;
			filePassNum[i] = stageParamLoader.doc[name.c_str()]["FilePass"].GetInt();
			floorObjectRender[i].objRender[0].data.handle = stageModelhandle[filePassNum[i]];
			floorObjectRender[i].objRender[1].data.handle = stageModelhandle[filePassNum[i]];
		}
	}

	//�t�@�C����������
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
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.scale.x);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.rotation.x);
					break;
				case 1:
					stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.pos.y);
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.scale.y);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.rotation.y);
					break;
				case 2:
					stageParamLoader.doc[name.c_str()]["Pos"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.pos.z);
					stageParamLoader.doc[name.c_str()]["Scale"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.scale.z);
					stageParamLoader.doc[name.c_str()]["Rota"][axisIndex].SetFloat(floorObjectRender[i].objRender[0].data.transform.rotation.z);
					break;
				default:
					break;
				}
			}
			stageParamLoader.doc[name.c_str()]["FilePass"].SetInt(filePassNum[i]);
		}
		stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "RezStageParamData.json");
	}

#pragma endregion




	for (int i = 0; i < floorObjectRender2.size(); ++i)
	{
		floorObjectRender2[i].Update();
	}

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
		float lVelZ = -5.0f;
		floorObjectRender[i].objRender[0].data.transform.pos.z += lVelZ;
		bool limitZLineFlag = floorObjectRender[i].objRender[0].data.transform.pos.z <= -1000.0f;

		floorObjectRender[i].objRender[1].data.transform = floorObjectRender[i].objRender[0].data.transform;
		floorObjectRender[i].objRender[0].data.transform.scale.y = floorObjectRender[i].initScale.y + EasingMaker(Out, Cubic, scaleRate) * 0.0f;
		floorObjectRender[i].objRender[0].data.transform.pos.y = -150.0f + floorObjectRender[i].objRender[0].data.transform.scale.y;

		floorObjectRender[i].objRender[1].data.transform.scale.y = floorObjectRender[i].objRender[0].data.transform.scale.y;
		floorObjectRender[i].objRender[1].data.transform.pos = floorObjectRender[i].objRender[0].data.transform.pos;
		floorObjectRender[i].objRender[1].data.transform.rotation = { 180.0f,0.0f,0.0f };

		floorObjectRender[i].objRender[1].data.transform.rotation.y = floorObjectRender[i].objRender[0].data.transform.rotation.y;
		
		if (limitZLineFlag)
		{
			floorObjectRender[i].objRender[0].data.transform.pos.z = 8000.0f;
			floorObjectRender[i].objRender[1].data.transform.pos = floorObjectRender[i].objRender[0].data.transform.pos + KazMath::Vec3<float>(0.0f, -(floorObjectRender[i].objRender[0].data.transform.scale.y * 2), 0.0f);
		}
	}
#pragma endregion


	std::array<MatData, 400>lArrayData;
	for (int i = 0; i < 400; ++i)
	{
		lArrayData[i].mat = DirectX::XMMatrixIdentity() * CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
		lArrayData[i].color = { 1.0f,1.0f,1.0f,1.0f };
	}
	poly->TransData(lArrayData.data(), lineDrawHandle, "MatData");


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
	for (int i = 0; i < 40; ++i)
	{
		for (int objectIndex = 0; objectIndex < floorObjectRender[i].objRender.size(); ++objectIndex)
		{
			//floorObjectRender[i].objRender[objectIndex].Draw();
		}
	}


	for (int i = 0; i < floorObjectRender2.size(); ++i)
	{
		floorObjectRender2[i].Draw();
	}

	for (int i = 0; i < gridRender.size(); ++i)
	{
		gridRender[i].Draw();
	}


	poly->Draw();


	selectingR.Draw();

	vaporWaveSunRender.Draw();



}