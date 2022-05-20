#include "SecondStage.h"

SecondStage::SecondStage()
{
	stageParamLoader.LoadFile(KazFilePathName::StageParamPath + "StageParamData_SecondStage.json");

	if (false)
	{
		std::array <std::array<char, 6>, 50>data;
		for (int i = 0; i < line.size(); ++i)
		{
			data[i][0] = 'L';
			data[i][1] = 'i';
			data[i][2] = 'n';
			data[i][3] = 'e';
			if (i < 10)
			{
				data[i][4] = std::to_string(i)[0];
				data[i][5] = '\0';
			}
			else
			{
				data[i][4] = std::to_string(i)[0];
				data[i][5] = std::to_string(i)[1];
				data[i][6] = '\0';
			}
		}

		for (int i = 0; i < line.size(); ++i)
		{
			XMVECTOR startPos = line[i].data.startPos;
			XMVECTOR endPos = line[i].data.endPos;

			//Line毎のメンバ変数を追加
			rapidjson::Value startPosArray(rapidjson::kArrayType);
			rapidjson::Value endPosArray(rapidjson::kArrayType);
			for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
			{
				startPosArray.PushBack(rapidjson::Value(startPos.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
				endPosArray.PushBack(rapidjson::Value(endPos.m128_f32[axisIndex]), stageParamLoader.doc.GetAllocator());
			}

			//Lineオブジェクトにデータを追加
			rapidjson::Value object(rapidjson::kObjectType);
			object.AddMember("StartPos", startPosArray, stageParamLoader.doc.GetAllocator());
			object.AddMember("EndPos", endPosArray, stageParamLoader.doc.GetAllocator());
			stageParamLoader.doc.AddMember(rapidjson::GenericStringRef<char>(data[i].data()), object, stageParamLoader.doc.GetAllocator());
		}
		stageParamLoader.ExportFile(KazFilePathName::StageParamPath + "StageParamData_SecondStage.json");
	}
}

void SecondStage::Update()
{
	for (int i = 0; i < line.size(); ++i)
	{
		line[i].data;
	}
}

void SecondStage::Draw()
{
	for (int i = 0; i < line.size(); ++i)
	{
		line[i].Draw();
	}
}
