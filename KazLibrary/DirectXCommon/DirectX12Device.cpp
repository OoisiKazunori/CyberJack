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

	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++) {
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(adapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		result = D3D12CreateDevice(adapter.Get(), levels[i], IID_PPV_ARGS(&debugDev));

		if (result == S_OK) {
			//�f�o�C�X�̐����ɐ��������甲����
			featureLevel = levels[i];
			break;
		}
	}

	dev->SetName(L"Divece");

}
