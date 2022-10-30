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


	for (int i = 0; i < gridRender.size(); ++i)
	{
		gridRender[i] = std::make_unique<DrawGrid>(KazMath::Color(29, 19, 72, 255));
	}

	KazMath::Color lBaseColor(115, 85, 140, 255);
	KazMath::Color lFlashColor(213, 5, 228, 255);
	std::array<KazMath::Color, 2>lFlashColorArray = { lBaseColor,lFlashColor };


	gridRender[0]->Init(true, 300.0f, -150.0f, &cameraIndex, true, false, lFlashColorArray);
	gridRender[1]->Init(true, 300.0f, 3000.0f, &cameraIndex, true, false, lFlashColorArray);
	gridRender[2]->Init(false, 300.0f, -3000.0f, &cameraIndex, true, false, lFlashColorArray);
	gridRender[3]->Init(false, 300.0f, 3000.0f, &cameraIndex, true, false, lFlashColorArray);


	for (int i = 0; i < filePassNum.size(); ++i)
	{
		filePassNum[i] = 2;
	}

	stageModelhandle[0] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain01_Model.obj");
	stageModelhandle[1] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain02_Model.obj");
	stageModelhandle[2] = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Mountain03_Model.obj");

	vaporWaveSunRender.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::RelativeResourcePath + "Stage/" + "vaporWaveSun.png");
	vaporWaveSunRender.data.transform.pos = { 0.0f,2000.0f,5000.0f };
	vaporWaveSunRender.data.transform.scale = { 0.0f,0.0f,1.0f };
	vaporWaveSunRender.data.colorData = { 255,0,0,255 };
	vaporWaveSunRender.data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;

	maxTimer = 60;



	selectingR.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	selectingR.data.color = { 255,0,0,255 };



	stageParamLoader.LoadFile(KazFilePathName::StageParamPath + "RezStageParamData.json");
	//パラメータから判断
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
	appearRate = 0.0f;



	InitBlockMountainData lInitData;
	lInitData.fogDesinty = 0.001f;
	lInitData.lightDir = { -1.0f,1.0f,1.0f };
	lInitData.xRange = 70;
	lInitData.yScale = 500.0f;
	lInitData.centralPos = { 0.0f,-1500.0f,0.0f };
	blockMountainArray[0] = std::make_unique<BlockMountain>(lInitData);

	lInitData.fogDesinty = 0.0005f;
	lInitData.lightDir = { 0.0f,0.0f,1.0f };
	lInitData.xRange = 70;
	lInitData.yScale = 500.0f;
	lInitData.centralPos = { 0.0f,5300.0f,0.0f };
	blockMountainArray[1] = std::make_unique<BlockMountain>(lInitData);

	
	lInitData.fogDesinty = 0.0005f;
	lInitData.lightDir = { -1.0f,0.0f,0.0f };
	lInitData.xRange = 10;
	lInitData.yScale = 1000.0f;
	lInitData.centralPos = { -6400.0f,0.0f,0.0f };
	blockMountainArray[2] = std::make_unique<BlockMountain>(lInitData);


	lInitData.fogDesinty = 0.0005f;
	lInitData.lightDir = { 1.0f,0.0f,0.0f };
	lInitData.xRange = 10;
	lInitData.yScale = 1000.0f;
	lInitData.centralPos = { 6400.0f,0.0f,0.0f };
	blockMountainArray[3] = std::make_unique<BlockMountain>(lInitData);
}

void RezStage::Update()
{
	for (int i = 0; i < gridRender.size(); ++i)
	{
		gridRender[i]->Update(-1.0f, false);
	}

	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		floorObjectRender[i].Update();
	}

	if (startFlag)
	{
		Rate(&appearRate, 0.01f, 1.0f);
		float easeRate = EasingMaker(Out, Cubic, appearRate) * 3.0f;
		vaporWaveSunRender.data.transform.scale.x = easeRate;
		vaporWaveSunRender.data.transform.scale.y = easeRate;
		vaporWaveSunRender.data.transform.scale.z = easeRate;
	}

	for (int i = 0; i < blockMountainArray.size(); ++i)
	{
		blockMountainArray[i]->Update();
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
		gridRender[i]->Draw();
	}

	for (int i = 0; i < blockMountainArray.size(); ++i)
	{
		blockMountainArray[i]->Draw();
	}
}