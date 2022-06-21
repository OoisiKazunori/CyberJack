#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"

/// <summary>
/// �f�o�C�X�𐶐��A�Ǘ����܂�
/// </summary>
class DirectX12Device:public ISingleton<DirectX12Device> 
{
public:
	DirectX12Device();
	~DirectX12Device();

	/// <summary>
	/// �f�o�C�X�̐���
	/// </summary>
	/// <param name="adapter">�A�_�v�^�[</param>
	void CreateDevice(ComPtr<IDXGIAdapter> adapter);
	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12DebugDevice1> debugDev;
private:
	HRESULT result;
	friend ISingleton<DirectX12Device>;
	
};

