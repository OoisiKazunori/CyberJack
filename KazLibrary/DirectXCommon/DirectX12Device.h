#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"

/// <summary>
/// デバイスを生成、管理します
/// </summary>
class DirectX12Device:public ISingleton<DirectX12Device> 
{
public:
	DirectX12Device();
	~DirectX12Device();

	/// <summary>
	/// デバイスの生成
	/// </summary>
	/// <param name="adapter">アダプター</param>
	void CreateDevice(ComPtr<IDXGIAdapter> adapter);
	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12DebugDevice1> debugDev;
private:
	HRESULT result;
	friend ISingleton<DirectX12Device>;
	
};

