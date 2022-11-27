#include "SplineParticle.h"
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

SplineParticle::SplineParticle(std::vector<DirectX::XMFLOAT4> VERT_NUM, float PARTICLE_SCALE)
{
	buffers = std::make_unique<CreateGpuBuffer>();


	//コマンドシグネチャ---------------------------
	std::array<D3D12_INDIRECT_ARGUMENT_DESC, 2> args{};
	args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
	args[0].UnorderedAccessView.RootParameterIndex = 0;
	args[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	D3D12_COMMAND_SIGNATURE_DESC desc{};
	desc.pArgumentDescs = args.data();
	desc.NumArgumentDescs = static_cast<UINT>(args.size());
	desc.ByteStride = sizeof(IndirectCommand);

	HRESULT lR =
		DirectX12Device::Instance()->dev->CreateCommandSignature(&desc, GraphicsRootSignature::Instance()->GetRootSignature(ROOTSIGNATURE_DATA_DRAW_UAV).Get(), IID_PPV_ARGS(&commandSig));
	//コマンドシグネチャ---------------------------
	if (lR != S_OK)
	{
		assert(0);
	}

	std::array<Vertex, 4>vertices;
	std::array<USHORT, 6> indices;
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, { 0.5f,0.5f });
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

	BUFFER_SIZE vertBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vertices.size(), sizeof(Vertex));
	BUFFER_SIZE indexBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(indices.size(), sizeof(unsigned int));

	//バッファ生成-------------------------
	//初期化用--------
	//描画情報
	vertexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetVertexBufferData(vertBuffSize));
	indexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetIndexBufferData(indexBuffSize));
	verticesDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMFLOAT4) * static_cast<UINT>(VERT_NUM.size())));

	//共通情報
	initCommonHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(InitCommonData)));

	//出力
	outputHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputData) * PARTICLE_MAX_NUM)));
	//初期化用--------

	//更新用
	updateHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(UpdateData) * PARTICLE_MAX_NUM)));
	updateCommonHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(UpdateCommonData)));
	updateLimitPosDataHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(UpdateLimitPosData)));

	//描画用
	drawCommandHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(IndirectCommand) * DRAW_CALL));

	//バッファ生成-------------------------

	//転送-------------------------
	buffers->TransData(vertexBufferHandle, vertices.data(), vertBuffSize);
	buffers->TransData(indexBufferHandle, indices.data(), indexBuffSize);

	{
		//頂点情報
		memcpy
		(
			buffers->GetMapAddres(verticesDataHandle),
			VERT_NUM.data(),
			VERT_NUM.size() * sizeof(DirectX::XMFLOAT4)
		);
	}

	IndirectCommand command;
	command.drawArguments.IndexCountPerInstance = static_cast<UINT>(indices.size());
	command.drawArguments.InstanceCount = PARTICLE_MAX_NUM;
	command.drawArguments.StartIndexLocation = 0;
	command.drawArguments.StartInstanceLocation = 0;
	command.drawArguments.BaseVertexLocation = 0;

	command.uav = buffers->GetGpuAddress(updateHandle);
	buffers->TransData(drawCommandHandle, &command, sizeof(IndirectCommand) * DRAW_CALL);
	//転送-------------------------

	outputViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputHandle).Get(),
		nullptr
	);

	vertDataViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		vertDataViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(DirectX::XMFLOAT4), static_cast<UINT>(VERT_NUM.size())),
		buffers->GetBufferData(verticesDataHandle).Get(),
		nullptr
	);

	updateViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		updateViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(UpdateData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(updateHandle).Get(),
		nullptr
	);

	vertexBufferView = KazBufferHelper::SetVertexBufferView(buffers->GetGpuAddress(vertexBufferHandle), vertBuffSize, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(buffers->GetGpuAddress(indexBufferHandle), indexBuffSize);


	resetSceneFlag = false;

	scale = PARTICLE_SCALE;
	scaleRotaMat = KazMath::CaluScaleMatrix({ scale,scale,scale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });


	constBufferData.initMaxIndex = 6;
	buffers->TransData(initCommonHandle, &constBufferData, sizeof(InitCommonData));

	//初期化処理--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_SPLINEPARTICLE_INIT);

	//頂点
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(vertDataViewHandle));
	//出力
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
	//共通用バッファのデータ送信
	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(initCommonHandle));
	DirectX12CmdList::Instance()->cmdList->Dispatch(100, 1, 1);
	//初期化処理--------------------------------------------


	limitPosData.limitPos[0] = { 0.0f,0.0f,0.0f,0.0f };
	limitPosData.limitPos[1] = { 0.0f,0.0f,0.0f,0.0f };
	limitPosData.limitPos[2] = { 100.0f,0.0f,200.0f,0.0f };
	limitPosData.limitPos[3] = { -100.0f,0.0f,400.0f,0.0f };
	limitPosData.limitPos[4] = { 100.0f,0.0f,600.0f,0.0f };
	limitPosData.limitPos[5] = { -100.0f,0.0f,800.0f,0.0f };
	limitPosData.limitIndexMaxNum = 6;
	buffers->TransData(updateLimitPosDataHandle, &limitPosData, sizeof(UpdateLimitPosData));
}

void SplineParticle::Init()
{
}

void SplineParticle::Update(RESOURCE_HANDLE HANDLE)
{
	ImGui::Begin("Spline");
	KazImGuiHelper::InputXMFLOAT4("1", &limitPosData.limitPos[1]);
	KazImGuiHelper::InputXMFLOAT4("2", &limitPosData.limitPos[2]);
	KazImGuiHelper::InputXMFLOAT4("3", &limitPosData.limitPos[3]);
	KazImGuiHelper::InputXMFLOAT4("4", &limitPosData.limitPos[4]);
	KazImGuiHelper::InputXMFLOAT4("5", &limitPosData.limitPos[5]);
	ImGui::End();

	buffers->TransData(updateLimitPosDataHandle, &limitPosData, sizeof(UpdateLimitPosData));

	DirectX::XMMATRIX lMatWorld = KazMath::CaluTransMatrix({ 0.0f,0.0f,0.0f }) * KazMath::CaluScaleMatrix({ scale,scale,scale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_SPLINEPARTICLE_UPDATE);


	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(updateViewHandle));

	//共通用バッファのデータ送信
	updateCommonData.scaleRotateBillboardMat = scaleRotaMat * CameraMgr::Instance()->GetMatBillBoard();
	updateCommonData.viewProjection = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
	buffers->TransData(updateCommonHandle, &updateCommonData, sizeof(UpdateCommonData));
	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(updateCommonHandle));
	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(3, buffers->GetGpuAddress(updateLimitPosDataHandle));
	DirectX12CmdList::Instance()->cmdList->Dispatch(100, 1, 1);
}

void SplineParticle::Draw()
{
	GraphicsPipeLineMgr::Instance()->SetPipeLineAndRootSignature(PIPELINE_NAME_GPUPARTICLE);
	DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	DirectX12CmdList::Instance()->cmdList->IASetIndexBuffer(&indexBufferView);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(drawCommandHandle).Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT
	);

	DirectX12CmdList::Instance()->cmdList->ExecuteIndirect
	(
		commandSig.Get(),
		1,
		buffers->GetBufferData(drawCommandHandle).Get(),
		0,
		nullptr,
		0
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(drawCommandHandle).Get(),
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);
}
