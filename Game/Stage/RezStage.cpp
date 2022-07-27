#include "RezStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"

RezStage::RezStage()
{
#pragma region Grid
	const float space = 500.0f;
	const float y = -150.0f;

	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		float index = static_cast<float>(i);
		gridFloorZLinePos[i][0].pos = { -5000.0f,y,-700.0f + index * space };
		gridFloorZLinePos[i][1] = gridFloorZLinePos[i][0];
		gridFloorZLinePos[i][1].pos.x *= -1;
	}

	for (int i = 0; i < gridFloorXLinePos.size(); ++i)
	{
		float index = static_cast<float>(i);
		gridFloorXLinePos[i][0].pos = { -5000.0f + index * space,y,-5000.0f };
		gridFloorXLinePos[i][1] = gridFloorXLinePos[i][0];
		gridFloorXLinePos[i][1].pos.z *= -1;
	}


	int lineRIndex = 0;
	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		lineR[lineRIndex].data.startPos = gridFloorZLinePos[i][0].pos;
		lineR[lineRIndex].data.endPos = gridFloorZLinePos[i][1].pos;
		++lineRIndex;
	}
	for (int i = 0; i < gridFloorXLinePos.size(); ++i)
	{
		lineR[lineRIndex].data.startPos = gridFloorXLinePos[i][0].pos;
		lineR[lineRIndex].data.endPos = gridFloorXLinePos[i][1].pos;
		++lineRIndex;
	}

	for (int i = 0; i < lineR.size(); ++i)
	{
		lineR[i].data.color = { 255,255,255,255 };
		lineR[i].data.pipelineName = PIPELINE_NAME_FOG_LINE;

		FogData lData;
		RESOURCE_HANDLE lHandle = lineR[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		lData.fogdata = { 1.0f,1.0f,1.0f,0.4f };
		lineR[i].TransData(&lData, lHandle, typeid(lData).name());
	}
#pragma endregion


#pragma region Cross

#pragma endregion
}

void RezStage::Update()
{

	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		gridFloorZLinePos[i][0].pos.z += -5.0f;
		gridFloorZLinePos[i][1].pos.z += -5.0f;

		if (gridFloorZLinePos[i][0].pos.z <= -700.0f)
		{
			gridFloorZLinePos[i][0].pos.z = -700.0f + static_cast<float>(gridFloorZLinePos.size()) * 300.0f;
			gridFloorZLinePos[i][1].pos.z = gridFloorZLinePos[i][0].pos.z;
		}
	}


	int lineRIndex = 0;
	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		lineR[lineRIndex].data.startPos = gridFloorZLinePos[i][0].pos;
		lineR[lineRIndex].data.endPos = gridFloorZLinePos[i][1].pos;
		++lineRIndex;
	}
}

void RezStage::Draw()
{
	for (int i = 0; i < lineR.size(); ++i)
	{
		lineR[i].Draw();
	}
}