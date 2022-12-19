#include "ComputeBufferHelper.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"

ComputeBufferHelper::ComputeBufferHelper()
{

}

RESOURCE_HANDLE ComputeBufferHelper::CreateBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM)
{
	RESOURCE_HANDLE lHandle = 0;
	RESOURCE_HANDLE lViewHandle = 0;

	UINT lBufferSize = STRUCTURE_BYTE_STRIDE * ELEMENT_NUM;
	switch (RANGE)
	{
	case GRAPHICS_RANGE_TYPE_CBV_VIEW:
		lHandle = buffers.CreateBuffer
		(
			KazBufferHelper::SetConstBufferData(lBufferSize)
		);
		break;
	case GRAPHICS_RANGE_TYPE_UAV_VIEW:
		lHandle = buffers.CreateBuffer
		(
			KazBufferHelper::SetRWStructuredBuffer(lBufferSize)
		);
		break;
	case GRAPHICS_RANGE_TYPE_UAV_DESC:
		lHandle = buffers.CreateBuffer
		(
			KazBufferHelper::SetRWStructuredBuffer(lBufferSize)
		);

		lViewHandle = UavViewHandleMgr::Instance()->GetHandle();
		DescriptorHeapMgr::Instance()->CreateBufferView(
			lViewHandle,
			KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
			buffers.GetBufferData(lHandle).Get(),
			nullptr
		);
		break;
	}

	bufferArrayData.push_back({});
	bufferArrayData[lHandle].rangeType = RANGE;
	bufferArrayData[lHandle].rootParamType = ROOTPARAM;
	bufferArrayData[lHandle].bufferSize = lBufferSize;
	bufferArrayData[lHandle].bufferHandle = lHandle;
	bufferArrayData[lHandle].viewHandle = lViewHandle;

	return lHandle;
}

void ComputeBufferHelper::TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE)
{
	buffers.TransData(HANDLE, TRANS_DATA, TRANSMISSION_DATA_SIZE);
}

void *ComputeBufferHelper::GetMapAddress(RESOURCE_HANDLE HANDLE)
{
	return buffers.GetMapAddres(HANDLE);
}

RESOURCE_HANDLE ComputeBufferHelper::GetDescriptorViewHandle(RESOURCE_HANDLE HANDLE)
{
	return bufferArrayData[HANDLE].viewHandle;
}

void ComputeBufferHelper::Compute(ComputePipeLineNames NAME, const DispatchCallData &DATA)
{
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature2(NAME);

	std::vector<RootSignatureParameter>lParamData = GraphicsRootSignature::Instance()->GetRootParam(static_cast<int>(NAME));

	for (int i = 0; i < bufferArrayData.size(); ++i)
	{
		const UINT L_PARAM = KazRenderHelper::SetBufferOnCmdList(lParamData, bufferArrayData[i].rangeType, bufferArrayData[i].rootParamType);

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
			DirectX12CmdList::Instance()->cmdList->SetComputeRootShaderResourceView(L_PARAM, buffers.GetGpuAddress(bufferArrayData[i].bufferHandle));
			break;
		case GRAPHICS_RANGE_TYPE_UAV_VIEW:
			DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(L_PARAM, buffers.GetGpuAddress(bufferArrayData[i].bufferHandle));
			break;
		case GRAPHICS_RANGE_TYPE_CBV_VIEW:
			DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(L_PARAM, buffers.GetGpuAddress(bufferArrayData[i].bufferHandle));
			break;
		default:
			break;
		}
	}

	DirectX12CmdList::Instance()->cmdList->Dispatch(DATA.x, DATA.y, DATA.z);
}
