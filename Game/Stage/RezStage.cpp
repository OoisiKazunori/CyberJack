#include "RezStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Easing/easing.h"

RezStage::RezStage()
{
#pragma region Grid
	const float L_SPACE = 300.0f;
	const float L_Y = -150.0f;
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
#pragma endregion


#pragma region Cross
	{
		int lineWallIndex = 0;
		float height = 1000.0f;
		float width = 1000.0f;
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
			lData.fogdata = { 0.0f,0.0f,0.0f,0.001f };
			gridWallLineRender[i].TransData(&lData, lHandle, typeid(lData).name());
		}



		lineWallIndex = 0;
		const float L_WALL_Z_POS = 10000.0f;
		for (int i = 0; i < zWallTopLinePos.size(); ++i)
		{
			fIndex = static_cast<float>(i);
			float lineWidth = (width * 2.0f) / static_cast<float>(zWallTopLinePos.size());

			zWallTopLinePos[i][0].pos = { -width + lineWidth * fIndex,height,-L_WALL_Z_POS };
			zWallTopLinePos[i][1].pos = { -width + lineWidth * fIndex,height,L_WALL_Z_POS };

			zGridWallLineRender[lineWallIndex].data.startPos = zWallTopLinePos[i][0].pos;
			zGridWallLineRender[lineWallIndex].data.endPos = zWallTopLinePos[i][1].pos;
			lineWallIndex++;
		}

		for (int i = 0; i < zWallLeftLinePos.size(); ++i)
		{
			fIndex = static_cast<float>(i);
			float lineWidth = (width * 2.0f) / static_cast<float>(zWallLeftLinePos.size());
			zWallLeftLinePos[i][0].pos = { -width,height - lineWidth * fIndex,-L_WALL_Z_POS };
			zWallLeftLinePos[i][1].pos = { -width,height - lineWidth * fIndex,L_WALL_Z_POS };

			zGridWallLineRender[lineWallIndex].data.startPos = zWallLeftLinePos[i][0].pos;
			zGridWallLineRender[lineWallIndex].data.endPos = zWallLeftLinePos[i][1].pos;
			lineWallIndex++;
		}

		for (int i = 0; i < zWallRightLinePos.size(); ++i)
		{
			fIndex = static_cast<float>(i);
			float lineWidth = (width * 2.0f) / static_cast<float>(zWallRightLinePos.size());
			zWallRightLinePos[i][0].pos = { width,height - lineWidth * fIndex,-L_WALL_Z_POS };
			zWallRightLinePos[i][1].pos = { width,height - lineWidth * fIndex,L_WALL_Z_POS };

			zGridWallLineRender[lineWallIndex].data.startPos = zWallRightLinePos[i][0].pos;
			zGridWallLineRender[lineWallIndex].data.endPos = zWallRightLinePos[i][1].pos;
			lineWallIndex++;
		}


		for (int i = 0; i < zGridWallLineRender.size(); ++i)
		{
			zGridWallLineRender[i].data.pipelineName = PIPELINE_NAME_FOG_LINE;
			FogData lData;
			RESOURCE_HANDLE lHandle = zGridWallLineRender[i].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
			lData.fogdata = { 0.0f,0.0f,0.0f,0.001f };
			zGridWallLineRender[i].TransData(&lData, lHandle, typeid(lData).name());
		}
	}
#pragma endregion


	//KazMath::Color backGroundColor(28, 19, 72, 255);
	//KazMath::Color backGroundColor(0, 0, 0, 255);
	//InitBackGround(backGroundColor, backGroundColor, 5000.0f, 60.0f);

	float cWallSpace = 280.0f;
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

			KazMath::Color lColor(29, 19, 72, 255);
			DirectX::XMFLOAT3 colorRate = lColor.ConvertColorRateToXMFLOAT3();
			lData.fogdata = { colorRate.x,colorRate.y,colorRate.z,0.0006f };
			gridCentralWallLineRender[i][wallIndex].TransData(&lData, lHandle, typeid(lData).name());
		}
	}


	model.data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "test_game_model.obj");


	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		const float maxXPos = 4000.0f;
		const float minXPos = 1000.0f;
		if (KazMath::Rand<int>(2, 0))
		{
			floorObjectRender[i].objRender[0].data.transform.pos.x = KazMath::Rand<float>(-maxXPos, -minXPos);
		}
		else
		{
			floorObjectRender[i].objRender[0].data.transform.pos.x = KazMath::Rand<float>(maxXPos, minXPos);
		}
		floorObjectRender[i].objRender[0].data.transform.pos.z = KazMath::Rand<float>(10000, -100);

		const float lScaleRate = abs(floorObjectRender[i].objRender[0].data.transform.pos.x) / abs(maxXPos);
		const float lScaleMin = 100.0f;
		const float lScaleMax = (lScaleRate * 200.0f) + lScaleMin;
		floorObjectRender[i].objRender[0].data.transform.scale =
		{
			KazMath::Rand<float>(lScaleMax,lScaleMin),
			KazMath::Rand<float>(lScaleMax,lScaleMin),
			KazMath::Rand<float>(lScaleMax,lScaleMin)
		};
		floorObjectRender[i].initScale = floorObjectRender[i].objRender[0].data.transform.scale;
		floorObjectRender[i].objRender[0].data.transform.pos.y = gridFloorZLinePos[0][0].pos.y + floorObjectRender[i].objRender[0].data.transform.scale.y;


		floorObjectRender[i].objRender[0].data.color = { 255,255,255,255 };
		floorObjectRender[i].objRender[0].data.pipelineName = PIPELINE_NAME_FOG_COLOR;

		floorObjectRender[i].objRender[1].data.transform.pos = floorObjectRender[i].objRender[0].data.transform.pos + KazMath::Vec3<float>(0.0f, -(floorObjectRender[i].objRender[0].data.transform.scale.y * 2), 0.0f);
		floorObjectRender[i].objRender[1].data.transform.scale = floorObjectRender[i].objRender[0].data.transform.scale;
		floorObjectRender[i].objRender[1].data.color = { 255,0,0,255 };
		floorObjectRender[i].objRender[1].data.pipelineName = PIPELINE_NAME_FOG_COLOR;

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
}

void RezStage::Update()
{
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


	for (int i = 0; i < gridCentralWallLineRender.size(); ++i)
	{
		for (int wallIndex = 0; wallIndex < gridCentralWallLineRender[i].size(); ++wallIndex)
		{
			gridCentralWallLineRender[i][wallIndex].data.startPos.z += lVelZ;
			gridCentralWallLineRender[i][wallIndex].data.endPos.z += lVelZ;

			if (gridCentralWallLineRender[i][wallIndex].data.startPos.z <= -100.0f)
			{
				gridCentralWallLineRender[i][wallIndex].data.startPos.z = -100.0f + static_cast<float>(gridCentralWallLineRender.size()) * 400.0f;
				gridCentralWallLineRender[i][wallIndex].data.endPos.z = -100.0f + static_cast<float>(gridCentralWallLineRender.size()) * 400.0f;
			}
		}
	}



	int lineWallIndex = 0;
	float fIndex = 0.0f;
	float zInterval = 100.0f;
	for (int i = 0; i < wallTopLinePos.size(); ++i)
	{
		fIndex = static_cast<float>(i);
		wallTopLinePos[i][0].pos.z += lVelZ;
		wallTopLinePos[i][1].pos.z += lVelZ;

		if (wallTopLinePos[i][0].pos.z <= -100.0f)
		{
			wallTopLinePos[i][0].pos.z = -100.0f + static_cast<float>(wallTopLinePos.size()) * zInterval;
			wallTopLinePos[i][1].pos.z = -100.0f + static_cast<float>(wallTopLinePos.size()) * zInterval;
		}

		gridWallLineRender[lineWallIndex].data.startPos = wallTopLinePos[i][0].pos;
		gridWallLineRender[lineWallIndex].data.endPos = wallTopLinePos[i][1].pos;
		lineWallIndex++;
	}

	for (int i = 0; i < wallLeftLinePos.size(); ++i)
	{
		fIndex = static_cast<float>(i);
		wallLeftLinePos[i][0].pos.z += lVelZ;
		wallLeftLinePos[i][1].pos.z += lVelZ;

		if (wallLeftLinePos[i][0].pos.z <= -100.0f)
		{
			wallLeftLinePos[i][0].pos.z = -100.0f + static_cast<float>(wallLeftLinePos.size()) * zInterval;
			wallLeftLinePos[i][1].pos.z = -100.0f + static_cast<float>(wallLeftLinePos.size()) * zInterval;
		}

		gridWallLineRender[lineWallIndex].data.startPos = wallLeftLinePos[i][0].pos;
		gridWallLineRender[lineWallIndex].data.endPos = wallLeftLinePos[i][1].pos;
		lineWallIndex++;
	}

	for (int i = 0; i < wallRightLinePos.size(); ++i)
	{
		fIndex = static_cast<float>(i);
		wallRightLinePos[i][0].pos.z += lVelZ;
		wallRightLinePos[i][1].pos.z += lVelZ;

		if (wallRightLinePos[i][0].pos.z <= -100.0f)
		{
			wallRightLinePos[i][0].pos.z = -100.0f + static_cast<float>(wallRightLinePos.size()) * zInterval;
			wallRightLinePos[i][1].pos.z = -100.0f + static_cast<float>(wallRightLinePos.size()) * zInterval;
		}

		gridWallLineRender[lineWallIndex].data.startPos = wallRightLinePos[i][0].pos;
		gridWallLineRender[lineWallIndex].data.endPos = wallRightLinePos[i][1].pos;
		lineWallIndex++;
	}




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
		floorObjectRender[i].objRender[0].data.transform.pos.z += lVelZ;
		floorObjectRender[i].objRender[1].data.transform.pos.z += lVelZ;
		bool limitZLineFlag = floorObjectRender[i].objRender[0].data.transform.pos.z <= -100.0f;

		floorObjectRender[i].objRender[0].data.transform.scale.y = floorObjectRender[i].initScale.y + EasingMaker(Out, Cubic, scaleRate) * 50.0f;
		floorObjectRender[i].objRender[1].data.transform.scale.y = floorObjectRender[i].objRender[0].data.transform.scale.y;


		if (limitZLineFlag)
		{
			floorObjectRender[i].objRender[0].data.transform.pos.z = 8000.0f;
			floorObjectRender[i].objRender[1].data.transform.pos = floorObjectRender[i].objRender[0].data.transform.pos + KazMath::Vec3<float>(0.0f, -(floorObjectRender[i].objRender[0].data.transform.scale.y * 2), 0.0f);
		}
	}

}

void RezStage::Draw()
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
	for (int i = 0; i < gridWallLineRender.size(); ++i)
	{
		//gridWallLineRender[i].Draw();
	}

	for (int i = 0; i < floorObjectRender.size(); ++i)
	{
		for (int objIndex = 0; objIndex < floorObjectRender[i].objRender.size(); ++objIndex)
		{
			floorObjectRender[i].objRender[objIndex].Draw();
		}
	}


	for (int i = 0; i < gridCentralLineRender.size(); ++i)
	{
		//gridCentralLineRender[i].Draw();
	}

	for (int i = 0; i < gridCentralWallLineRender.size(); ++i)
	{
		for (int wallIndex = 0; wallIndex < gridCentralWallLineRender[i].size(); ++wallIndex)
		{
			//gridCentralWallLineRender[i][wallIndex].Draw();
		}
	}

	for (int i = 0; i < zGridWallLineRender.size(); ++i)
	{
		//zGridWallLineRender[i].Draw();
	}

	vaporWaveSunRender.Draw();


	ImGui::Begin("Model");
	ImGui::DragFloat("POS_X", &vaporWaveSunRender.data.transform.pos.x);
	ImGui::DragFloat("POS_Y", &vaporWaveSunRender.data.transform.pos.y);
	ImGui::DragFloat("POS_Z", &vaporWaveSunRender.data.transform.pos.z);
	ImGui::DragFloat("SCALE_X", &vaporWaveSunRender.data.transform.scale.x);
	ImGui::DragFloat("SCALE_Y", &vaporWaveSunRender.data.transform.scale.y);
	ImGui::End();

	model.data.transform.rotation.y = 180.0f;
	model.data.transform.rotation.z += 0.5f;
	//model.Draw();
	//DrawBackGround();


}