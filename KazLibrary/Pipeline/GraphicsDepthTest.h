#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"

//àÍå¬ÇµÇ©ê∂ê¨Ç≈Ç´Ç»Ç¢ÇÃÇ≈íçà”
class GraphicsDepthTest {
public:
	GraphicsDepthTest();
	~GraphicsDepthTest();

	RESOURCE_HANDLE CreateBuffer();
	void Clear(RESOURCE_HANDLE HANDLE);

	vector<D3D12_CPU_DESCRIPTOR_HANDLE> dsvH;
private:
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	vector<ComPtr<ID3D12Resource>> depthBuffer;

	HandleMaker handle;
};

