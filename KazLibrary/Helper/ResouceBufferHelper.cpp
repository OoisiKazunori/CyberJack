#include "ResouceBufferHelper.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"

ResouceBufferHelper::ResouceBufferHelper()
{
}

RESOURCE_HANDLE ResouceBufferHelper::CreateBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG)
{
	RESOURCE_HANDLE lHandle = handle.GetHandle();
	RESOURCE_HANDLE lViewHandle = 0;
	UINT lBufferSize = STRUCTURE_BYTE_STRIDE * ELEMENT_NUM;

	bufferArrayData.push_back(ComputeBufferHelper::BufferData());
	bufferArrayData[lHandle].rangeType = RANGE;
	bufferArrayData[lHandle].rootParamType = ROOTPARAM;
	bufferArrayData[lHandle].bufferSize = lBufferSize;
	bufferArrayData[lHandle].viewHandle = lViewHandle;
	bufferArrayData[lHandle].elementNum = ELEMENT_NUM;

	switch (RANGE)
	{
	case GRAPHICS_RANGE_TYPE_CBV_VIEW:
		bufferArrayData[lHandle].bufferWrapper.CreateBuffer(KazBufferHelper::SetConstBufferData(lBufferSize));
		break;
	case GRAPHICS_RANGE_TYPE_UAV_VIEW:
		bufferArrayData[lHandle].bufferWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lBufferSize));
		break;
	case GRAPHICS_RANGE_TYPE_UAV_DESC:
		bufferArrayData[lHandle].bufferWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lBufferSize));

		bufferArrayData[lHandle].viewHandle = UavViewHandleMgr::Instance()->GetHandle();

		if (GENERATE_COUNTER_BUFFER_FLAG)
		{
			DescriptorHeapMgr::Instance()->CreateBufferView(
				bufferArrayData[lHandle].viewHandle,
				KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
				bufferArrayData[lHandle].bufferWrapper.buffer.Get(),
				bufferArrayData[lHandle].counterWrapper.buffer.Get()
			);
		}
		else
		{
			DescriptorHeapMgr::Instance()->CreateBufferView(
				bufferArrayData[lHandle].viewHandle,
				KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
				bufferArrayData[lHandle].bufferWrapper.buffer.Get(),
				nullptr
			);
		}
		break;
	}

	return lHandle;
}

RESOURCE_HANDLE ResouceBufferHelper::CreateBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG)
{
	RESOURCE_HANDLE lHandle = handle.GetHandle();

	bufferArrayData.push_back(ComputeBufferHelper::BufferData());
	bufferArrayData[lHandle].rangeType = RANGE;
	bufferArrayData[lHandle].rootParamType = ROOTPARAM;
	bufferArrayData[lHandle].bufferSize = STRUCTURE_BYTE_STRIDE * ELEMENT_NUM;
	bufferArrayData[lHandle].bufferWrapper.CreateBuffer(BUFFER_OPTION_DATA);

	switch (RANGE)
	{
	case GRAPHICS_RANGE_TYPE_UAV_DESC:
		bufferArrayData[lHandle].viewHandle = UavViewHandleMgr::Instance()->GetHandle();

		if (GENERATE_COUNTER_BUFFER_FLAG)
		{
			bufferArrayData[lHandle].counterWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT), "CounterBuffer"));
			UINT lInitNum = 0;
			bufferArrayData[lHandle].counterWrapper.TransData(&lInitNum, sizeof(UINT));

			DescriptorHeapMgr::Instance()->CreateBufferView(
				bufferArrayData[lHandle].viewHandle,
				KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
				bufferArrayData[lHandle].bufferWrapper.buffer.Get(),
				bufferArrayData[lHandle].counterWrapper.buffer.Get()
			);
		}
		else
		{
			DescriptorHeapMgr::Instance()->CreateBufferView(
				bufferArrayData[lHandle].viewHandle,
				KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
				bufferArrayData[lHandle].bufferWrapper.buffer.Get(),
				nullptr
			);
		}
		break;
	}

	return lHandle;
}

ComputeBufferHelper::BufferData ResouceBufferHelper::CreateAndGetBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG)
{
	ComputeBufferHelper::BufferData lBufferData;

	RESOURCE_HANDLE lViewHandle = -1;
	UINT lBufferSize = STRUCTURE_BYTE_STRIDE * ELEMENT_NUM;

	lBufferData.rangeType = RANGE;
	lBufferData.rootParamType = ROOTPARAM;
	lBufferData.bufferSize = lBufferSize;
	lBufferData.viewHandle = lViewHandle;
	lBufferData.elementNum = ELEMENT_NUM;

	switch (RANGE)
	{
	case GRAPHICS_RANGE_TYPE_CBV_VIEW:
		lBufferData.bufferWrapper.CreateBuffer(KazBufferHelper::SetConstBufferData(lBufferSize));
		break;
	case GRAPHICS_RANGE_TYPE_UAV_VIEW:
		lBufferData.bufferWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lBufferSize));
		break;
	case GRAPHICS_RANGE_TYPE_UAV_DESC:
		lBufferData.bufferWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lBufferSize));

		lBufferData.viewHandle = UavViewHandleMgr::Instance()->GetHandle();

		if (GENERATE_COUNTER_BUFFER_FLAG)
		{
			lBufferData.counterWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT), "CounterBuffer"));
			UINT lInitNum = 0;
			lBufferData.counterWrapper.TransData(&lInitNum, sizeof(UINT));

			DescriptorHeapMgr::Instance()->CreateBufferView(
				lBufferData.viewHandle,
				KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
				lBufferData.bufferWrapper.buffer.Get(),
				lBufferData.counterWrapper.buffer.Get()
			);
		}
		else
		{
			DescriptorHeapMgr::Instance()->CreateBufferView(
				lBufferData.viewHandle,
				KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
				lBufferData.bufferWrapper.buffer.Get(),
				nullptr
			);
		}
		break;
	}

	return lBufferData;
}

ComputeBufferHelper::BufferData ResouceBufferHelper::CreateAndGetBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG)
{
	RESOURCE_HANDLE lViewHandle = 0;
	UINT lBufferSize = STRUCTURE_BYTE_STRIDE * ELEMENT_NUM;

	ComputeBufferHelper::BufferData lBufferData;
	lBufferData.rangeType = RANGE;
	lBufferData.rootParamType = ROOTPARAM;
	lBufferData.bufferSize = lBufferSize;
	lBufferData.viewHandle = lViewHandle;
	lBufferData.elementNum = ELEMENT_NUM;

	switch (RANGE)
	{
	case GRAPHICS_RANGE_TYPE_CBV_VIEW:
		lBufferData.bufferWrapper.CreateBuffer(KazBufferHelper::SetConstBufferData(lBufferSize));
		break;
	case GRAPHICS_RANGE_TYPE_UAV_VIEW:
		lBufferData.bufferWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lBufferSize));
		break;
	case GRAPHICS_RANGE_TYPE_UAV_DESC:
		lBufferData.bufferWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(lBufferSize));

		lBufferData.viewHandle = UavViewHandleMgr::Instance()->GetHandle();

		if (GENERATE_COUNTER_BUFFER_FLAG)
		{
			lBufferData.counterWrapper.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT), "CounterBuffer"));
			UINT lInitNum = 0;
			lBufferData.counterWrapper.TransData(&lInitNum, sizeof(UINT));

			DescriptorHeapMgr::Instance()->CreateBufferView(
				lBufferData.viewHandle,
				KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
				lBufferData.bufferWrapper.buffer.Get(),
				lBufferData.counterWrapper.buffer.Get()
			);
		}
		else
		{
			DescriptorHeapMgr::Instance()->CreateBufferView(
				lBufferData.viewHandle,
				KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
				lBufferData.bufferWrapper.buffer.Get(),
				nullptr
			);
		}
		break;
	}

	return lBufferData;
}

void ResouceBufferHelper::TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE)
{
}
