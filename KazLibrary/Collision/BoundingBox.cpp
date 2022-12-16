#include "BoundingBox.h"
#include"../KazLibrary/DirectXCommon/DirectX12Device.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Pipeline/GraphicsRootSignature.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"

BoundingBox::BoundingBox(std::vector<DirectX::XMFLOAT4> VERT_DATA)
{
	//読み込んだモデルの情報から頂点座標を入手し、VRAMに保存する
	vertBufferHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMFLOAT4) * static_cast<BUFFER_SIZE>(VERT_DATA.size())));

	memcpy
	(
		buffers.GetMapAddres(vertBufferHandle),
		VERT_DATA.data(),
		VERT_DATA.size() * sizeof(DirectX::XMFLOAT4)
	);

	//BBを形成する処理用意
	bbBufferHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(BoundingBoxBufferData)));
}

BoundingBoxData BoundingBox::GetData()
{
	BoundingBoxBufferData*lData = (BoundingBoxBufferData*)buffers.GetMapAddres(bbBufferHandle);

	BoundingBoxData lTmpData;
	lTmpData.minPos = { lData->minPos.x, lData->minPos.y, lData->minPos.z };
	lTmpData.maxPos = { lData->maxPos.x, lData->maxPos.y, lData->maxPos.z };
	return lTmpData;
}

void BoundingBox::Compute()
{
	//計算処理--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_BB);

	//頂点情報
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(0, buffers.GetGpuAddress(vertBufferHandle));
	//出力
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(1, buffers.GetGpuAddress(bbBufferHandle));

	DirectX12CmdList::Instance()->cmdList->Dispatch(2, 1, 1);
	//計算処理--------------------------------------------
}
