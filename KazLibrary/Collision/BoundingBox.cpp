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
	////読み込んだモデルの情報から頂点座標を入手し、VRAMに保存する
	//vertBufferHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMFLOAT4) * static_cast<BUFFER_SIZE>(VERT_DATA.size())));

	//memcpy
	//(
	//	buffers.GetMapAddres(vertBufferHandle),
	//	VERT_DATA.data(),
	//	VERT_DATA.size() * sizeof(DirectX::XMFLOAT4)
	//);

	////BBを形成する処理用意
	//bbBufferHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(BoundingBoxBufferData)));

	//matBufferHandle = buffers.CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(DirectX::XMMATRIX)));

	//DirectX::XMMATRIX lMat = KazMath::CaluWorld(KazMath::Transform3D({ 0.0f,0.0f,0.0f }, { 10.0f,10.0f,10.0f }, { 0.0f,0.0f,0.0f }), { 0.0f,1.0f,0.0f }, { 0.0f, 0.0f, 1.0f });
	//buffers.TransData(matBufferHandle, &lMat, sizeof(DirectX::XMMATRIX));

	//bbViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	//DescriptorHeapMgr::Instance()->CreateBufferView(
	//	bbViewHandle,
	//	KazBufferHelper::SetUnorderedAccessView(sizeof(BoundingBoxBufferData), 1),
	//	buffers.GetBufferData(bbBufferHandle).Get(),
	//	nullptr
	//);

	//Compute();


	//読み込んだモデルの情報から頂点座標を入手し、VRAMに保存する
	bbBufferHandle = computeBuffer.CreateBuffer(sizeof(DirectX::XMFLOAT4) * static_cast<BUFFER_SIZE>(VERT_DATA.size()), GRAPHICS_RANGE_TYPE_UAV_VIEW, GRAPHICS_PRAMTYPE_DATA, static_cast<BUFFER_SIZE>(VERT_DATA.size()));
	computeBuffer.TransData(bbBufferHandle, VERT_DATA.data(), sizeof(DirectX::XMFLOAT4) * static_cast<BUFFER_SIZE>(VERT_DATA.size()));

	//BBを形成する処理用意
	bbBufferHandle = computeBuffer.CreateBuffer(sizeof(BoundingBoxBufferData), GRAPHICS_RANGE_TYPE_UAV_DESC, GRAPHICS_PRAMTYPE_DATA2, 1);
	bbViewHandle = computeBuffer.GetDescriptorViewHandle(bbBufferHandle);

	//モデルのTransform指定
	matBufferHandle = computeBuffer.CreateBuffer(sizeof(DirectX::XMMATRIX), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA3, 1);
	DirectX::XMMATRIX lMat = KazMath::CaluWorld(KazMath::Transform3D({ 0.0f,0.0f,0.0f }, { 10.0f,10.0f,10.0f }, { 0.0f,0.0f,0.0f }), { 0.0f,1.0f,0.0f }, { 0.0f, 0.0f, 1.0f });
	computeBuffer.TransData(matBufferHandle, &lMat, sizeof(DirectX::XMMATRIX));
}

BoundingBoxData BoundingBox::GetData()
{
	BoundingBoxBufferData *lData = (BoundingBoxBufferData *)computeBuffer.GetMapAddress(bbBufferHandle);

	BoundingBoxData lTmpData;
	lTmpData.minPos = { lData->minPos.x, lData->minPos.y, lData->minPos.z };
	lTmpData.maxPos = { lData->maxPos.x, lData->maxPos.y, lData->maxPos.z };
	return lTmpData;
}

D3D12_GPU_DESCRIPTOR_HANDLE BoundingBox::GetViewHandle()
{
	return DescriptorHeapMgr::Instance()->GetGpuDescriptorView(bbViewHandle);
}

void BoundingBox::Compute()
{
	computeBuffer.Compute(PIPELINE_COMPUTE_NAME_HITBOX_BB, { 2,1,1 });
}

void BoundingBox::DebugDraw()
{
}
