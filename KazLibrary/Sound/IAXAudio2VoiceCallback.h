#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

class XAudio2VoiceCallback :public IXAudio2VoiceCallback 
{
public:

	//ボイス処理パスの開始時
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 ByteRequired) {};
	//ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};

	//バッファストリームの再生が終了した時
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	//バッファの使用開始時
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	//バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) {
		//バッファ解放
		delete[] pBufferContext;
	};

	STDMETHOD_(void, OnLoopEnd)(THIS_ void *pBufferContext) {};
	STDMETHOD_(void, OnVoiceError)(THIS_ void *pBufferContext, HRESULT Error) {};

private:
};

