#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../Game/Helper/ComputeBufferHelper.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"

struct DispatchCallData
{
	UINT x, y, z;
};

class ComputeBufferHelper
{
public:
	struct BufferData
	{
		GraphicsRangeType rangeType;
		GraphicsRootParamType rootParamType;
		UINT bufferSize;
		KazRenderHelper::ID3D12ResourceWrapper bufferWrapper;
		RESOURCE_HANDLE viewHandle;

		void operator=(const BufferData &rhs)
		{
			rangeType = rhs.rangeType;
			rootParamType = rhs.rootParamType;
			bufferSize = rhs.bufferSize;
			bufferWrapper = rhs.bufferWrapper;
			viewHandle = rhs.viewHandle;
		};
	};

	ComputeBufferHelper();

	RESOURCE_HANDLE CreateBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM);
	RESOURCE_HANDLE CreateBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM,UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM);
	void TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE);
	void Compute(ComputePipeLineNames NAME, const DispatchCallData &DATA);
	//積まれているデータの末尾に外部のデータをコピーしてハンドルを返す
	RESOURCE_HANDLE SetBuffer(const BufferData &BUFFER_DATA, GraphicsRootParamType RANGE);

	void *GetMapAddress(RESOURCE_HANDLE HANDLE);
	RESOURCE_HANDLE GetDescriptorViewHandle(RESOURCE_HANDLE HANDLE);

	const BufferData &GetBufferData(RESOURCE_HANDLE HANDLE);
private:
	HandleMaker handle;


	std::vector<BufferData>bufferArrayData;

};

