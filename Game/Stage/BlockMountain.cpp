#include "BlockMountain.h"
#include"../KazLibrary/Imgui/MyImgui.h"

BlockMountain::BlockMountain() :boxRender(std::make_unique<BoxPolygonRender>(true, BOX_MAX_NUM))
{
}

void BlockMountain::Init()
{
	boxRender->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR_MULTITEX_SHADING;
	bloomBufferHandle = boxRender->CreateConstBuffer(sizeof(BloomData), typeid(BloomData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA2);







	instanceBufferHandle = boxRender->CreateConstBuffer(sizeof(MatData) * BOX_MAX_NUM, typeid(MatData).name(), GRAPHICS_RANGE_TYPE_UAV, GRAPHICS_PRAMTYPE_DATA);


	const float L_SCALE = 100.0f;
	const int X_MAX_NUM = 40;
	const int Z_MAX_NUM = 40;
	const KazMath::Vec3<float>L_CENTRAL_POS(-(static_cast<float>(X_MAX_NUM) * (L_SCALE * 2.0f)) / 2.0f, -1000.0f, -(static_cast<float>(Z_MAX_NUM) * (L_SCALE * 2.0f)) / 2.0f);

	//const float L_X_SIZE = 1000.0f;
	//const float L_Z_SIZE = 1000.0f;

	int lBoxIndex = 0;
	for (int x = 0; x < X_MAX_NUM; ++x)
	{
		for (int z = 0; z < Z_MAX_NUM; ++z)
		{
			boxDataArray[lBoxIndex].pos =
			{
				L_CENTRAL_POS.x + static_cast<float>(x) * (L_SCALE * 2.2f),
				L_CENTRAL_POS.y,
				L_CENTRAL_POS.z + static_cast<float>(z) * (L_SCALE * 2.2f),
			};

			boxDataArray[lBoxIndex].scale = { L_SCALE ,KazMath::Rand(500.0f,50.0f),L_SCALE };
			++lBoxIndex;
		}
	}

	lBloomData.luminanceColor = { 0.0f,0.0f,0.0f,1.0f };
	DirectX::XMFLOAT3 colorRate = KazMath::Color(29, 19, 72, 255).ConvertColorRateToXMFLOAT3();
	lBloomData.fogData = { colorRate.x,colorRate.y,colorRate.z,0.001f };

	basePosY = -340.000;
}

void BlockMountain::Update()
{
	DirectX::XMFLOAT3 colorRate = KazMath::Color(29, 19, 72, 255).ConvertColorRateToXMFLOAT3();
	lBloomData.fogData.x = colorRate.x;
	lBloomData.fogData.y = colorRate.y;
	lBloomData.fogData.z = colorRate.z;

	ImGui::Begin("Block");
	KazImGuiHelper::InputVec4("Color", &fogColor);
	ImGui::DragFloat("BaseY", &basePosY);
	ImGui::End();

	//lBloomData.fogData.w = fogColor.a;
	boxRender->TransData(&lBloomData, bloomBufferHandle, typeid(BloomData).name());


	std::array<MatData, BOX_MAX_NUM> lData;
	for (int i = 0; i < boxDataArray.size(); ++i)
	{
		boxDataArray[i].pos.y = -1000.0f + basePosY;

		boxDataArray[i].pos += -KazMath::Vec3<float>(0.0f, 0.0f, 5.0f);
		if (boxDataArray[i].pos.z <= -(40.0f * 100.0f * 2.2f) / 2.0f)
		{
			boxDataArray[i].pos.z = (40.0f * 100.0f * 2.2f) / 2.0f;
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
