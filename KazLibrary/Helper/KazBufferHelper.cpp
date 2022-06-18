#include "KazBufferHelper.h"

KazBufferHelper::BufferResourceData KazBufferHelper::SetConstBufferData(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME)
{
	BufferResourceData data
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer((BUFFER_SIZE + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		BUFFER_NAME
	);
	return data;
}
KazBufferHelper::BufferResourceData KazBufferHelper::SetShaderResourceBufferData(const D3D12_RESOURCE_DESC &TEXTURE_DATA, const string &BUFFER_NAME)
{
	BufferResourceData data
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		TEXTURE_DATA,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		BUFFER_NAME
	);
	return data;
}
KazBufferHelper::BufferResourceData KazBufferHelper::SetVertexBufferData(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME)
{
	BufferResourceData data
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(BUFFER_SIZE),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		BUFFER_NAME
	);
	return data;
}
KazBufferHelper::BufferResourceData KazBufferHelper::SetIndexBufferData(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME)
{
	BufferResourceData data
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(BUFFER_SIZE),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		BUFFER_NAME
	);
	return data;
}

KazBufferHelper::BufferResourceData KazBufferHelper::SetRenderTargetData(const D3D12_HEAP_PROPERTIES &HEAP_PROPERTIES, const D3D12_RESOURCE_DESC &RESOURCE, D3D12_CLEAR_VALUE *CLEAR_COLOR, const string &BUFFER_NAME)
{
	KazBufferHelper::BufferResourceData data
	(
		HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		RESOURCE,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		CLEAR_COLOR,
		BUFFER_NAME
	);
	return data;
}

KazBufferHelper::BufferResourceData KazBufferHelper::SetStructureBuffer(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME)
{
	KazBufferHelper::BufferResourceData data
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(BUFFER_SIZE),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		BUFFER_NAME
	);
	return data;
}

KazBufferHelper::BufferResourceData KazBufferHelper::SetRWStructuredBuffer(const unsigned int &BUFFER_SIZE, const string &BUFFER_NAME)
{
	D3D12_HEAP_PROPERTIES prop{};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	prop.CreationNodeMask = 1;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	prop.Type = D3D12_HEAP_TYPE_CUSTOM;
	prop.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(BUFFER_SIZE);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;


	KazBufferHelper::BufferResourceData data
	(
		prop,
		D3D12_HEAP_FLAG_NONE,
		desc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		BUFFER_NAME
	);
	return data;
}

D3D12_VERTEX_BUFFER_VIEW KazBufferHelper::SetVertexBufferView(const D3D12_GPU_VIRTUAL_ADDRESS &GPU_ADDRESS, const unsigned int &BUFFER_SIZE, const unsigned int &ONE_VERTICES_SIZE)
{
	D3D12_VERTEX_BUFFER_VIEW view;
	view.BufferLocation = GPU_ADDRESS;
	view.SizeInBytes = BUFFER_SIZE;
	view.StrideInBytes = ONE_VERTICES_SIZE;
	return view;
}

D3D12_INDEX_BUFFER_VIEW KazBufferHelper::SetIndexBufferView(const D3D12_GPU_VIRTUAL_ADDRESS &GPU_ADDRESS, const unsigned int &BUFFER_SIZE)
{
	D3D12_INDEX_BUFFER_VIEW view;
	view.BufferLocation = GPU_ADDRESS;
	view.Format = DXGI_FORMAT_R16_UINT;
	view.SizeInBytes = BUFFER_SIZE;
	return view;
}