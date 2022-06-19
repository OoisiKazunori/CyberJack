#pragma once
#include"../DirectXCommon/Base.h"
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>
#include"../Helper/ISinglton.h"

struct ChunkHeader 
{
	char id[4];		//チャンク毎のID
	int32_t size;	//チャンクサイズ
};

struct RiffHeader
{
	ChunkHeader chunk;	//RIFF
	char type[4];	//WAVE
};

struct FormatChunk
{
	ChunkHeader chunk;	//fmt
	WAVEFORMATEX fmt; //波形フォーマット
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
	/// 音を読み込みます
	/// </summary>
	/// <param name="FILENAME">ファイル名</param>
	/// <returns>音データ</returns>
	SoundData SoundLoadWave(string FILENAME);

	/// <summary>
	/// 音データを削除します
	/// </summary>
	/// <param name="SOUNDDATA">削除したい音データ</param>
	void SoundUnload(SoundData *SOUNDDATA);

	/// <summary>
	/// 音を鳴らします
	/// </summary>
	/// <param name="XAUDIO2">Audio</param>
	/// <param name="SOUNDDATA">読み込んだ音データ</param>
	/// <param name="LOOP">ループさせるかどうか</param>
	void SoundPlayWave(const SoundData &SOUNDDATA, int LOOP = 0);

	ComPtr<IXAudio2> xAudio2;
	SoundData Shandle[5];
private:

	
	IXAudio2MasteringVoice *masterVoice;

};

