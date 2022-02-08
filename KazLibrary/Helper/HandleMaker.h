#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/KazBufferHelper.h"

/// <summary>
/// �g�p���Ă��Ȃ��n���h����Ԃ��A�w��̃n���h�����폜�ł���N���X
/// </summary>
class HandleMaker
{
public:
	HandleMaker();

	/// <summary>
	///�Ԃ�l�Ńn���h����Ԃ��Ă���܂�
	/// </summary>
	/// <returns>�g�p���Ă��Ȃ��n���h��</returns>
	short GetHandle();

	/// <summary>
	/// �w��̃n���h�����폜���܂�
	/// </summary>
	/// <param name="HANDLE">�w��̃n���h��</param>
	void DeleteHandle(short HANDLE);


	bool CheckHandleWasDeleteOrNot(const short &HANDLE);
	bool CheckHandleWasUsedOrNot(const short &HANDLE);

	void SetHandleSize(const BufferMemorySize &SIZE);


	short CaluNowHandle(const short &HANDLE);

	short minSize, maxSize;
private:
	short handle;
	short setHandle;
	std::vector<short> deleteHandleNumber;

	/// <summary>
	/// �n���h�����ߋ��ɍ폜����Ă������ǂ����������܂�
	/// </summary>
	/// <param name="HANDLE">�n���h��</param>
	/// <returns>true...�폜����Ă��܂���,false...�폜����Ă܂���ł���</returns>
	bool IsItDeleted(short HANDLE);
};