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

DrawHitBoxPos::DrawHitBoxPos(D3D12_GPU_VIRTUAL_ADDRESS POS_HANDLE, UINT MAX_NUM)
{
	posViewHandle = POS_HANDLE;
	matHandle = buffers.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(OutputData), "DebugHitBoxMat"));
	cameraHandle = buffers.CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CameraData), "CameraData"));


	//デバック用の描画-------------------------------------------------------------------------------------
	InitExcuteIndirect lInitData;
	lInitData.elementNum = MAX_NUM;
	RESOURCE_HANDLE lSphereHandle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "sphere.fbx");
	lInitData.vertexBufferView = FbxModelResourceMgr::Instance()->GetResourceData(lSphereHandle)->vertexBufferView;
	lInitData.updateView = buffers.GetGpuAddress(matHandle);
	lInitData.rootsignatureName = ROOTSIGNATURE_DATA_DRAW_UAV;

	std::array<D3D12_INDIRECT_ARGUMENT_DESC, 2> args{};
	args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
	args[0].UnorderedAccessView.RootParameterIndex = 0;
	args[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	lInitData.argument.push_back(args[0]);
	lInitData.argument.push_back(args[1]);
	excuteIndirect = std::make_unique<DrawExcuteIndirect>(lInitData);
	//デバック用の描画-------------------------------------------------------------------------------------
}

void DrawHitBoxPos::Update()
{
	//計算処理--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_SETCIRCLE_IN_BB);

	//当たり判定座標
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, posViewHandle);
	//描画用
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(1, buffers.GetGpuAddress(matHandle));
	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers.GetGpuAddress(cameraHandle));

	DirectX12CmdList::Instance()->cmdList->Dispatch(1, 1, 1);
	//計算処理--------------------------------------------
}

void DrawHitBoxPos::Draw()
{
	excuteIndirect->Draw(PIPELINE_NAME_GPUPARTICLE);
}
