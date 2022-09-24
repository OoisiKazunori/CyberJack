#pragma once
#include"../Game/UI/Letter.h"
#include<array>

class AttackLog
{
public:
	AttackLog();
	void Init();
	void Update();
	void Draw();

	void WriteLog(const std::string &STRING, float FONT_SIZE);
private:
	struct StringData
	{
		String log;
		int logIndex;
	};
	std::array<StringData, 15>logStringArray;
	int logArrayIndex;
	int logDataIndex;
};

