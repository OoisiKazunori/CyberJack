#include "RegisterNameWithData.h"
#include"../Helper/KazHelper.h"

RegisterNameWithData::RegisterNameWithData()
{
}

RegisterNameWithData::~RegisterNameWithData()
{
}

bool RegisterNameWithData::RegisterName(string NAME)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (name[i] == NAME)
		{
			string dangerString = "�댯:" + NAME + "��o�^���悤�Ƃ��܂������A�������O���������דo�^�ł��܂���ł���";
			ErrorCheck(dangerString.c_str());
			return false;
		}
	}

	name.push_back(NAME);
	return true;
}

bool RegisterNameWithData::UnRegisterName(string NAME)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (name[i] == NAME)
		{
			name.erase(name.begin() + i);
			return true;
		}
	}

	string dangerString = "�댯:" + NAME + "���폜���悤�Ƃ��܂������A���������׍폜�Ɏ��s���܂���";
	ErrorCheck(dangerString.c_str());
	return false;
}

int RegisterNameWithData::IsNameExists(string NAME)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (name[i] == NAME)
		{
			return i;
		}
	}
	string dangerString = "�댯:" + NAME + "���o�^����Ă��܂���";
	ErrorCheck(dangerString.c_str());
	return -1;
}

void RegisterNameWithData::UnRegisterName(int HANDLE)
{
	if (KazHelper::IsitInAnArray(HANDLE, name.size()))
	{
		name[HANDLE] = "";
	}
}
