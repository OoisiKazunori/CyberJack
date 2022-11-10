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


	filePass[0] = KazFilePathName::EnemyPath + "BattleShip/" + "BattleshipEnemy_Model.obj";
	filePass[1] = KazFilePathName::EnemyPath + "Bike/" + "Bike_Model.obj";
	filePass[2] = KazFilePathName::EnemyPath + "MisileEnemy/" + "Gunner_Model.obj";
	filePass[3] = KazFilePathName::EnemyPath + "Move/" + "MoveEnemy_Model.obj";
	filePass[4] = KazFilePathName::EnemyPath + "PopEnemy/" + "PopEnemy_Model.obj";
	filePass[5] = KazFilePathName::EnemyPath + "Summon/" + "SummonEnemy_anim.fbx";
	filePass[6] = KazFilePathName::TestPath + "plane.fbx";
	filePass[7] = KazFilePathName::TestPath + "block.fbx";

	RESOURCE_HANDLE lHandle = FbxModelResourceMgr::Instance()->LoadModel(filePass[NUM]);
	std::vector<DirectX::XMFLOAT4>modelVertArrayData = FbxModelResourceMgr::Instance()->GetResourceData(lHandle)->vertData;
	std::vector<UINT>modelIndexArrayData = FbxModelResourceMgr::Instance()->GetResourceData(lHandle)->indexData;
	indexNum = static_cast<UINT>(modelIndexArrayData.size());

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
	verticesDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMFLOAT4) * static_cast<UINT>(modelVertArrayData.size())));
	indexDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT) * static_cast<UINT>(modelIndexArrayData.size())));

	//共通情報
	initCommonHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(InitCommonData)));

	//出力
	outputHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputData) * PARTICLE_MAX_NUM)));
	//初期化用--------

	//更新用
	updateHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(UpdateData) * PARTICLE_MAX_NUM)));
	updateCommonHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(UpdateCommonData)));

	//描画用
	drawCommandHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(IndirectCommand) * DRAW_CALL));

	//バッファ生成-------------------------

	//転送-------------------------
	buffers->TransData(vertexBufferHandle, vertices.data(), vertBuffSize);
	buffers->TransData(indexBufferHandle, indices.data(), indexBuffSize);

	enemyIndex = NUM;
	oldEnemyNum = NUM;
	{
		model.data.handle = lHandle;
		fbxModel.data.handle = lHandle;

		//頂点情報
		memcpy
		(
			buffers->GetMapAddres(verticesDataHandle),
			modelVertArrayData.data(),
			modelVertArrayData.size() * sizeof(DirectX::XMFLOAT4)
		);

		//インデックス情報
		memcpy
		(
			buffers->GetMapAddres(indexDataHandle),
			modelIndexArrayData.data(),
			modelIndexArrayData.size() * sizeof(UINT)
		);

		constBufferData.worldPos = {};
		constBufferData.vertMaxNum = static_cast<UINT>(modelVertArrayData.size());
		constBufferData.indexMaxNum = static_cast<UINT>(modelIndexArrayData.size());
		buffers->TransData(initCommonHandle, &constBufferData, sizeof(InitCommonData));
	}

	model.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	model.data.removeMaterialFlag = true;
	model.data.colorData.color = { 255,255,255,255 };
	model.data.transform.scale = { 1.0f,1.0f,1.0f };

	fbxModel.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	fbxModel.data.removeMaterialFlag = true;
	fbxModel.data.colorData.color = { 255,255,255,255 };
	fbxModel.data.transform.scale = { 1.0f,1.0f,1.0f };

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
		KazBufferHelper::SetUnorderedAccessView(sizeof(DirectX::XMFLOAT4), static_cast<UINT>(modelVertArrayData.size())),
		buffers->GetBufferData(verticesDataHandle).Get(),
		nullptr
	);

	indexViewDataHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		indexViewDataHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(UINT), static_cast<UINT>(modelIndexArrayData.size())),
		buffers->GetBufferData(indexDataHandle).Get(),
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



	//初期化処理--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_MESHPARTICLE_INIT);

	//頂点
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(vertDataViewHandle));
	//インデックス
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(indexViewDataHandle));
	//出力
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
	//共通用バッファのデータ送信
	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(3, buffers->GetGpuAddress(initCommonHandle));

	UINT lNum = indexNum / 1024;
	if (lNum <= 0)
	{
		lNum = 1;
	}
	DirectX12CmdList::Instance()->cmdList->Dispatch(50, 1, 1);
	//初期化処理--------------------------------------------



	resetSceneFlag = false;

	float lScale = 0.18f;
	scaleRotaMat = KazMath::CaluScaleMatrix({ lScale,lScale,lScale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });

	updateCommonData.indexMaxNum = indexNum;
}

MeshParticleEmitter::~MeshParticleEmitter()
{
	DescriptorHeapMgr::Instance()->Release(vertDataViewHandle);
	DescriptorHeapMgr::Instance()->Release(indexViewDataHandle);
	DescriptorHeapMgr::Instance()->Release(outputViewHandle);
	DescriptorHeapMgr::Instance()->Release(updateViewHandle);
}

void MeshParticleEmitter::Init(DirectX::XMMATRIX *MOTHER_MAT)
{
	motherMat = MOTHER_MAT;
	sceneNum = 0;
	oldEnemyNum = 0;
	resetSceneFlag = false;
}

void MeshParticleEmitter::Update()
{

	//初期化処理--------------------------------------------
	//DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	//GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_MESHPARTICLE_INIT);

	////頂点
	//DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(vertDataViewHandle));
	////インデックス
	//DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(indexViewDataHandle));
	////出力
	//DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
	////共通用バッファのデータ送信
	//DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(3, buffers->GetGpuAddress(initCommonHandle));
	//DirectX12CmdList::Instance()->cmdList->Dispatch(50, 1, 1);
	//初期化処理--------------------------------------------


	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_MESHPARTICLE_UPDATE);

	{
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(updateViewHandle));
	}

	//共通用バッファのデータ送信
	{
		updateCommonData.scaleRotateBillboardMat = scaleRotaMat * CameraMgr::Instance()->GetMatBillBoard();
		updateCommonData.viewProjection = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
		updateCommonData.motherMat = *motherMat;
		buffers->TransData(updateCommonHandle, &updateCommonData, sizeof(UpdateCommonData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(updateCommonHandle));
	}
	UINT lNum = indexNum / 1024;
	if (lNum <= 0)
	{
		lNum = 1;
	}
	DirectX12CmdList::Instance()->cmdList->Dispatch(80, 1, 1);

	ImGui::Begin("Mesh");
	ImGui::SliderInt("ColorA", &fbxModel.data.colorData.color.a, 0, 255);
	ImGui::InputInt("Enemy", &enemyIndex);
	ImGui::End();

	if (enemyIndex != oldEnemyNum)
	{
		oldEnemyNum = enemyIndex;
		resetSceneFlag = true;
	}
}

void MeshParticleEmitter::Draw()
{

	//model.Draw();
	fbxModel.Draw();

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
