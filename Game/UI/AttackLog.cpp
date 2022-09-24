#include "AttackLog.h"

AttackLog::AttackLog()
{
}

void AttackLog::Init()
{
	logArrayIndex = 0;
	logDataIndex = 0;
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		logStringArray[i].logIndex = -1;
	}
}

void AttackLog::Update()
{
	//ログの廃棄
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		if (logStringArray[i].logIndex == -1)
		{
			logStringArray[i].log.Finalize();
			logStringArray[i].logIndex = -1;
		}
	}

	for (int i = 0; i < logStringArray.size(); ++i)
	{
		if (logStringArray[i].logIndex != -1)
		{
			logStringArray[i].log.Update(logStringArray[i].logIndex);
		}
	}
}

void AttackLog::Draw()
{
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		logStringArray[i].log.Draw();
	}
}

void AttackLog::WriteLog(const std::string &STRING, float FONT_SIZE)
{
	logStringArray[logArrayIndex].log.Init(KazMath::Vec2<float>(10.0f, 300.0f), STRING, FONT_SIZE);
	logStringArray[logArrayIndex].logIndex = logDataIndex;

	//ログの順番が最大値まで到達したらその最大値のままにする
	++logDataIndex;
	if (logStringArray.size() <= logDataIndex)
	{
		logDataIndex = static_cast<int>(logStringArray.size() - 1);
		//最大値まで行ったら全てのログのインデックスを一つ下げ、文字の座標をずらす
		for (int i = 0; i < logStringArray.size(); ++i)
		{
			--logStringArray[i].logIndex;
		}
	}

	//ログ配列の添え字が最大値まで到達したら最初のログを使い回す
	++logArrayIndex;
	if (logStringArray.size() <= logArrayIndex)
	{
		logArrayIndex = 0;
	}
}
