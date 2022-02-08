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
	//ハンドルの番号決定
	if (1 <= deleteHandleNumber.size())
	{
		//解放された場所に優先的に確保する
		handle = deleteHandleNumber[deleteHandleNumber.size() - 1];
	}
	else
	{
		handle = setHandle;
		setHandle++;
	}


	//配列の末尾から確保していくので末尾から消していく
	if (deleteHandleNumber.size() != 0)
	{
		deleteHandleNumber.pop_back();
	}

	//ハンドルが最大サイズを超えていたらエラー値を返す
	if (maxSize <= handle)
	{
		return -1;
	}

    return handle;
}

void HandleMaker::DeleteHandle(short HANDLE)
{
	//ハンドルが生成された最大値の内&&削除されていない値
	if (KazHelper::IsitInAnArray(HANDLE, setHandle) && !IsItDeleted(HANDLE))
	{	
		deleteHandleNumber.push_back(HANDLE);
	}
	else
	{
		ErrorCheck("危険:そのハンドルは既に削除済みか、元々無いハンドルです");
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
