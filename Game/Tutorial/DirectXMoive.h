#pragma once
#include<mfapi.h>
#include<mfidl.h>
#include<mfreadwrite.h>

#include<filesystem>
#include<compare>
#include<string>
#include <mfapi.h>
#include <mfidl.h>
#include <mfmediaengine.h>
#include <stdexcept>

#include"../KazLibrary/Buffer/CreateGpuBuffer.h"

#include"../KazLibrary/Render/MovieRender.h"

/// <summary>
/// 動画公開のテスト
/// </summary>
class DirectXMoive
{
public:
	DirectXMoive();
	void Update();
	void Draw();
	void DecodeFrame();
	void UpdateTexture(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);

	void Play()
	{
		if (isPlaying)
		{
			return;
		}

		QueryPerformanceCounter(&startedTime);
		isPlaying = true;
	}

	bool TransferFrame()
	{
		LONGLONG pts;
		HRESULT hr = mediaEngine->OnVideoStreamTick(&pts);
		if (SUCCEEDED(hr))
		{
			Microsoft::WRL::ComPtr<ID3D11Texture2D> mediaTexture;
			hr = DirectX12Device::Instance()->dev11->OpenSharedResource(hSharedTexture, IID_PPV_ARGS(mediaTexture.GetAddressOf()));
			if (SUCCEEDED(hr))
			{
				MFVideoNormalizedRect rect{ 0.0f, 0.0f, 1.0f, 1.0f };
				RECT rcTarget{ 0, 0, LONG(width), LONG(height) };
				MFARGB m_bkgColor{ 0xFF, 0xFF, 0xFF, 0xFF };
				hr = mediaEngine->TransferVideoFrame(mediaTexture.Get(), &rect, &rcTarget, &m_bkgColor);
				if (SUCCEEDED(hr))
				{
					return true;
				}
			}
		}
	}


	void OnMediaEngineEvent(uint32_t meEvent)
	{
		switch (meEvent)
		{
		case MF_MEDIA_ENGINE_EVENT_LOADSTART:
			OutputDebugStringA("MF_MEDIA_ENGINE_EVENT_LOADSTART\n");
			break;
		case MF_MEDIA_ENGINE_EVENT_PROGRESS:
			OutputDebugStringA("MF_MEDIA_ENGINE_EVENT_PROGRESS\n");
			break;
		case MF_MEDIA_ENGINE_EVENT_LOADEDDATA:
			SetEvent(hPrepare);
			break;
		case MF_MEDIA_ENGINE_EVENT_PLAY:
			OutputDebugStringA("MF_MEDIA_ENGINE_EVENT_PLAY\n");
			break;

		case MF_MEDIA_ENGINE_EVENT_CANPLAY:
			break;

		case MF_MEDIA_ENGINE_EVENT_WAITING:
			OutputDebugStringA("MF_MEDIA_ENGINE_EVENT_WAITING\n");
			break;

		case MF_MEDIA_ENGINE_EVENT_TIMEUPDATE:
			break;
		case MF_MEDIA_ENGINE_EVENT_ENDED:
			OutputDebugStringA("MF_MEDIA_ENGINE_EVENT_ENDED\n");
			if (mediaEngine) {
				mediaEngine->Pause();
				isPlaying = false;
				isFinished = true;
			}
			break;

		case MF_MEDIA_ENGINE_EVENT_ERROR:
			if (mediaEngine) {
				Microsoft::WRL::ComPtr<IMFMediaError> err;
				if (SUCCEEDED(mediaEngine->GetError(&err))) {
					USHORT errCode = err->GetErrorCode();
					HRESULT hr = err->GetExtendedErrorCode();
					char buf[256] = { 0 };
					sprintf_s(buf, "ERROR: Media Foundation Event Error %u (%08X)\n", errCode, static_cast<unsigned int>(hr));
					OutputDebugStringA(buf);
				}
				else
				{
					OutputDebugStringA("ERROR: Media Foundation Event Error *FAILED GetError*\n");
				}
			}
			break;
		}
	}




private:
	IMFSourceReader *mfSourceReader;
	UINT width, height;
	double fps;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts;
	UINT64 rowPitchBytes = 0;

	struct MovieFrameInfo
	{
		double timestamp;
		int bufferIndex;
	};

	std::list<MovieFrameInfo>decorded;
	int writeBufferIndex;
	static const int DecodeBufferCount = 2;

	std::array<RESOURCE_HANDLE, DecodeBufferCount>frameDecoded;

	std::unique_ptr<CreateGpuBuffer>gpuBuffer;

	BufferMemorySize memorySize;
	RESOURCE_HANDLE movieTexHandle;
	MovieRender render;


	bool isPlaying;
	bool isFinished;
	bool isDecodeFinished;
	bool isLoop;
	LARGE_INTEGER startedTime;


	Microsoft::WRL::ComPtr<IMFDXGIDeviceManager>dxgiManager;
	Microsoft::WRL::ComPtr<IMFMediaEngine> mediaEngine;
	HANDLE hPrepare;
	HANDLE hSharedTexture;
	DXGI_FORMAT format;
	void InitMediaEngine();

};

/// <summary>
/// コールバック
/// </summary>
class MediaEngineNotify : public IMFMediaEngineNotify {
	long m_cRef;
	DirectXMoive *m_callback;
public:
	MediaEngineNotify() : m_cRef(0), m_callback(nullptr) { }
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {
		if (__uuidof(IMFMediaEngineNotify) == riid) {
			*ppv = static_cast<IMFMediaEngineNotify *>(this);
		}
		else {
			*ppv = nullptr;
			return E_NOINTERFACE;
		}
		AddRef();
		return S_OK;
	}
	STDMETHODIMP_(ULONG) AddRef() {
		return InterlockedIncrement(&m_cRef);
	}
	STDMETHODIMP_(ULONG) Release() {
		LONG cRef = InterlockedDecrement(&m_cRef);
		if (cRef == 0) { delete this; }
		return cRef;
	}
	void SetCallback(DirectXMoive *target) {
		m_callback = target;
	}
	STDMETHODIMP EventNotify(
		DWORD meEvent, DWORD_PTR PARAM1, DWORD_PTR PARAM2) {
		if (meEvent == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE) {
			SetEvent(reinterpret_cast<HANDLE>(PARAM1));
		}
		else {
			m_callback->OnMediaEngineEvent(meEvent);
		}
		return S_OK;
	}
};

