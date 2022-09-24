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
	std::array<String, 15>logString;
	int logIndex;
};

