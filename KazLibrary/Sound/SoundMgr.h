#pragma once
#include"../DirectXCommon/Base.h"
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>
#include"../Helper/ISinglton.h"

struct ChunkHeader 
{
	char id[4];		//�`�����N����ID
	int32_t size;	//�`�����N�T�C�Y
};

struct RiffHeader
{
	ChunkHeader chunk;	//RIFF
	char type[4];	//WAVE
};

struct FormatChunk
{
	ChunkHeader chunk;	//fmt
	WAVEFORMATEX fmt; //�g�`�t�H�[�}�b�g
};

struct SoundData 
{
	WAVEFORMATEX wfex;
	BYTE *pBuffer;
	unsigned int bufferSize;
};

class SoundMgr:public ISingleton<SoundMgr>
{
public:
	SoundMgr();

	/// <summary>
	/// ����ǂݍ��݂܂�
	/// </summary>
	/// <param name="FILENAME">�t�@�C����</param>
	/// <returns>���f�[�^</returns>
	SoundData SoundLoadWave(string FILENAME);

	/// <summary>
	/// ���f�[�^���폜���܂�
	/// </summary>
	/// <param name="SOUNDDATA">�폜���������f�[�^</param>
	void SoundUnload(SoundData *SOUNDDATA);

	/// <summary>
	/// ����炵�܂�
	/// </summary>
	/// <param name="XAUDIO2">Audio</param>
	/// <param name="SOUNDDATA">�ǂݍ��񂾉��f�[�^</param>
	/// <param name="LOOP">���[�v�����邩�ǂ���</param>
	void SoundPlayWave(const SoundData &SOUNDDATA, int LOOP = 0);

	ComPtr<IXAudio2> xAudio2;
	SoundData Shandle[5];
private:

	
	IXAudio2MasteringVoice *masterVoice;

};

