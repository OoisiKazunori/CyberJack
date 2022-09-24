#include "AttackLog.h"

AttackLog::AttackLog()
{
}

void AttackLog::Init()
{
	logIndex = 0;
}

void AttackLog::Update()
{
	for (int i = 0; i < logString.size(); ++i)
	{
		logString[i].Update();
	}
}

void AttackLog::Draw()
{
	for (int i = 0; i < logString.size(); ++i)
	{
		logString[i].Draw();
	}
}

void AttackLog::WriteLog(const std::string &STRING, float FONT_SIZE)
{
	logString[logIndex].Init(STRING, FONT_SIZE);
	++logIndex;
	if (logString.size() <= logIndex)
	{
		logIndex = 0;
	}
}
