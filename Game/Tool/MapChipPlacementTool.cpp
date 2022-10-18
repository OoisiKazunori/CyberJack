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
	instanceBoxRender->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR;
	instanceBoxHandle = instanceBoxRender->CreateConstBuffer(sizeof(MatData) * BLOCK_MAX_NUM, typeid(MatData).name(), GRAPHICS_RANGE_TYPE_UAV, GRAPHICS_PRAMTYPE_DATA);

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
}

void MapChipPlacementTool::Init()
{
}

void MapChipPlacementTool::Update()
{
	if (inputFlag)
	{
		bool debug = false;
		debug = true;
	}

	int lBlockNum = 0;
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
						defaultBlockPosArray[y][x],
						CameraMgr::Instance()->GetViewMatrix(),
						CameraMgr::Instance()->GetPerspectiveMatProjection(),
						{ 0,1,0 },
						{ 0,0,1 }
					);

					blockMatData[lBlockNum].color = { 0.0f,1.0f,0.0f,1.0f };
					++lBlockNum;
				}

				MESH_DIR lDir;
				if ((lDir = CheckBlock(blockPosArray[x][y][z])) != NONE)
				{
					selectR.data.transform.pos = blockPosArray[x][y][z];
					selectingIndex = KazMath::Vec3<int>(x, y, z);
					putIndex = selectingIndex + AdjIndex(lDir);
					break;
				}

			}
		}
	}


	ImGui::Begin("MapChipPlacementTool");
	//現在指定しているブロックの座標
	KazImGuiHelper::DrawVec3("SelectingBlockPos", selectR.data.transform.pos);
	//一括配置
	KazImGuiHelper::InputVec3("PutBlockInOnce", &blockPutInOnceSize);
	bool lPutFlag = ImGui::Button("Put");
	ImGui::End();

	//一括配置の処理
	if (lPutFlag)
	{

	}

	//配置
	if (inputFlag)
	{
		//blockPosArray[putIndex.x][putIndex.y][putIndex.z] = putIndex.Float() * BLOCK_SIZE;
	}

	//削除,別の所に置く
	if (releaseFlag)
	{
		//blockPosArray[selectingIndex.x][selectingIndex.y][selectingIndex.z] = { REV_VALUE,0.0f,0.0f };
	}

	matIndex = 0;
	for (int y = 0; y < defaultBlockPosArray.size(); ++y)
	{
		for (int x = 0; x < defaultBlockPosArray[y].size(); ++x)
		{
			lMatData[matIndex].mat = KazMath::CaluMat(
				defaultBlockPosArray[y][x],
				CameraMgr::Instance()->GetViewMatrix(),
				CameraMgr::Instance()->GetPerspectiveMatProjection(),
				{ 0,1,0 },
				{ 0,0,1 }
			);
			++matIndex;
		}
	}
	instanceFlameRender->TransData(lMatData.data(), instanceHandle, typeid(MatData).name());
	instanceBoxRender->TransData(blockMatData.data(), instanceBoxHandle, typeid(MatData).name());
}

void MapChipPlacementTool::Draw()
{
	instanceFlameRender->Draw();
	instanceBoxRender->Draw();
	selectR.Draw();

	cursorR.Draw();
}

MapChipPlacementTool::MESH_DIR MapChipPlacementTool::CheckBlock(const KazMath::Vec3<float> &POS)
{
	KazMath::Vec3<float>lCentralPos = POS;

	std::array<Plane, 6>lPlane;
	for (int planeIndex = 0; planeIndex < lPlane.size(); ++planeIndex)
	{
		MESH_DIR lDir = static_cast<MESH_DIR>(planeIndex);

		//どの方向に向くかで板の位置と向きを決める
		lCentralPos += AdjIndex(lDir).Float() * (BLOCK_SIZE / 2.0f);
		lPlane[planeIndex].distance = lCentralPos.Length();
		lPlane[planeIndex].normal = AdjIndex(lDir).Float();

		//判定が取れたら座標を入手できたことを示す
		if (CollisionManager::Instance()->CheckRayAndPlane(ray, lPlane[planeIndex], nullptr, nullptr))
		{
			return lDir;
		}
	}
	return static_cast<MESH_DIR>(-1);
}