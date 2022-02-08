#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include"../Helper/KazHelper.h"
#include"../Helper/KazBufferHelper.h"
#include"../Buffer/CreateGpuBuffer.h"

class DescriptorHeapMgr :public ISingleton<DescriptorHeapMgr>
{
public:
	DescriptorHeapMgr();
	~DescriptorHeapMgr();


	void CreateBufferView(const short &HANDLE, const D3D12_SHADER_RESOURCE_VIEW_DESC &BUFFER_VIEW, ID3D12Resource *ADDRESS);
	void CreateBufferView(const short &HANDLE, const D3D12_RENDER_TARGET_VIEW_DESC &BUFFER_VIEW, ID3D12Resource *ADDRESS);
	void Release(const short &HANDLE);


	const BufferMemorySize GetSize(const BufferMemory &TYPE);
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorView(const short &HANDLE);
	const D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptorView(const short &HANDLE);
	void SetDescriptorHeap();


	int GetType(const short &HANDLE);

private:

	//デスクリプタヒープの処理-----------------------------------------------------------------------------
	static const short DESCRIPTOR_HEAP_SIZE_MAX = 4096;
	ComPtr<ID3D12DescriptorHeap> heaps;
	array<BufferMemorySize, DESCRIPTORHEAP_MEMORY_MAX> heapMaxSizes;
	UINT64 shaderResourceHeapIncreSize;
	UINT64 rtvHeapIncreSize;
	array<bool, DESCRIPTOR_HEAP_SIZE_MAX> usedHandle;

	bool isSafeToReadThisHandle(const short &HANDLE)
	{
		return KazHelper::IsitInAnArray(HANDLE, DESCRIPTOR_HEAP_SIZE_MAX) && usedHandle[HANDLE];
	};
	bool isSafeToUseThisHandle(const short &HANDLE)
	{
		return KazHelper::IsitInAnArray(HANDLE, DESCRIPTOR_HEAP_SIZE_MAX) && !usedHandle[HANDLE];
	};
};

