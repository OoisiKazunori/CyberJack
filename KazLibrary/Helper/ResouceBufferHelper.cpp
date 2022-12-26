#include "ResouceBufferHelper.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"

ResouceBufferHelper::ResouceBufferHelper() :counterBufferData(
	CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
	D3D12_HEAP_FLAG_NONE,
	CD3DX12_RESOURCE_DESC::Buffer(sizeof(UINT)),
	D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
	nullptr,
	"CounterBuffer"
)
{
	//counterBufferData.resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	counterBufferData = KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT), "CounterBuffer");
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
			bufferArrayData[lHandle].counterWrapper.CreateBuffer(counterBufferData);

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
			bufferArrayData[lHandle].counterWrapper.CreateBuffer(counterBufferData);

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
			lBufferData.counterWrapper.CreateBuffer(counterBufferData);

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
			lBufferData.counterWrapper.CreateBuffer(counterBufferData);

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

RESOURCE_HANDLE ResouceBufferHelper::SetBuffer(const ComputeBufferHelper::BufferData &DATA, GraphicsRootParamType ROOTPARAM)
{
	RESOURCE_HANDLE lHandle = handle.GetHandle();
	if (lHandle <= bufferArrayData.size())
	{
		bufferArrayData.push_back(DATA);
	}
	else
	{
		bufferArrayData[lHandle] = DATA;
	}
	bufferArrayData[lHandle].rootParamType = ROOTPARAM;
	return lHandle;
}

void ResouceBufferHelper::TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE)
{
	bufferArrayData[HANDLE].bufferWrapper.TransData(TRANS_DATA, TRANSMISSION_DATA_SIZE);
}

void ResouceBufferHelper::StackToCommandListAndCallDispatch(ComputePipeLineNames NAME, const DispatchCallData &DISPATCH_DATA)
{
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(NAME);
	std::vector<RootSignatureParameter>lParamData = GraphicsRootSignature::Instance()->GetRootParam(static_cast<int>(NAME));

	for (int i = 0; i < bufferArrayData.size(); ++i)
	{
		const int L_PARAM = KazRenderHelper::SetBufferOnCmdList(lParamData, bufferArrayData[i].rangeType, bufferArrayData[i].rootParamType);

		//デスクリプタヒープにコマンドリストに積む。余りが偶数ならデスクリプタヒープだと判断する
		if (bufferArrayData[i].rangeType % 2 == 0)
		{
			DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(L_PARAM, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(bufferArrayData[i].viewHandle));
			continue;
		}

		//ビューで積む際はそれぞれの種類に合わせてコマンドリストに積む
		switch (bufferArrayData[i].rangeType)
		{
		case GRAPHICS_RANGE_TYPE_SRV_VIEW:
			DirectX12CmdList::Instance()->cmdList->SetComputeRootShaderResourceView(L_PARAM, bufferArrayData[i].bufferWrapper.GetGpuAddress());
			break;
		case GRAPHICS_RANGE_TYPE_UAV_VIEW:
			DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(L_PARAM, bufferArrayData[i].bufferWrapper.GetGpuAddress());
			break;
		case GRAPHICS_RANGE_TYPE_CBV_VIEW:
			DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(L_PARAM, bufferArrayData[i].bufferWrapper.GetGpuAddress());
			break;
		default:
			break;
		}
	}

	DirectX12CmdList::Instance()->cmdList->Dispatch(DISPATCH_DATA.x, DISPATCH_DATA.y, DISPATCH_DATA.z);
}

void ResouceBufferHelper::StackToCommandList(PipeLineNames NAME)
{
}

void ResouceBufferHelper::DeleteAllData()
{
	handle.DeleteAllHandle();
	bufferArrayData.clear();
	bufferArrayData.shrink_to_fit();
}

void ResouceBufferHelper::InitCounterBuffer()
{
	for (int i = 0; i < bufferArrayData.size(); ++i)
	{
		if (bufferArrayData[i].counterWrapper.buffer)
		{
			UINT lNum = 0;
			bufferArrayData[i].counterWrapper.TransData(&lNum, sizeof(UINT));
		}
	}
}

const ComputeBufferHelper::BufferData &ResouceBufferHelper::GetBufferData(RESOURCE_HANDLE HANDLE)
{
	return bufferArrayData[HANDLE];
}
