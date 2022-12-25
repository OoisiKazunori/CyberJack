#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"
#include"ComputeBufferHelper.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"

class ComputeBufferHelperPtr
{
public:

	ComputeBufferHelperPtr();

	RESOURCE_HANDLE CreateBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	RESOURCE_HANDLE CreateBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	ComputeBufferHelper::BufferData CreateAndGetBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	ComputeBufferHelper::BufferData CreateAndGetBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	void TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE);
	void Compute(ComputePipeLineNames NAME, const DispatchCallData &DATA);

	//積まれているデータの末尾に外部のデータをコピーしてハンドルを返す
	RESOURCE_HANDLE SetBuffer(const ComputeBufferHelper::BufferData *BUFFER_DATA, GraphicsRootParamType ROOTPARAM);
	//バッファの中身をコピーするのみで共有はしない
	RESOURCE_HANDLE CopyBuffer(const ComputeBufferHelper::BufferData &BUFFER_DATA, GraphicsRootParamType ROOTPARAM);

	void *GetMapAddress(RESOURCE_HANDLE HANDLE);
	RESOURCE_HANDLE GetDescriptorViewHandle(RESOURCE_HANDLE HANDLE);
	const ComputeBufferHelper::BufferData &GetBufferData(RESOURCE_HANDLE HANDLE);

	void ClearBuffer();
	void SetParamDataType(RESOURCE_HANDLE HANDLE, GraphicsRootParamType ROOTPARAM);

	void InitCounterBuffer(RESOURCE_HANDLE HANDLE);

private:
	HandleMaker handle;
	std::vector<const ComputeBufferHelper::BufferData*>bufferArrayData;


	ComputeBufferHelper::BufferData appendBuffer,copyAppendBuffer;
	ComputeBufferHelper::BufferData testBuffer, testBuffer2;
	ComputeBufferHelper::BufferData commonBuffer;

	RESOURCE_HANDLE appendHandle, testHandle, test2Handle;


	std::array<DirectX::XMFLOAT3, 5> buffArray;
	std::vector<ComputeBufferHelper::BufferData> buffVecArray;

	void SetBuffer(const ComputeBufferHelper::BufferData &BUFF);
	ComputeBufferHelper::BufferData CreateApendBuffer();

	ResouceBufferHelper resourceHelper;
};
