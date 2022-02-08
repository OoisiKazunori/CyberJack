#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

class XAudio2VoiceCallback :public IXAudio2VoiceCallback 
{
public:

	//�{�C�X�����p�X�̊J�n��
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 ByteRequired) {};
	//�{�C�X�����p�X�̏I����
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};

	//�o�b�t�@�X�g���[���̍Đ����I��������
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	//�o�b�t�@�̎g�p�J�n��
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	//�o�b�t�@�̖����ɒB������
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) {
		//�o�b�t�@���
		delete[] pBufferContext;
	};

	STDMETHOD_(void, OnLoopEnd)(THIS_ void *pBufferContext) {};
	STDMETHOD_(void, OnVoiceError)(THIS_ void *pBufferContext, HRESULT Error) {};

private:
};

