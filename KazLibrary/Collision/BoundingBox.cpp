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
	//�ǂݍ��񂾃��f���̏�񂩂璸�_���W����肵�AVRAM�ɕۑ�����
	vertBufferHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMFLOAT4) * static_cast<BUFFER_SIZE>(VERT_DATA.size())));

	memcpy
	(
		buffers.GetMapAddres(vertBufferHandle),
		VERT_DATA.data(),
		VERT_DATA.size() * sizeof(DirectX::XMFLOAT4)
	);

	//BB���`�����鏈���p��
	bbBufferHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(BoundingBoxBufferData)));

	matBufferHandle = buffers.CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(DirectX::XMMATRIX)));

	DirectX::XMMATRIX lMat = KazMath::CaluWorld(KazMath::Transform3D({ 0.0f,0.0f,0.0f }, { 10.0f,10.0f,10.0f }, { 0.0f,0.0f,0.0f }), { 0.0f,1.0f,0.0f }, { 0.0f, 0.0f, 1.0f });
	buffers.TransData(matBufferHandle, &lMat, sizeof(DirectX::XMMATRIX));

	Compute();
}

BoundingBoxData BoundingBox::GetData()
{
	BoundingBoxBufferData*lData = (BoundingBoxBufferData*)buffers.GetMapAddres(bbBufferHandle);

	BoundingBoxData lTmpData;
	lTmpData.minPos = { lData->minPos.x, lData->minPos.y, lData->minPos.z };
	lTmpData.maxPos = { lData->maxPos.x, lData->maxPos.y, lData->maxPos.z };
	return lTmpData;
}

D3D12_GPU_VIRTUAL_ADDRESS BoundingBox::GetViewHandle()
{
	return buffers.GetGpuAddress(bbBufferHandle);
}

void BoundingBox::Compute()
{
	//�v�Z����--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_BB);

	//���_���
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(0, buffers.GetGpuAddress(vertBufferHandle));
	//�o��
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(1, buffers.GetGpuAddress(bbBufferHandle));

	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers.GetGpuAddress(matBufferHandle));

	DirectX12CmdList::Instance()->cmdList->Dispatch(2, 1, 1);
	//�v�Z����--------------------------------------------
}

void BoundingBox::DebugDraw()
{
}
