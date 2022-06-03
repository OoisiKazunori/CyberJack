#pragma once
#include"../DirectXCommon/Base.h"
#include"../Math/KazMath.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"

namespace KazHelper
{
	/// <summary>
	/// new���ꂽ�I�u�W�F�N�g�����S�ɍ폜���܂�
	/// </summary>
	/// <typeparam name="T">�e���v���[�g�^</typeparam>
	/// <param name="OBJECT">�폜�������I�u�W�F�N�g</param>
	template<typename T>
	void SaftyDelete(T *OBJECT);

	/// <summary>
	/// ���l���z��̍ő�𒴂��邩�ǂ������ׂ܂�
	/// </summary>
	/// <param name="NUM">���ׂ������l</param>
	/// <param name="MAX_SIZE">�z��̍ő吔</param>
	/// <returns>true...���̐��l��0~�z��̍ő吔�̓��ɓ���܂��Bfalse...���̐��l��0~�z��̍ő吔�̓��ɓ���܂���</returns>
	inline bool IsitInAnArray(short NUM,short MAX_SIZE)
	{
		if (0 <= NUM && NUM < MAX_SIZE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/// <summary>
	/// string�^�̕������wchar_t�^�̕�����ɕϊ����܂�
	/// </summary>
	/// <param name="STRING">string�^�̕�����</param>
	/// <param name="WCHAR_STRING">wchar_t�^�̕�����</param>
	/// <param name="ARRAY_SIZE">wchar_t�^�̕�����̍ő�̗v�f��</param>
	void ConvertStringToWchar_t(string STRING, wchar_t *WCHAR_STRING, int ARRAY_SIZE);

	/// <summary>
	/// ���t���[���l���ς�������ǂ����m�F���܂�
	/// </summary>
	/// <param name="VALUE">���݂̒l</param>
	/// <param name="OLD_VALUE">1Flame�O�̒l</param>
	/// <returns>true...�ω���������,false...�ω����Ȃ�����</returns>
	bool DirtyFlag(XMFLOAT3 VALUE, XMFLOAT3 OLD_VALUE);
	bool DirtyFlag(XMFLOAT2 VALUE, XMFLOAT2 OLD_VALUE);
	bool DirtyFlag(float VALUE, float OLD_VALUE);


	/// <summary>
	/// ���݂̃��[���h���W����ʓ��ɂ��邩�ǂ������܂�
	/// </summary>
	/// <param name="POS">���ׂ���W</param>
	/// <returns>true...��ʓ�,false...��ʊO</returns>
	bool IsInScreen(XMFLOAT3 POS);


	XMVECTOR GetScreenPos(XMFLOAT3 POS, XMFLOAT2 LEFTUP_POS = { 0.0f,0.0f });
	XMFLOAT3 GetRightDownStagePos(XMFLOAT3 POS, XMFLOAT2 RIGHT_POS = { WIN_X - 400,WIN_Y });


	int GetDigit(int NUM);

	std::vector<int> CountNumber(int TIME, int ZERO = -1);
}


