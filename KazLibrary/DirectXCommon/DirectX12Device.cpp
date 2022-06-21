#include "DirectX12Device.h"

//HRESULT DirectX12Device::result = S_FALSE;
//ComPtr<ID3D12Device> DirectX12Device::dev;

DirectX12Device::DirectX12Device()
{
	result = S_FALSE;
}

DirectX12Device::~DirectX12Device()
{
	dev.ReleaseAndGetAddressOf();
}

void DirectX12Device::CreateDevice(ComPtr<IDXGIAdapter> adapter) {

	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++) {
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(adapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		result = D3D12CreateDevice(adapter.Get(), levels[i], IID_PPV_ARGS(&debugDev));

		if (result == S_OK) {
			//デバイスの生成に成功したら抜ける
			featureLevel = levels[i];
			break;
		}
	}

	dev->SetName(L"Divece");

}
