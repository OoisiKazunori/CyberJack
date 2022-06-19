#pragma once
#include"../DirectXCommon/Base.h"
#include"SoundMgr.h"
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>
#include"../Helper/ISinglton.h"
#include"../Helper/HandleMaker.h"


struct Sound
{
	string filePass;
	SoundData *soundData;
	IXAudio2SourceVoice *soundSorce;
};
/// <summary>
/// ���Đ��Ɋւ���N���X�ł�
/// </summary>
class SoundManager :public ISingleton<SoundManager>
{
public:
	SoundManager();	

	/// <summary>
	/// �S���̉����J�����܂�
	/// </summary>
	void Finalize();

	/// <summary>
	/// .wav�̉���ǂݍ��݂܂�
	/// </summary>
	/// <param name="FILE_PASS">�t�@�C���p�X</param>
	/// <returns>�n���h��</returns>
	RESOURCE_HANDLE LoadSoundMem(string FILE_PASS, bool BGM_FLAG = true);

	/// <summary>
	/// �����Đ����܂�
	/// </summary>
	/// <param name="HANDLE">�Đ����������f�[�^�̃n���h��</param>
	/// <param name="VOLUME">����</param>
	/// <param name="LOOP_FLAG">�������[�v�Đ����邩���Ȃ���</param>
	void PlaySoundMem(RESOURCE_HANDLE HANDLE, int VOLUME, bool LOOP_FLAG = false);

	void ChangeSoundMem(RESOURCE_HANDLE HANDLE, int VOLUME);
	/// <summary>
	/// ���y���~�߂܂�
	/// </summary>
	/// <param name="HANDLE">�~�߂������y�f�[�^�̃n���h��</param>
	void StopSoundMem(RESOURCE_HANDLE HANDLE);

	/// <summary>
	/// ���f�[�^�������[�X���܂�
	/// </summary>
	/// <param name="HANDLE">�����[�X���������f�[�^�̃n���h��</param>
	void ReleaseSoundMem(RESOURCE_HANDLE HANDLE);


private:
	HandleMaker handle;
	array<Sound, 30> masterSoundData;

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice *masterVoice;
};

