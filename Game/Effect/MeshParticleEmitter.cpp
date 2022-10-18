#include "MeshParticleEmitter.h"
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

MeshParticleEmitter::MeshParticleEmitter(int NUM)
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
	vertexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetVertexBufferData(vertBuffSize));
	indexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetIndexBufferData(indexBuffSize));

	commonInitBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonData)));

	verticesDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer((sizeof(DirectX::XMFLOAT4) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
	indexDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer((sizeof(UINT) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
	outputInitBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer((sizeof(OutputInitData) * 50000)));
	drawCommandHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(IndirectCommand) * DRAW_CALL));
	countIndexBuffHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT)));
	counterBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT)));
	//バッファ生成-------------------------

	//転送-------------------------
	buffers->TransData(vertexBufferHandle, vertices.data(), vertBuffSize);
	buffers->TransData(indexBufferHandle, indices.data(), indexBuffSize);
	UINT reset = 0;
	buffers->TransData(countIndexBuffHandle, &reset, sizeof(UINT));
	buffers->TransData(counterBufferHandle, &reset, sizeof(UINT));


	filePass[0] = KazFilePathName::EnemyPath + "BattleShip/" + "BattleshipEnemy_Model.obj";
	filePass[1] = KazFilePathName::EnemyPath + "Bike/" + "Bike_Model.obj";
	filePass[2] = KazFilePathName::EnemyPath + "MisileEnemy/" + "Gunner_Model.obj";
	filePass[3] = KazFilePathName::EnemyPath + "Move/" + "MoveEnemy_Model.obj";
	filePass[4] = KazFilePathName::EnemyPath + "PopEnemy/" + "PopEnemy_Model.obj";
	filePass[5] = KazFilePathName::EnemyPath + "Summon/" + "SummonEnemy_Model.obj";

	enemyIndex = NUM;
	oldEnemyNum = NUM;
	{
		RESOURCE_HANDLE lHandle = ObjResourceMgr::Instance()->LoadModel(filePass[NUM]);
		model.data.handle = lHandle;

		//頂点情報
		memcpy
		(
			buffers->GetMapAddres(verticesDataHandle),
			ObjResourceMgr::Instance()->GetResourceData(lHandle).vertices.data(),
			ObjResourceMgr::Instance()->GetResourceData(lHandle).vertices.size() * sizeof(DirectX::XMFLOAT4)
		);

		//インデックス情報
		memcpy
		(
			buffers->GetMapAddres(indexDataHandle),
			ObjResourceMgr::Instance()->GetResourceData(lHandle).index.data(),
			ObjResourceMgr::Instance()->GetResourceData(lHandle).index.size() * sizeof(UINT)
		);

		constBufferData.worldPos = {};
		constBufferData.vertMaxNum = static_cast<UINT>(ObjResourceMgr::Instance()->GetResourceData(lHandle).vertices.size());
		constBufferData.indexMaxNum = static_cast<UINT>(ObjResourceMgr::Instance()->GetResourceData(lHandle).index.size());
	}


	model.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	model.data.removeMaterialFlag = true;
	model.data.colorData.color = { 255,255,255,255 };

	IndirectCommand command;
	command.drawArguments.IndexCountPerInstance = static_cast<UINT>(indices.size());
	command.drawArguments.InstanceCount = 50000;
	command.drawArguments.StartIndexLocation = 0;
	command.drawArguments.StartInstanceLocation = 0;
	command.drawArguments.BaseVertexLocation = 0;

	command.uav = buffers->GetGpuAddress(outputInitBufferHandle);
	buffers->TransData(drawCommandHandle, &command, sizeof(IndirectCommand) * DRAW_CALL);
	//転送-------------------------

	outputInitViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputInitViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputInitData), 50000),
		buffers->GetBufferData(outputInitBufferHandle).Get(),
		buffers->GetBufferData(counterBufferHandle).Get()
	);

	vertDataViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		vertDataViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(DirectX::XMFLOAT4), PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM),
		buffers->GetBufferData(verticesDataHandle).Get(),
		nullptr
	);

	indexViewDataHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		indexViewDataHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(UINT), PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM),
		buffers->GetBufferData(indexDataHandle).Get(),
		nullptr
	);

	counterIndexBufferViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		counterIndexBufferViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(UINT), 1),
		buffers->GetBufferData(countIndexBuffHandle).Get(),
		nullptr
	);

	vertexBufferView = KazBufferHelper::SetVertexBufferView(buffers->GetGpuAddress(vertexBufferHandle), vertBuffSize, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(buffers->GetGpuAddress(indexBufferHandle), indexBuffSize);


	resetSceneFlag = false;
}

MeshParticleEmitter::~MeshParticleEmitter()
{
	DescriptorHeapMgr::Instance()->Release(outputInitViewHandle);
	DescriptorHeapMgr::Instance()->Release(vertDataViewHandle);
	DescriptorHeapMgr::Instance()->Release(indexViewDataHandle);
	DescriptorHeapMgr::Instance()->Release(counterIndexBufferViewHandle);
}

void MeshParticleEmitter::Init(const KazMath::Vec3<float> &POS)
{
	pos = POS;
	sceneNum = 0;
	oldEnemyNum = 0;
	resetSceneFlag = false;
}

void MeshParticleEmitter::Update()
{
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_MESHPARTICLE);

	UINT reset = 0;
	buffers->TransData(countIndexBuffHandle, &reset, sizeof(UINT));
	buffers->TransData(counterBufferHandle, &reset, sizeof(UINT));

	{
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputInitViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(vertDataViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(indexViewDataHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(3, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(counterIndexBufferViewHandle));
	}

	//共通用バッファのデータ送信
	{
		constBufferData.cameraMat = CameraMgr::Instance()->GetViewMatrix();
		constBufferData.projectionMat = CameraMgr::Instance()->GetPerspectiveMatProjection();
		constBufferData.bollboardMat = CameraMgr::Instance()->GetMatBillBoard();
		buffers->TransData(commonInitBufferHandle, &constBufferData, sizeof(CommonData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(4, buffers->GetGpuAddress(commonInitBufferHandle));
	}


	{
		RESOURCE_HANDLE lHandle = ObjResourceMgr::Instance()->LoadModel(filePass[enemyIndex]);
		model.data.handle = lHandle;

		//頂点情報
		memcpy
		(
			buffers->GetMapAddres(verticesDataHandle),
			ObjResourceMgr::Instance()->GetResourceData(lHandle).vertices.data(),
			ObjResourceMgr::Instance()->GetResourceData(lHandle).vertices.size() * sizeof(DirectX::XMFLOAT4)
		);

		//インデックス情報
		memcpy
		(
			buffers->GetMapAddres(indexDataHandle),
			ObjResourceMgr::Instance()->GetResourceData(lHandle).index.data(),
			ObjResourceMgr::Instance()->GetResourceData(lHandle).index.size() * sizeof(UINT)
		);

		constBufferData.worldPos = {};
		constBufferData.vertMaxNum = static_cast<UINT>(ObjResourceMgr::Instance()->GetResourceData(lHandle).vertices.size());
		constBufferData.indexMaxNum = static_cast<UINT>(ObjResourceMgr::Instance()->GetResourceData(lHandle).index.size());
	}


	DirectX12CmdList::Instance()->cmdList->Dispatch(10, 1, 1);

	ImGui::Begin("Mesh");
	ImGui::SliderInt("ColorA", &model.data.colorData.color.a, 0, 255);
	ImGui::InputInt("Enemy", &enemyIndex);
	ImGui::End();
	constBufferData.dev = lDev;

	if (enemyIndex != oldEnemyNum)
	{
		oldEnemyNum = enemyIndex;
		resetSceneFlag = true;
	}
}

void MeshParticleEmitter::Draw()
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

	model.Draw();

}
