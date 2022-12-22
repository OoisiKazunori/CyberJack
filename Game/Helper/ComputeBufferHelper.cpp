#include "ComputeBufferHelper.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"

ComputeBufferHelper::ComputeBufferHelper()
{

}

RESOURCE_HANDLE ComputeBufferHelper::CreateBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM)
{
	RESOURCE_HANDLE lHandle = handle.GetHandle();
	RESOURCE_HANDLE lViewHandle = 0;
	UINT lBufferSize = STRUCTURE_BYTE_STRIDE * ELEMENT_NUM;

	bufferArrayData.push_back(BufferData());
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
		DescriptorHeapMgr::Instance()->CreateBufferView(
			bufferArrayData[lHandle].viewHandle,
			KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
			bufferArrayData[lHandle].bufferWrapper.buffer.Get(),
			nullptr
		);
		break;
	}

	return lHandle;
}

RESOURCE_HANDLE ComputeBufferHelper::CreateBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM)
{
	RESOURCE_HANDLE lHandle = handle.GetHandle();

	bufferArrayData.push_back(BufferData());
	bufferArrayData[lHandle].rangeType = RANGE;
	bufferArrayData[lHandle].rootParamType = ROOTPARAM;
	bufferArrayData[lHandle].bufferSize = STRUCTURE_BYTE_STRIDE * ELEMENT_NUM;
	bufferArrayData[lHandle].bufferWrapper.CreateBuffer(BUFFER_OPTION_DATA);

	switch (RANGE)
	{
	case GRAPHICS_RANGE_TYPE_UAV_DESC:
		bufferArrayData[lHandle].viewHandle = UavViewHandleMgr::Instance()->GetHandle();
		DescriptorHeapMgr::Instance()->CreateBufferView(
			bufferArrayData[lHandle].viewHandle,
			KazBufferHelper::SetUnorderedAccessView(STRUCTURE_BYTE_STRIDE, ELEMENT_NUM),
			bufferArrayData[lHandle].bufferWrapper.buffer.Get(),
			nullptr
		);
		break;
	}

	return lHandle;
}

RESOURCE_HANDLE ComputeBufferHelper::SetBuffer(const ComputeBufferHelper::BufferData &BUFFER_DATA, GraphicsRootParamType RANGE)
{
	RESOURCE_HANDLE lHandle = handle.GetHandle();
	bufferArrayData.push_back(BufferData());
	bufferArrayData[lHandle] = BUFFER_DATA;
	bufferArrayData[lHandle].rootParamType = RANGE;
	return lHandle;
}

RESOURCE_HANDLE ComputeBufferHelper::CopyBuffer(const BufferData &BUFFER_DATA, GraphicsRootParamType RANGE)
{
	const RESOURCE_HANDLE L_HANDLE = CreateBuffer(
		BUFFER_DATA.bufferSize,
		BUFFER_DATA.rangeType,
		RANGE,
		BUFFER_DATA.viewHandle
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		BUFFER_DATA.bufferWrapper.buffer.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COPY_SOURCE
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		bufferArrayData[L_HANDLE].bufferWrapper.buffer.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COPY_DEST
	);

	DirectX12CmdList::Instance()->cmdList->CopyResource(BUFFER_DATA.bufferWrapper.buffer.Get(), bufferArrayData[L_HANDLE].bufferWrapper.buffer.Get());

	RenderTargetStatus::Instance()->ChangeBarrier(
		BUFFER_DATA.bufferWrapper.buffer.Get(),
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		bufferArrayData[L_HANDLE].bufferWrapper.buffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);

	return L_HANDLE;
}

void ComputeBufferHelper::TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE)
{
	bufferArrayData[HANDLE].bufferWrapper.TransData(TRANS_DATA, TRANSMISSION_DATA_SIZE);
}

void *ComputeBufferHelper::GetMapAddress(RESOURCE_HANDLE HANDLE)
{
	return bufferArrayData[HANDLE].bufferWrapper.GetMapAddres();
}

RESOURCE_HANDLE ComputeBufferHelper::GetDescriptorViewHandle(RESOURCE_HANDLE HANDLE)
{
	return bufferArrayData[HANDLE].viewHandle;
}

const ComputeBufferHelper::BufferData &ComputeBufferHelper::GetBufferData(RESOURCE_HANDLE HANDLE)
{
	return bufferArrayData[HANDLE];
}

void ComputeBufferHelper::Compute(ComputePipeLineNames NAME, const DispatchCallData &DATA)
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

	DirectX12CmdList::Instance()->cmdList->Dispatch(DATA.x, DATA.y, DATA.z);
}
