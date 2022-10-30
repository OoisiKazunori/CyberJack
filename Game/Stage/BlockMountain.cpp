#include "BlockMountain.h"
#include"../KazLibrary/Imgui/MyImgui.h"

BlockMountain::BlockMountain(const InitBlockMountainData &INIT_DATA)
{
	const float L_SCALE = 100.0f;
	const int X_MAX_NUM = INIT_DATA.xRange;
	const int Z_MAX_NUM = 70;

	//それぞれの軸のブロックの配置最大数からマップの真ん中に配置するようにする
	const KazMath::Vec3<float>L_BLOCK_CENTRAL_POS(
		-(static_cast<float>(X_MAX_NUM) * (L_SCALE * 2.0f)) / 2.0f,
		0.0f,
		-(static_cast<float>(Z_MAX_NUM) * (L_SCALE * 2.0f)) / 2.0f
	);


	boxMaxNum = X_MAX_NUM * Z_MAX_NUM;

	boxRender = std::make_unique<BoxPolygonRender>(true, boxMaxNum);
	boxRender->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR_MULTITEX_SHADING;
	objectBufferHandle = boxRender->CreateConstBuffer(sizeof(ObjectData), typeid(ObjectData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA2);
	instanceBufferHandle = boxRender->CreateConstBuffer(sizeof(MatData) * boxMaxNum, typeid(MatData).name(), GRAPHICS_RANGE_TYPE_UAV, GRAPHICS_PRAMTYPE_DATA);

	boxDataArray.resize(boxMaxNum);


	int lBoxIndex = 0;
	for (int x = 0; x < X_MAX_NUM; ++x)
	{
		for (int z = 0; z < Z_MAX_NUM; ++z)
		{
			boxDataArray[lBoxIndex].pos =
			{
				INIT_DATA.centralPos.x + L_BLOCK_CENTRAL_POS.x + static_cast<float>(x) * (L_SCALE * 2.2f),
				INIT_DATA.centralPos.y,
				INIT_DATA.centralPos.z + L_BLOCK_CENTRAL_POS.z + static_cast<float>(z) * (L_SCALE * 2.2f),
			};



			boxDataArray[lBoxIndex].scale = { L_SCALE ,KazMath::Rand(INIT_DATA.yScale,50.0f),L_SCALE };
			++lBoxIndex;
		}
	}

	fogColor.a = INIT_DATA.fogDesinty;
	objectData.luminanceColor = { 0.0f,0.0f,0.0f,1.0f };
	DirectX::XMFLOAT3 colorRate = KazMath::Color(29, 19, 72, 255).ConvertColorRateToXMFLOAT3();
	objectData.fogData = { colorRate.x,colorRate.y,colorRate.z,fogColor.a };
	objectData.lightDir = INIT_DATA.lightDir.ConvertXMFLOAT3();
	boxRender->TransData(&objectData, objectBufferHandle, typeid(ObjectData).name());
}

void BlockMountain::Update()
{

	std::vector<MatData> lData(boxMaxNum);
	for (int i = 0; i < boxDataArray.size(); ++i)
	{
		boxDataArray[i].pos += -KazMath::Vec3<float>(0.0f, 0.0f, 5.0f);
		if (boxDataArray[i].pos.z <= -(70.0f * 100.0f * 2.2f) / 2.0f)
		{
			boxDataArray[i].pos.z = (70.0f * 100.0f * 2.2f) / 2.0f;
		}

		lData[i].mat = KazMath::CaluMat(boxDataArray[i], CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection(), { 0,1,0 }, { 0,0,1 });
		lData[i].color = { 0.32f, 0.32f, 0.54f,1.0f };
	}


	boxRender->TransData
	(
		lData.data(),
		instanceBufferHandle,
		typeid(MatData).name()
	);
}

void BlockMountain::Draw()
{
	boxRender->Draw();
}
