#pragma once
#include"../DirectXCommon/Base.h"

/// <summary>
/// ���O��o�^����N���X
/// </summary>
class RegisterNameWithData
{
public:
	RegisterNameWithData();
	~RegisterNameWithData();

	/// <summary>
	/// ���O��o�^���܂�
	/// �������O���������ꍇ�͓o�^���ꂸ�ɏo�͂Ɍx������������܂�</summary>
	/// <param name="NAME">�o�^���������O</param>
	/// <returns>true...�o�^�ɐ������܂����@false...�o�^�Ɏ��s���܂���</returns>
	bool RegisterName(string NAME);

	/// <summary>
	/// �o�^�������O���폜���܂�
	/// �폜���������O�����������ꍇ�͏o�͂Ɍx������������܂�
	/// </summary>
	/// <param name="NAME">�폜���������O</param>
	bool UnRegisterName(string NAME);

	/// <summary>
	/// ���O�����邩�ǂ����m�F���܂�
	/// </summary>
	/// <param name="NANME">�m�F���������O</param>
	/// <returns>0�ȏ�̒l...���O�𔭌����܂����B���Ԗڂ̒l���n���܂��@-1...���O�͌�����܂���ł���</returns>
	int IsNameExists(string NAME);

	/// <summary>
	/// �n���h������o�^�������O���폜���܂�
	/// </summary>
	/// <param name="HANDLE">�폜�������n���h��</param>
	void UnRegisterName(int HANDLE);

private:
	vector<string> name;
};

