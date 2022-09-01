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


	gridRender[0].Init(true, 300.0f, -150.0f, &cameraIndex);
	gridRender[1].Init(true, 300.0f, 3000.0f, &cameraIndex);
	gridRender[2].Init(false, 300.0f, -3000.0f, &cameraIndex);
	gridRender[3].Init(false, 300.0f, 3000.0f, &cameraIndex);


	for (int i = 0; i < filePassNum.size(); ++i)
	{
		filePassNum[i] = 2;
	}

	stageModelhandle[0] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain01_Model.obj");
	stageModelhandle[1] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain02_Model.obj");
	stageModelhandle[2] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain03_Model.obj");

	vaporWaveSunRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::RelativeResourcePath + "Stage/" + "vaporWaveSun.png");
	vaporWaveSunRender.data.transform.pos = { 0.0f,2000.0f,5000.0f };
	vaporWaveSunRender.data.transform.scale = { 3.0f,3.0f,1.0f };
	vaporWaveSunRender.data.colorData = { 255,0,0,255 };
	vaporWaveSunRender.data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;


	maxTimer = 60;




	selectingR.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	selectingR.data.color = { 255,0,0,255 };



	stageParamLoader.LoadFile(KazFilePathName::StageParamPath + "RezStageParamData.json");
	//ÉpÉâÉÅÅ[É^Ç©ÇÁîªíf
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
		floorObjectRender[i].Init(initTrans, stageModelhandle[lNum], &cameraIndex);

		initTrans.pos.z -= 9000.0f;
		floorObjectRender[40 + i].Init(initTrans, stageModelhandle[lNum], &cameraIndex);
	}

	vaporWaveSunRender.data.colorData.color.a = 0;
}

void RezStage::Update()
{
	for (int i = 0; i < gridRender.size(); ++i)
	{
		gridRender[i].Update();
	}

	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		floorObjectRender[i].Update();
	}
}

void RezStage::Draw()
{
	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		floorObjectRender[i].Draw();
	}

	for (int i = 0; i < gridRender.size(); ++i)
	{
		gridRender[i].Draw();
	}
}