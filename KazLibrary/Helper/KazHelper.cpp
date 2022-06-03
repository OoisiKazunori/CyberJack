#include "KazHelper.h"
#include"../Camera/CameraMgr.h"

template<typename T>
void KazHelper::SaftyDelete(T *OBJECT)
{
	if (*OBJECT != nullptr)
	{
		delete OBJECT;
		OBJECT = nullptr;
	}
}

bool KazHelper::DirtyFlag(XMFLOAT3 VALUE, XMFLOAT3 OLD_VALUE)
{
	if (VALUE.x != OLD_VALUE.x||
		VALUE.y != OLD_VALUE.y||
		VALUE.z != OLD_VALUE.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KazHelper::DirtyFlag(XMFLOAT2 VALUE, XMFLOAT2 OLD_VALUE)
{
	if (VALUE.x != OLD_VALUE.x ||
		VALUE.y != OLD_VALUE.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KazHelper::DirtyFlag(float VALUE, float OLD_VALUE)
{
	if (VALUE != OLD_VALUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KazHelper::IsInScreen(XMFLOAT3 POS)
{
	XMVECTOR tmp = KazMath::LoadFloat3ToVector(POS);
	XMVECTOR tmp2 = KazMath::ConvertWorldPosToScreenPos(tmp, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	XMFLOAT3 screenPos = KazMath::LoadVecotrToXMFLOAT3(tmp2);

	bool outFlag = true;
	if (screenPos.x <= 0)
	{
		outFlag = false;
	}
	if (screenPos.y <= 0)
	{
		outFlag = false;
	}
	if (WIN_X <= screenPos.x)
	{
		outFlag = false;
	}
	if (WIN_Y <= screenPos.y)
	{
		outFlag = false;
	}
	return outFlag;
}

XMVECTOR KazHelper::GetScreenPos(XMFLOAT3 POS, XMFLOAT2 LEFTUP_POS)
{
	XMVECTOR tmp = KazMath::LoadFloat3ToVector(POS);
	XMVECTOR tmp2 = KazMath::ConvertWorldPosToScreenPos(tmp, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	XMFLOAT3 playerScreenPos = KazMath::LoadVecotrToXMFLOAT3(tmp2);

	//¶ãA‰Eã
	XMVECTOR leftUp =
		KazMath::ConvertScreenPosToWorldPos({ LEFTUP_POS.x,LEFTUP_POS.y,playerScreenPos.z }, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

	return leftUp;
}

XMFLOAT3 KazHelper::GetRightDownStagePos(XMFLOAT3 POS, XMFLOAT2 RIGHT_POS)
{
	XMVECTOR tmp = KazMath::LoadFloat3ToVector(POS);
	XMVECTOR tmp2 = KazMath::ConvertWorldPosToScreenPos(tmp, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	XMFLOAT3 playerScreenPos = KazMath::LoadVecotrToXMFLOAT3(tmp2);

	XMVECTOR rightDown =
		KazMath::ConvertScreenPosToWorldPos({ RIGHT_POS.x,RIGHT_POS.y,playerScreenPos.z }, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

	XMFLOAT3 result = KazMath::LoadVecotrToXMFLOAT3(rightDown);
	return result;
}

int KazHelper::GetDigit(int NUM)
{
	if (NUM == 0)return 1;

	int result = 0;
	while (NUM != 0)
	{
		NUM /= 10;
		result++;
	}
	return result;
}

std::vector<int> KazHelper::CountNumber(int TIME, int ZERO)
{
	float score = TIME;

	int haveZero = 0;

	//0–„‚ß‚·‚é
	if (ZERO != -1)
	{
		haveZero = ZERO;
	}
	else
	{
		haveZero = GetDigit(TIME);
	}

	std::vector<int> Number(haveZero);

	int tmp = score;
	for (int i = 0; tmp > 0; i++)
	{
		float result = tmp % 10;
		Number[i] = result;
		tmp /= 10.0f;
	}
	for (int i = 0; i < Number.size(); i++)
	{
		if (Number[i] == 0)
		{
			Number[i] = 0;
		}
	}
	std::reverse(Number.begin(), Number.end());
	return Number;
}


void KazHelper::ConvertStringToWchar_t(string STRING, wchar_t *WCHAR_STRING, int ARRAY_SIZE)
{
	MultiByteToWideChar(CP_ACP, 0, STRING.c_str(), -1, WCHAR_STRING, ARRAY_SIZE);
}
