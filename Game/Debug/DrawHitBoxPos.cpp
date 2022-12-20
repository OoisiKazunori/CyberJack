#include "DrawHitBoxPos.h"
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

DrawHitBoxPos::DrawHitBoxPos(const ComputeBufferHelper::BufferData &POS_HANDLE, UINT MAX_NUM) :countNum(MAX_NUM)
{
	computeHelper.SetBuffer(POS_HANDLE, GRAPHICS_PRAMTYPE_DATA);

	matHandle = computeHelper.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(sizeof(OutputData) * countNum, "DebugHitBoxMat"),
		GRAPHICS_RANGE_TYPE_UAV_VIEW,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(OutputData),
		countNum
	);

	cameraHandle = computeHelper.CreateBuffer(sizeof(CameraData), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA3, countNum);



	std::array<Vertex, 4>lVerticesArray;
	std::array<USHORT, 6> lIndicesArray;
	lIndicesArray = KazRenderHelper::InitIndciesForPlanePolygon();
	KazRenderHelper::InitVerticesPos(&lVerticesArray[0].pos, &lVerticesArray[1].pos, &lVerticesArray[2].pos, &lVerticesArray[3].pos, { 0.5f,0.5f });
	KazRenderHelper::InitUvPos(&lVerticesArray[0].uv, &lVerticesArray[1].uv, &lVerticesArray[2].uv, &lVerticesArray[3].uv);

	BUFFER_SIZE lVertBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lVerticesArray.size(), sizeof(Vertex));
	BUFFER_SIZE lIndexBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lIndicesArray.size(), sizeof(UINT));


	vertexBuffer = std::make_unique<KazRenderHelper::ID3D12ResourceWrapper>();
	indexBuffer = std::make_unique<KazRenderHelper::ID3D12ResourceWrapper>();

	vertexBuffer->CreateBuffer(KazBufferHelper::SetVertexBufferData(lVertBuffSize));
	indexBuffer->CreateBuffer(KazBufferHelper::SetIndexBufferData(lIndexBuffSize));

	vertexBuffer->TransData(lVerticesArray.data(), lVertBuffSize);
	indexBuffer->TransData(lIndicesArray.data(), lIndexBuffSize);

	//デバック用の描画-------------------------------------------------------------------------------------
	InitDrawIndexedExcuteIndirect lInitData;

	lInitData.vertexBufferView = KazBufferHelper::SetVertexBufferView(vertexBuffer->GetGpuAddress(), lVertBuffSize, sizeof(lVerticesArray[0]));
	lInitData.indexBufferView = KazBufferHelper::SetIndexBufferView(indexBuffer->GetGpuAddress(), lIndexBuffSize);
	lInitData.indexNum = static_cast<UINT>(lIndicesArray.size());
	lInitData.elementNum = countNum;
	lInitData.updateView = computeHelper.GetBufferData(matHandle).bufferWrapper.buffer->GetGPUVirtualAddress();
	lInitData.rootsignatureName = ROOTSIGNATURE_DATA_DRAW_UAV;

	std::array<D3D12_INDIRECT_ARGUMENT_DESC, 2> args{};
	args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
	args[0].UnorderedAccessView.RootParameterIndex = 0;
	args[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	lInitData.argument.push_back(args[0]);
	lInitData.argument.push_back(args[1]);
	excuteIndirect = std::make_unique<DrawExcuteIndirect>(lInitData);
	//デバック用の描画-------------------------------------------------------------------------------------
}

void DrawHitBoxPos::Update()
{
	DirectX::XMMATRIX lMatWorld = KazMath::CaluTransMatrix({ 0.0f,0.0f,0.0f }) * KazMath::CaluScaleMatrix({ 0.05f,0.05f,0.05f }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });
	CameraData lData;
	lData.scaleRotateBillboardMat = lMatWorld * CameraMgr::Instance()->GetMatBillBoard();
	lData.viewProjection = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
	lData.posMaxNum = countNum;
	computeHelper.TransData(cameraHandle, &lData, sizeof(CameraData));

	UINT lXthreadNum = countNum / 1024;
	if (lXthreadNum <= 0)
	{
		lXthreadNum = 1;
	}
	computeHelper.Compute(PIPELINE_COMPUTE_NAME_HITBOX_CALUMAT, { lXthreadNum,1,1 });
}

void DrawHitBoxPos::Draw()
{
	excuteIndirect->Draw(PIPELINE_NAME_GPUPARTICLE);
}
