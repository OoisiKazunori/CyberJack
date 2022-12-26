#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"

/// <summary>
/// ÉÇÉfÉãÇÃí∏ì_èÓïÒÇVRAMè„Ç…ï€ë∂Ç∑ÇÈ
/// </summary>
class CreateMeshBuffer
{
public:
	struct BufferHandle
	{
		RESOURCE_HANDLE bufferHandle;
		RESOURCE_HANDLE descriptorViewHandle;
	};

	enum MeshBufferView
	{
		DATA_VERT,
		DATA_UV,
		DATA_NORMAL,
		DATA_MAX
	};

	CreateMeshBuffer(RESOURCE_HANDLE HANDLE);
	const ComputeBufferHelper::BufferData &GetBufferData(MeshBufferView ENUM_VIEW);

private:
	std::array<BufferHandle, DATA_MAX>bufferHandleDataArray;

	ResouceBufferHelper computeHelper;
};

