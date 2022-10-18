#include "MapChipPlacementTool.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

MapChipPlacementTool::MapChipPlacementTool() :BLOCK_SIZE(10.0f)
{
	instanceFlameRender = std::make_unique<BoxPolygonRender>(true, matSize);
	instanceFlameRender->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR_WIREFLAME;
	instanceHandle = instanceFlameRender->CreateConstBuffer(sizeof(MatData) * matSize, typeid(MatData).name(), GRAPHICS_RANGE_TYPE_UAV, GRAPHICS_PRAMTYPE_DATA);

	matIndex = 0;
	for (int y = 0; y < defaultBlockPosArray.size(); ++y)
	{
		for (int x = 0; x < defaultBlockPosArray[y].size(); ++x)
		{
			defaultBlockPosArray[y][x] = KazMath::Transform3D(KazMath::Vec3<float>(static_cast<float>(y) * 2.0f, 0.0f, static_cast<float>(x) * 2.0f));

			lMatData[matIndex].mat = KazMath::CaluMat(
				defaultBlockPosArray[y][x],
				CameraMgr::Instance()->GetViewMatrix(),
				CameraMgr::Instance()->GetPerspectiveMatProjection(),
				{ 0,1,0 },
				{ 0,0,1 }
			);

			if (x % 10 == 0)
			{
				lMatData[matIndex].color = { 155,0,0,255 };
			}
			else
			{
				lMatData[matIndex].color = { 155,155,155,255 };
			}
			++matIndex;
		}
	}

	cursorR.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "tex.png");



	instanceBoxRender = std::make_unique<BoxPolygonRender>(true, BLOCK_MAX_NUM);
	instanceBoxRender->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR_WIREFLAME;
	instanceBoxHandle = instanceBoxRender->CreateConstBuffer(sizeof(MatData) * BLOCK_MAX_NUM, typeid(MatData).name(), GRAPHICS_RANGE_TYPE_UAV, GRAPHICS_PRAMTYPE_DATA);


	for (int x = 0; x < blockPosArray.size(); ++x)
	{
		for (int y = 0; y < blockPosArray[x].size(); ++y)
		{
			for (int z = 0; z < blockPosArray[x][y].size(); ++z)
			{
				blockPosArray[x][y][z].x = REV_VALUE;
			}
		}
	}

	blockPosArray[0][0][0] = { 0.0f,0.0f,0.0f };
}

void MapChipPlacementTool::Input(bool PUT_FLAG, bool RELEASE_FLAG, const DirectX::XMFLOAT2 &MOUSE_POS)
{
	//ブロックの設置
	inputFlag = PUT_FLAG;
	//ブロックの削除
	releaseFlag = RELEASE_FLAG;

	KazMath::Vec3<float> nearPos = { MOUSE_POS.x,MOUSE_POS.y,0.0f };
	KazMath::Vec3<float> farPos = { MOUSE_POS.x,MOUSE_POS.y,1.0f };
	KazMath::Vec3<float> endPos = KazMath::ConvertScreenPosToWorldPos(farPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	ray.start = KazMath::ConvertScreenPosToWorldPos(nearPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	KazMath::Vec3<float> dir(endPos - ray.start);
	dir.Normalize();
	ray.dir = dir;

	cursorR.data.transform.pos = { MOUSE_POS.x,MOUSE_POS.y };

}

void MapChipPlacementTool::Init()
{
}

void MapChipPlacementTool::Update()
{
	if (inputFlag)
	{
		int u = 0;
		u = 0;
	}
	int lBlockNum = 0;

	putIndex = { 0,0,0 };
	selectingIndex = { 0,0,0 };
	//ブロックとレイとの判定
	for (int x = 0; x < blockPosArray.size(); ++x)
	{
		for (int y = 0; y < blockPosArray[x].size(); ++y)
		{
			for (int z = 0; z < blockPosArray[x][y].size(); ++z)
			{
				if (blockPosArray[x][y][z].x != REV_VALUE)
				{
					blockMatData[lBlockNum].mat = KazMath::CaluMat(
						KazMath::Transform3D(blockPosArray[x][y][z], { BLOCK_SIZE,BLOCK_SIZE ,BLOCK_SIZE }, {}),
						CameraMgr::Instance()->GetViewMatrix(),
						CameraMgr::Instance()->GetPerspectiveMatProjection(),
						{ 0,1,0 },
						{ 0,0,1 }
					);

					blockMatData[lBlockNum].color = { 0.0f,1.0f,0.0f,1.0f };
					++lBlockNum;
				}
				else
				{
					continue;
				}

				MESH_DIR lDir;
				if ((lDir = CheckBlock(blockPosArray[x][y][z])) != NONE)
				{
					selectR.data.transform.pos = blockPosArray[x][y][z];
					selectingIndex = KazMath::Vec3<int>(x, y, z);
					putIndex = selectingIndex + AdjIndex(lDir);
				}
			}
		}
	}


	ImGui::Begin("MapChipPlacementTool");
	//現在指定しているブロックの座標
	ImGui::Text("selectingIndex,X:%d,Y:%d,Z:%d", selectingIndex.x, selectingIndex.y, selectingIndex.z);
	//一括配置
	KazImGuiHelper::InputVec3("PutBlockInOnce", &blockPutInOnceSize);
	bool lPutFlag = ImGui::Button("Put");
	ImGui::Text("PutPos,X:%d,Y:%d,Z:%d", putIndex.x, putIndex.y, putIndex.z);
	ImGui::End();

	//一括配置の処理
	if (lPutFlag)
	{

	}

	if (putIndex.x <= -1)
	{
		putIndex.x = 0;
	}
	if (putIndex.y <= -1)
	{
		putIndex.y = 0;
	}
	if (putIndex.z <= -1)
	{
		putIndex.z = 0;
	}
	if (selectingIndex.x <= -1)
	{
		selectingIndex.x = 0;
	}
	if (selectingIndex.y <= -1)
	{
		selectingIndex.y = 0;
	}
	if (selectingIndex.z <= -1)
	{
		selectingIndex.z = 0;
	}



	//配置
	if (inputFlag)
	{
		blockPosArray[putIndex.x][putIndex.y][putIndex.z] = putIndex.Float() * (BLOCK_SIZE * 2.0f);
	}

	//削除,別の所に置く
	if (releaseFlag)
	{
		blockPosArray[selectingIndex.x][selectingIndex.y][selectingIndex.z] = { CONTINE_VALUE,0.0f,0.0f };
	}

	matIndex = 0;
	//for (int y = 0; y < defaultBlockPosArray.size(); ++y)
	//{
	//	for (int x = 0; x < defaultBlockPosArray[y].size(); ++x)
	//	{
	//		lMatData[matIndex].mat = KazMath::CaluMat(
	//			defaultBlockPosArray[y][x],
	//			CameraMgr::Instance()->GetViewMatrix(),
	//			CameraMgr::Instance()->GetPerspectiveMatProjection(),
	//			{ 0,1,0 },
	//			{ 0,0,1 }
	//		);
	//		++matIndex;
	//	}
	//}
	//instanceFlameRender->TransData(lMatData.data(), instanceHandle, typeid(MatData).name());
	instanceBoxRender->TransData(blockMatData.data(), instanceBoxHandle, typeid(MatData).name());

}

void MapChipPlacementTool::Draw()
{
	//instanceFlameRender->Draw();
	instanceBoxRender->Draw();
	selectR.Draw();

	//cursorR.Draw();
	//b.Draw();
}

MapChipPlacementTool::MESH_DIR MapChipPlacementTool::CheckBlock(const KazMath::Vec3<float> &POS)
{
	KazMath::Vec3<float>lCentralPos = POS;

	struct HitData
	{
		HitData(MESH_DIR DIR, float DISTANCE) :dir(DIR), distance(DISTANCE)
		{};
		MESH_DIR dir;
		float distance;
	};

	std::vector<float>lDistanceArray;
	std::vector<HitData>lHitDataArray;

	std::array<ModiRectangle, 6>lPlane;
	for (int planeIndex = 0; planeIndex < lPlane.size(); ++planeIndex)
	{
		MESH_DIR lDir = static_cast<MESH_DIR>(planeIndex);

		const float HALF_SCALE = BLOCK_SIZE;
		//どの方向に向くかで板の位置と向きを決める
		lCentralPos += AdjIndex(lDir).Float() * HALF_SCALE;

		//左上、左下、右上、右下の順で並べる
		switch (lDir)
		{
		case MapChipPlacementTool::NONE:
			break;
		case MapChipPlacementTool::TOP:
		case MapChipPlacementTool::BUTTOM:
			lPlane[planeIndex].p0 = lCentralPos + KazMath::Vec3<float>(-HALF_SCALE, 0.0f, HALF_SCALE);
			lPlane[planeIndex].p1 = lCentralPos + KazMath::Vec3<float>(-HALF_SCALE, 0.0f, -HALF_SCALE);
			lPlane[planeIndex].p2 = lCentralPos + KazMath::Vec3<float>(HALF_SCALE, 0.0f, HALF_SCALE);
			lPlane[planeIndex].p3 = lCentralPos + KazMath::Vec3<float>(HALF_SCALE, 0.0f, -HALF_SCALE);
			break;
		case MapChipPlacementTool::FRONT:
		case MapChipPlacementTool::BACK:
			lPlane[planeIndex].p0 = lCentralPos + KazMath::Vec3<float>(-HALF_SCALE, HALF_SCALE, 0.0f);
			lPlane[planeIndex].p1 = lCentralPos + KazMath::Vec3<float>(-HALF_SCALE, -HALF_SCALE, 0.0f);
			lPlane[planeIndex].p2 = lCentralPos + KazMath::Vec3<float>(HALF_SCALE, HALF_SCALE, 0.0f);
			lPlane[planeIndex].p3 = lCentralPos + KazMath::Vec3<float>(HALF_SCALE, -HALF_SCALE, 0.0f);
			break;
		case MapChipPlacementTool::LEFT:
		case MapChipPlacementTool::RIGHT:
			lPlane[planeIndex].p0 = lCentralPos + KazMath::Vec3<float>(0.0f, HALF_SCALE, HALF_SCALE);
			lPlane[planeIndex].p1 = lCentralPos + KazMath::Vec3<float>(0.0f, -HALF_SCALE, HALF_SCALE);
			lPlane[planeIndex].p2 = lCentralPos + KazMath::Vec3<float>(0.0f, HALF_SCALE, -HALF_SCALE);
			lPlane[planeIndex].p3 = lCentralPos + KazMath::Vec3<float>(0.0f, -HALF_SCALE, -HALF_SCALE);
			break;
		default:
			break;
		}

		float lDistance;
		//判定が取れたら座標を入手できたことを示す
		if (CollisionManager::Instance()->CheckRayAndPlane3D(ray, lPlane[planeIndex], &lDistance))
		{
			lDistanceArray.push_back(lDistance);
			lHitDataArray.push_back(HitData(lDir, lDistance));
		}
	}
	std::sort(lDistanceArray.begin(), lDistanceArray.end());

	std::vector<HitData>lResultArray;

	//ソートをかける
	for (int i = 0; i < lDistanceArray.size(); ++i)
	{
		for (int dataIndex = 0; dataIndex < lHitDataArray.size(); ++dataIndex)
		{
			if (lDistanceArray[i] == lHitDataArray[dataIndex].distance)
			{
				lResultArray.push_back(lHitDataArray[dataIndex]);
			}
		}
	}

	if (lResultArray.size() != 0)
	{
		return static_cast<MESH_DIR>(lResultArray[0].dir);
	}
	else
	{
		return static_cast<MESH_DIR>(-1);
	}
}