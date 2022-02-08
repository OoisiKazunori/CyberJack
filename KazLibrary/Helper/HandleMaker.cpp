#include "HandleMaker.h"
#include"../Helper/KazHelper.h"

HandleMaker::HandleMaker()
{
	handle = 0;
	setHandle = 0;
	maxSize = 10000;
}

short HandleMaker::GetHandle()
{
	//�n���h���̔ԍ�����
	if (1 <= deleteHandleNumber.size())
	{
		//������ꂽ�ꏊ�ɗD��I�Ɋm�ۂ���
		handle = deleteHandleNumber[deleteHandleNumber.size() - 1];
	}
	else
	{
		handle = setHandle;
		setHandle++;
	}


	//�z��̖�������m�ۂ��Ă����̂Ŗ�����������Ă���
	if (deleteHandleNumber.size() != 0)
	{
		deleteHandleNumber.pop_back();
	}

	//�n���h�����ő�T�C�Y�𒴂��Ă�����G���[�l��Ԃ�
	if (maxSize <= handle)
	{
		return -1;
	}

    return handle;
}

void HandleMaker::DeleteHandle(short HANDLE)
{
	//�n���h�����������ꂽ�ő�l�̓�&&�폜����Ă��Ȃ��l
	if (KazHelper::IsitInAnArray(HANDLE, setHandle) && !IsItDeleted(HANDLE))
	{	
		deleteHandleNumber.push_back(HANDLE);
	}
	else
	{
		ErrorCheck("�댯:���̃n���h���͊��ɍ폜�ς݂��A���X�����n���h���ł�");
	}
}

bool HandleMaker::CheckHandleWasDeleteOrNot(const short &HANDLE)
{
	for (int i = 0; i < deleteHandleNumber.size(); i++)
	{
		if (deleteHandleNumber[i] == HANDLE)
		{
			return true;
		}
	}
	return false;
}

bool HandleMaker::CheckHandleWasUsedOrNot(const short &HANDLE)
{
	if (HANDLE < handle)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void HandleMaker::SetHandleSize(const BufferMemorySize &SIZE)
{
	minSize = SIZE.startSize;
	maxSize = SIZE.endSize;

	setHandle = minSize;
}

short HandleMaker::CaluNowHandle(const short &HANDLE)
{
	return HANDLE - minSize;
}

bool HandleMaker::IsItDeleted(short HANDLE)
{
	for (int i = 0; i < deleteHandleNumber.size(); i++)
	{
		if (deleteHandleNumber[i] == HANDLE)
		{
			return true;
		}
	}
	return false;
}
