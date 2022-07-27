#include "RezStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"

RezStage::RezStage()
{
#pragma region Grid
	const float space = 500.0f;
	const float y = -800.0f;

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
		gridLineRender[lineRIndex].data.startPos = gridFloorZLinePos[i][0].pos;
		gridLineRender[lineRIndex].data.endPos = gridFloorZLinePos[i][1].pos;
		++lineRIndex;
	}
	for (int i = 0; i < gridFloorXLinePos.size(); ++i)
	{
		gridLineRender[lineRIndex].data.startPos = gridFloorXLinePos[i][0].pos;
		gridLineRender[lineRIndex].data.endPos = gridFloorXLinePos[i][1].pos;
		++lineRIndex;
	}

	for (int i = 0; i < gridLineRender.size(); ++i)
	{
		gridLineRender[i].data.color = { 255,255,255,255 };
		gridLineRender[i].data.pipelineName = PIPELINE_NAME_FOG_LINE;

		FogData lData;
		RESOURCE_HANDLE lHandle = gridLineRender[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		lData.fogdata = { 0.0f,0.0f,0.0f,0.0006f };
		gridLineRender[i].TransData(&lData, lHandle, typeid(lData).name());
	}
#pragma endregion


#pragma region Cross
	{
		int lineWallIndex = 0;
		float height = 200.0f;
		float width = 300.0f;
		float fIndex = 0.0f;
		float zInterval = 100.0f;
		for (int i = 0; i < wallTopLinePos.size(); ++i)
		{
			fIndex = static_cast<float>(i);
			wallTopLinePos[i][0].pos = { width,height,-100.0f + fIndex * zInterval };
			wallTopLinePos[i][1].pos = { -width,height,-100.0f + fIndex * zInterval };

			gridWallLineRender[lineWallIndex].data.startPos = wallTopLinePos[i][0].pos;
			gridWallLineRender[lineWallIndex].data.endPos = wallTopLinePos[i][1].pos;
			lineWallIndex++;
		}

		for (int i = 0; i < wallLeftLinePos.size(); ++i)
		{
			fIndex = static_cast<float>(i);
			wallLeftLinePos[i][0].pos = { -width,height,-100.0f + fIndex * zInterval };
			wallLeftLinePos[i][1].pos = { -width,-1000.0f,-100.0f + fIndex * zInterval };

			gridWallLineRender[lineWallIndex].data.startPos = wallLeftLinePos[i][0].pos;
			gridWallLineRender[lineWallIndex].data.endPos = wallLeftLinePos[i][1].pos;
			lineWallIndex++;
		}

		for (int i = 0; i < wallRightLinePos.size(); ++i)
		{
			fIndex = static_cast<float>(i);
			wallRightLinePos[i][0].pos = { width,height,-100.0f + fIndex * zInterval };
			wallRightLinePos[i][1].pos = { width,-1000.0f,-100.0f + fIndex * zInterval };

			gridWallLineRender[lineWallIndex].data.startPos = wallRightLinePos[i][0].pos;
			gridWallLineRender[lineWallIndex].data.endPos = wallRightLinePos[i][1].pos;
			lineWallIndex++;
		}

		for (int i = 0; i < gridWallLineRender.size(); ++i)
		{
			gridWallLineRender[i].data.pipelineName = PIPELINE_NAME_FOG_LINE;
			FogData lData;
			RESOURCE_HANDLE lHandle = gridWallLineRender[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
			lData.fogdata = { 0.0f,0.0f,0.0f,0.004f };
			gridWallLineRender[i].TransData(&lData, lHandle, typeid(lData).name());
		}
	}
#pragma endregion


	//KazMath::Color backGroundColor(28, 19, 72, 255);
	//KazMath::Color backGroundColor(0, 0, 0, 255);
	//InitBackGround(backGroundColor, backGroundColor, 5000.0f, 60.0f);

	float cWallSpace = 170.0f;
	gridCentralLineRender[0].data.startPos = { cWallSpace,-cWallSpace,10000.0f };
	gridCentralLineRender[0].data.endPos = { cWallSpace,-cWallSpace,-10000.0f };

	gridCentralLineRender[1].data.startPos = { -cWallSpace,-cWallSpace,10000.0f };
	gridCentralLineRender[1].data.endPos = { -cWallSpace,-cWallSpace,-10000.0f };

	gridCentralLineRender[2].data.startPos = { cWallSpace, cWallSpace,10000.0f };
	gridCentralLineRender[2].data.endPos = { cWallSpace, cWallSpace,-10000.0f };

	gridCentralLineRender[3].data.startPos = { -cWallSpace,cWallSpace,10000.0f };
	gridCentralLineRender[3].data.endPos = { -cWallSpace,cWallSpace,-10000.0f };

	for (int i = 0; i < gridCentralWallLineRender.size(); ++i)
	{
		//ã•”
		gridCentralWallLineRender[i][0].data.startPos = gridCentralLineRender[2].data.startPos;
		gridCentralWallLineRender[i][0].data.endPos = gridCentralLineRender[3].data.startPos;
		//‰º•”
		gridCentralWallLineRender[i][1].data.startPos = gridCentralLineRender[0].data.startPos;
		gridCentralWallLineRender[i][1].data.endPos = gridCentralLineRender[1].data.startPos;
		//¶
		gridCentralWallLineRender[i][2].data.startPos = gridCentralLineRender[1].data.startPos;
		gridCentralWallLineRender[i][2].data.endPos = gridCentralLineRender[3].data.startPos;
		//‰E
		gridCentralWallLineRender[i][3].data.startPos = gridCentralLineRender[0].data.startPos;
		gridCentralWallLineRender[i][3].data.endPos = gridCentralLineRender[2].data.startPos;

		float fIndex = static_cast<float>(i);
		float wallInterval = 400.0f;
		gridCentralWallLineRender[i][0].data.startPos.z = -100.0f + fIndex * wallInterval;
		gridCentralWallLineRender[i][0].data.endPos.z = -100.0f + fIndex * wallInterval;
		gridCentralWallLineRender[i][1].data.startPos.z = -100.0f + fIndex * wallInterval;
		gridCentralWallLineRender[i][1].data.endPos.z = -100.0f + fIndex * wallInterval;
		gridCentralWallLineRender[i][2].data.startPos.z = -100.0f + fIndex * wallInterval;
		gridCentralWallLineRender[i][2].data.endPos.z = -100.0f + fIndex * wallInterval;
		gridCentralWallLineRender[i][3].data.startPos.z = -100.0f + fIndex * wallInterval;
		gridCentralWallLineRender[i][3].data.endPos.z = -100.0f + fIndex * wallInterval;

	}

	for (int i = 0; i < gridCentralLineRender.size(); ++i)
	{
		gridCentralLineRender[i].data.pipelineName = PIPELINE_NAME_FOG_LINE;
		FogData lData;
		RESOURCE_HANDLE lHandle = gridCentralLineRender[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		lData.fogdata = { 0.0f,0.0f,0.0f,0.001f };
		gridCentralLineRender[i].TransData(&lData, lHandle, typeid(lData).name());
	}


	for (int i = 0; i < gridCentralWallLineRender.size(); ++i)
	{
		for (int wallIndex = 0; wallIndex < gridCentralWallLineRender[i].size(); ++wallIndex)
		{
			gridCentralWallLineRender[i][wallIndex].data.pipelineName = PIPELINE_NAME_FOG_LINE;
			FogData lData;
			RESOURCE_HANDLE lHandle = gridCentralWallLineRender[i][wallIndex].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
			lData.fogdata = { 0.0f,0.0f,0.0f,0.001f };
			gridCentralWallLineRender[i][wallIndex].TransData(&lData, lHandle, typeid(lData).name());
		}
	}
}

void RezStage::Update()
{
	float lVelZ = 5.0f;

	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		gridFloorZLinePos[i][0].pos.z += -lVelZ;
		gridFloorZLinePos[i][1].pos.z += -lVelZ;

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


	for (int i = 0; i < gridCentralWallLineRender.size(); ++i)
	{
		for (int wallIndex = 0; wallIndex < gridCentralWallLineRender[i].size(); ++wallIndex)
		{
			gridCentralWallLineRender[i][wallIndex].data.startPos.z += -lVelZ;
			gridCentralWallLineRender[i][wallIndex].data.endPos.z += -lVelZ;

			if (gridCentralWallLineRender[i][wallIndex].data.startPos.z <= -100.0f)
			{
				gridCentralWallLineRender[i][wallIndex].data.startPos.z = -100.0f + static_cast<float>(gridCentralWallLineRender.size()) * 400.0f;
				gridCentralWallLineRender[i][wallIndex].data.endPos.z = -100.0f + static_cast<float>(gridCentralWallLineRender.size()) * 400.0f;
			}
		}
	}

}

void RezStage::Draw()
{

	for (int i = 0; i < gridLineRender.size(); ++i)
	{
		gridLineRender[i].Draw();
	}
	for (int i = 0; i < gridWallLineRender.size(); ++i)
	{
		gridWallLineRender[i].Draw();
	}
	for (int i = 0; i < gridCentralLineRender.size(); ++i)
	{
		gridCentralLineRender[i].Draw();
	}

	for (int i = 0; i < gridCentralWallLineRender.size(); ++i)
	{
		for (int wallIndex = 0; wallIndex < gridCentralWallLineRender[i].size(); ++wallIndex)
		{
			gridCentralWallLineRender[i][wallIndex].Draw();
		}
	}
	//DrawBackGround();
}