#include "SoundMgr.h"

SoundMgr::SoundMgr()
{
	//XAudioエンジンのインスタンスを生成
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//マスターボイスを生成
	xAudio2->CreateMasteringVoice(&masterVoice);
}

SoundData SoundMgr::SoundLoadWave(string FILENAME)
{
	ifstream file;

	file.open(FILENAME, ios_base::binary);
	assert(file.is_open());

	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}



	FormatChunk format = {};
	file.read((char *)&format, sizeof(ChunkHeader));

	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char *)&format.fmt, format.chunk.size);


	ChunkHeader data;
	file.read((char *)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		file.seekg(data.size, ios_base::cur);
		file.read((char *)&data, sizeof(data));
	}

	if (strncmp(data.id, "LIST ", 4) == 0)
	{
		file.seekg(data.size, ios_base::cur);
		file.read((char *)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0)
	{
		assert(0);
	}



	char *pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();

	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE *>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void SoundMgr::SoundUnload(SoundData *SOUNDDATA)
{
	delete[]SOUNDDATA->pBuffer;

	SOUNDDATA->pBuffer = 0;
	SOUNDDATA->bufferSize = 0;
	SOUNDDATA->wfex = {};
}

void SoundMgr::SoundPlayWave(const SoundData &SOUNDDATA, int ROOP)
{
	HRESULT result;

	IXAudio2SourceVoice *pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &SOUNDDATA.wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = SOUNDDATA.pBuffer;
	buf.AudioBytes = SOUNDDATA.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.LoopCount = ROOP;

	pSourceVoice->SetVolume(0.8f);

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}
