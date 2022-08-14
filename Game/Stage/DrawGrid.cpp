#include "DrawGrid.h"

DrawGrid::DrawGrid()
{
	for (int i = 0; i < gridLineRender.size(); ++i)
	{
		gridLineRender[i].data.color = { 255,255,255,255 };
		gridLineRender[i].data.pipelineName = PIPELINE_NAME_FOG_LINE;

		FogData lData;
		RESOURCE_HANDLE lHandle = gridLineRender[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		KazMath::Color lColor(29, 19, 72, 255);
		DirectX::XMFLOAT3 colorRate = lColor.ConvertColorRateToXMFLOAT3();
		lData.fogdata = { colorRate.x,colorRate.y,colorRate.z,0.0006f };
		gridLineRender[i].TransData(&lData, lHandle, typeid(lData).name());
	}
}

void DrawGrid::Init(bool USE_FOR_FLOOR_FLAG, float SPACE, float BASE_POS)
{
	if (USE_FOR_FLOOR_FLAG)
	{
		InitFloor(SPACE, BASE_POS);
	}
	else
	{
		InitWall(SPACE, BASE_POS);
	}

	int lLineRIndex = 0;
	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		gridLineRender[lLineRIndex].data.startPos = gridFloorZLinePos[i][0].pos;
		gridLineRender[lLineRIndex].data.endPos = gridFloorZLinePos[i][1].pos;
		++lLineRIndex;
	}
	for (int i = 0; i < gridFloorXLinePos.size(); ++i)
	{
		gridLineRender[lLineRIndex].data.startPos = gridFloorXLinePos[i][0].pos;
		gridLineRender[lLineRIndex].data.endPos = gridFloorXLinePos[i][1].pos;
		++lLineRIndex;
	}
}

void DrawGrid::Update()
{
#pragma region Grid
	const float lVelZ = -5.0f;

	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		gridFloorZLinePos[i][0].pos.z += lVelZ;
		gridFloorZLinePos[i][1].pos.z += lVelZ;

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
#pragma endregion


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


#pragma region LightEffect

	KazMath::Vec3<float> vel = {};
	KazMath::Vec3<float> vel1 = {};
	if (floorFlag)
	{
		vel1= KazMath::Vec3<float>(-10.0f, 0.0f, 0.0f);
		vel = KazMath::Vec3<float>(0.0f, 0.0f, -10.0f);
	}
	else
	{
		vel1 = KazMath::Vec3<float>(0.0f, 10.0f, 0.0f);
		vel = KazMath::Vec3<float>(0.0f, -10.0f, 0.0f);
	}

	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		for (int limitPointIndex = 0; limitPointIndex < lightEffectGridFloorLineStartPos[i].size(); limitPointIndex++)
		{
			lightEffectGridFloorLineStartPos[i][limitPointIndex] = gridFloorZLinePos[i][0].pos + KazMath::Vec3<float>(static_cast<float>(limitPointIndex) * -10.0f, 0.0f, 0.0f);
			lightEffectGridFloorLineEndPos[i][limitPointIndex] = gridFloorZLinePos[i][1].pos + vel1 * static_cast<float>(limitPointIndex);
		}
	}

	for (int i = static_cast<int>(gridFloorZLinePos.size()); i < gridFloorZLinePos.size() + gridFloorXLinePos.size(); ++i)
	{
		for (int limitPointIndex = 0; limitPointIndex < lightEffectGridFloorLineStartPos[i].size(); limitPointIndex++)
		{
			lightEffectGridFloorLineStartPos[i][limitPointIndex] = gridFloorXLinePos[i - 50][0].pos + KazMath::Vec3<float>(0.0f, 0.0f, static_cast<float>(limitPointIndex) * -10.0f);
			lightEffectGridFloorLineEndPos[i][limitPointIndex] = gridFloorXLinePos[i - 50][1].pos + vel * static_cast<float>(limitPointIndex);
		}
	}






	if (maxTimer <= timer)
	{
		maxTimer = KazMath::Rand<int>(60 * 2, 60);
		int countTrue = 0;
		while (countTrue < lightEffectArray.size())
		{
			lightEffectIndex = KazMath::Rand<int>(static_cast<int>(lightEffectArray.size() - 1), 0);
			if (lightEffectArray[lightEffectIndex][0].IsFinish() && lightEffectInitFlagArray[lightEffectIndex])
			{
				break;
			}
			lightEffectInitFlagArray[lightEffectIndex] = true;
			++countTrue;
		}
		timer = 0;
	}
	else
	{
		++timer;
	}

	for (int squareIndex = 0; squareIndex < lightEffectArray[lightEffectIndex].size(); ++squareIndex)
	{
		if (lightEffectArray[lightEffectIndex][squareIndex].IsFinish())
		{
			std::vector<KazMath::Vec3<float> *>lPosArray;
			lPosArray.push_back(&lightEffectGridFloorLineStartPos[lightEffectIndex][squareIndex]);
			lPosArray.push_back(&lightEffectGridFloorLineEndPos[lightEffectIndex][squareIndex]);

			lightEffectArray[lightEffectIndex][squareIndex].Init(lPosArray, 60 * 10);
		}
	}

	for (int i = 0; i < lightEffectArray.size(); ++i)
	{
		for (int squareIndex = 0; squareIndex < lightEffectArray[i].size(); ++squareIndex)
		{
			if (lightEffectArray[i][squareIndex].IsFinish() && lightEffectInitFlagArray[i])
			{
				std::vector<KazMath::Vec3<float> *>lPosArray;
				lPosArray.push_back(&lightEffectGridFloorLineStartPos[i][squareIndex]);
				lPosArray.push_back(&lightEffectGridFloorLineEndPos[i][squareIndex]);
				lightEffectArray[i][squareIndex].Init(lPosArray, 60 * 10);
			}
		}
	}


	for (int i = 0; i < lightEffectArray.size(); ++i)
	{
		for (int squareIndex = 0; squareIndex < lightEffectArray[i].size(); ++squareIndex)
		{
			lightEffectArray[i][squareIndex].Update();
		}
	}

#pragma endregion

}

void DrawGrid::Draw()
{
	for (int i = 0; i < gridLineRender.size(); ++i)
	{
		KazMath::Color baseColor(115, 85, 140, 255);
		KazMath::Color flashColor(213, 5, 228, 255);
		KazMath::Color nowColor(0, 0, 0, 255);
		nowColor.color = (flashColor.color - baseColor.color);
		KazMath::Vec4<float> tmpFloatColor = baseColor.color.Float() + nowColor.color.Float() * scaleRate;
		gridLineRender[i].data.color.color = tmpFloatColor.Int();

		gridLineRender[i].Draw();
	}

	/*for (int i = 0; i < lightEffectArray.size(); ++i)
	{
		for (int squareIndex = 0; squareIndex < lightEffectArray[i].size(); ++squareIndex)
		{
			lightEffectArray[i][squareIndex].Draw();
		}
	}*/
}

void DrawGrid::InitFloor(float SPACE, float BASE_POS)
{
	floorFlag = true;
	const float L_SPACE = SPACE;
	const float L_Y = BASE_POS;
	const float L_X = -10000.0f;
	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		float index = static_cast<float>(i);
		gridFloorZLinePos[i][0].pos = { L_X,L_Y,-700.0f + index * L_SPACE };
		gridFloorZLinePos[i][1] = gridFloorZLinePos[i][0];
		gridFloorZLinePos[i][1].pos.x *= -1;
	}

	for (int i = 0; i < gridFloorXLinePos.size(); ++i)
	{
		float index = static_cast<float>(i);
		gridFloorXLinePos[i][0].pos = { L_X + index * L_SPACE,L_Y,-5000.0f };
		gridFloorXLinePos[i][1] = gridFloorXLinePos[i][0];
		gridFloorXLinePos[i][1].pos.z *= -1;
	}
}

void DrawGrid::InitWall(float SPACE, float BASE_POS)
{
	floorFlag = false;
	const float L_SPACE = SPACE;
	const float L_Y = -150.0f;
	const float L_X = BASE_POS;
	for (int i = 0; i < gridFloorZLinePos.size(); ++i)
	{
		float index = static_cast<float>(i);
		gridFloorZLinePos[i][0].pos = { L_X,10000.0f,-700.0f + index * L_SPACE };
		gridFloorZLinePos[i][1] = gridFloorZLinePos[i][0];
		gridFloorZLinePos[i][1].pos.y *= -1;
	}

	for (int i = 0; i < gridFloorXLinePos.size(); ++i)
	{
		float index = static_cast<float>(i);
		gridFloorXLinePos[i][0].pos = { L_X,L_Y + index * L_SPACE,-5000.0f };
		gridFloorXLinePos[i][1] = gridFloorXLinePos[i][0];
		gridFloorXLinePos[i][1].pos.z *= -1;
	}
}
