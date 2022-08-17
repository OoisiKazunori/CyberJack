#include "BlockParticleStage.h"
#include"../KazLibrary/DirectXCommon/DirectX12Device.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Pipeline/GraphicsRootSignature.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

BlockParticleStage::BlockParticleStage()
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

	//static const int VERT_MAX_NUM = 200;
	const float lSize = 1.0f;
	Vertex lVertices[] =
	{
		//x,y,z	法線	u,v
	//前
	{{-lSize, -lSize, -lSize},{}, {0.0f,1.0f}},		//左下
	{{-lSize,  lSize, -lSize},{}, {0.0f,0.0f}},		//左上
	{{ lSize, -lSize, -lSize}, {}, {1.0f,1.0f}},		//右下
	{{ lSize,  lSize, -lSize}, {}, {1.0f,0.0f}},		//右上

	//後
	{{-lSize,-lSize,  lSize}, {}, {0.0f,1.0f}},		//左下
	{{-lSize, lSize,  lSize}, {},{0.0f,0.0f}},		//左上
	{{ lSize,-lSize,  lSize},{},{1.0f,1.0f}},		//右下
	{{ lSize, lSize,  lSize},{}, {1.0f,0.0f}},		//右上

	//左
	{{-lSize,-lSize, -lSize},{}, {0.0f,1.0f}},		//左下
	{{-lSize,-lSize,  lSize}, {}, {0.0f,0.0f}},		//左上
	{{-lSize, lSize, -lSize}, {}, {1.0f,1.0f}},		//右下
	{{-lSize, lSize,  lSize}, {}, {1.0f,0.0f}},		//右上

	//右
	{{lSize,-lSize, -lSize},{}, {0.0f,1.0f}},		//左下
	{{lSize,-lSize,  lSize}, {}, {0.0f,0.0f}},		//左上
	{{lSize, lSize, -lSize}, {}, {1.0f,1.0f}},		//右下
	{{lSize, lSize,  lSize}, {}, {1.0f,0.0f}},		//右上

	//下
	{{ lSize, -lSize, lSize}, {}, {0.0f,1.0f}},		//左下
	{{ lSize, -lSize,-lSize}, {}, {0.0f,0.0f}},		//左上
	{{-lSize, -lSize, lSize}, {}, {1.0f,1.0f}},		//右下
	{{-lSize, -lSize,-lSize}, {}, {1.0f,0.0f}},	//右上

	//上
	{{ lSize, lSize, lSize}, {}, {0.0f,1.0f}},			//左下
	{{ lSize, lSize,-lSize}, {}, {0.0f,0.0f}},			//左上
	{{-lSize, lSize, lSize}, {}, {1.0f,1.0f}},			//右下
	{{-lSize, lSize,-lSize}, {}, {1.0f,0.0f}}			//右上
	};
	unsigned short lIndices[] =
	{
		//前
		0,1,2,		//三角形1つ目
		2,1,3,		//三角形2つ目
		//後
		4,6,5,		//三角形3つ目
		6,7,5,		//三角形4つ目
		//左
		8,9,10,	//三角形1つ目
		10,9,11,	//三角形2つ目
		//右
		12,14,13,	//三角形1つ目
		13,14,15,//三角形2つ目
		//下
		16,18,17,	//三角形1つ目
		17,18,19,	//三角形2つ目
		//上
		20,21,22,	//三角形3つ目
		22,21,23	//三角形4つ目
	};

	const int FRONT_LEFT_UP = 0;
	const int FRONT_LEFT_DOWN = 1;
	const int FRONT_RIGHT_UP = 2;
	const int FRONT_RIGHT_DOWN = 3;
	const int BACK_LEFT_UP = 4;
	const int BACK_LEFT_DOWN = 5;
	const int BACK_RIGHT_UP = 6;
	const int BACK_RIGHT_DOWN = 7;

	//正面左上
	constBufferData.vertices[FRONT_LEFT_UP] = { lVertices[1].pos.x, lVertices[1].pos.y, lVertices[1].pos.z,0.0f };
	//正面左下
	constBufferData.vertices[FRONT_LEFT_DOWN] = { lVertices[0].pos.x, lVertices[0].pos.y, lVertices[0].pos.z,0.0f };
	//正面右上
	constBufferData.vertices[FRONT_RIGHT_UP] = { lVertices[2].pos.x, lVertices[2].pos.y, lVertices[2].pos.z,0.0f };
	//正面右下
	constBufferData.vertices[FRONT_RIGHT_DOWN] = { lVertices[3].pos.x, lVertices[3].pos.y, lVertices[3].pos.z,0.0f };

	//後面左上
	constBufferData.vertices[BACK_LEFT_UP] = { lVertices[1].pos.x, lVertices[1].pos.y, lVertices[1].pos.z,0.0f };
	//後面左下
	constBufferData.vertices[BACK_LEFT_DOWN] = { lVertices[0].pos.x, lVertices[0].pos.y, lVertices[0].pos.z,0.0f };
	//後面右上
	constBufferData.vertices[BACK_RIGHT_UP] = { lVertices[2].pos.x, lVertices[2].pos.y, lVertices[2].pos.z,0.0f };
	//後面右下
	constBufferData.vertices[BACK_RIGHT_DOWN] = { lVertices[3].pos.x, lVertices[3].pos.y, lVertices[3].pos.z,0.0f };



	int indexDataNum = 0;
	//正面左上から伸びる辺
	constBufferData.index0[0] = FRONT_LEFT_UP;
	constBufferData.index0[1] = FRONT_LEFT_DOWN;
	++indexDataNum;		 
	constBufferData.index1[0] = FRONT_LEFT_UP;
	constBufferData.index1[1] = FRONT_RIGHT_UP;
	++indexDataNum;		 
	constBufferData.index2[0] = FRONT_LEFT_UP;
	constBufferData.index2[1] = BACK_LEFT_UP;

	//正面右下から伸びる辺  
	++indexDataNum;
	constBufferData.index3[0] = FRONT_RIGHT_DOWN;
	constBufferData.index3[1] = FRONT_RIGHT_UP;
	++indexDataNum;		 
	constBufferData.index4[0] = FRONT_RIGHT_DOWN;
	constBufferData.index4[1] = FRONT_LEFT_DOWN;
	++indexDataNum;		 
	constBufferData.index5[0] = FRONT_RIGHT_DOWN;
	constBufferData.index5[1] = BACK_RIGHT_DOWN;

	//後面左上から伸びる辺
	++indexDataNum;
	constBufferData.index6[0] = BACK_LEFT_UP;
	constBufferData.index6[1] = BACK_LEFT_DOWN;
	++indexDataNum;		 
	constBufferData.index7[0] = BACK_LEFT_UP;
	constBufferData.index7[1] = BACK_RIGHT_UP;

	//後面右下から伸びる辺
	++indexDataNum;
	constBufferData.index8[0] = BACK_RIGHT_DOWN;
	constBufferData.index8[1] = BACK_LEFT_DOWN;
	++indexDataNum;
	constBufferData.index9[0] = BACK_RIGHT_DOWN;
	constBufferData.index9[1] = BACK_RIGHT_UP;

	//正面左下から伸びる辺
	++indexDataNum;
	constBufferData.index10[0] = FRONT_LEFT_UP;
	constBufferData.index10[1] = BACK_LEFT_DOWN;

	//後面右上から伸びる辺
	++indexDataNum;
	constBufferData.index11[0] = BACK_RIGHT_UP;
	constBufferData.index11[1] = FRONT_RIGHT_UP;


	size_t vertArraySize = sizeof(lVertices) / sizeof(lVertices[0]);
	size_t indexArraySize = sizeof(lIndices) / sizeof(lIndices[0]);

	BUFFER_SIZE vertBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vertArraySize, sizeof(Vertex));
	BUFFER_SIZE indexBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(indexArraySize, sizeof(unsigned int));

	//バッファ生成-------------------------
	vertexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetVertexBufferData(vertBuffSize));
	indexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetIndexBufferData(indexBuffSize));

	commonBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonData)));
	particleDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(ParticleData) * PARTICLE_MAX_NUM));
	outputBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer((sizeof(OutputData) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
	drawCommandHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(IndirectCommand) * DRAW_CALL));
	//counterBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT)));
	//バッファ生成-------------------------

	//転送-------------------------
	buffers->TransData(vertexBufferHandle, lVertices, vertBuffSize);
	buffers->TransData(indexBufferHandle, lIndices, indexBuffSize);
	UINT reset = 0;
	//buffers->TransData(counterBufferHandle, &reset, sizeof(UINT));


	{
		std::array<ParticleData, PARTICLE_MAX_NUM>lData;
		for (int i = 0; i < lData.size(); ++i)
		{
			lData[i].pos = { 0.0f,0.0f,0.0f,0.0f };
		}
		buffers->TransData(particleDataHandle, &reset, sizeof(ParticleData) * PARTICLE_MAX_NUM);
	}


	IndirectCommand command;
	command.drawArguments.IndexCountPerInstance = static_cast<UINT>(indexArraySize);
	command.drawArguments.InstanceCount = PER_USE_PARTICLE_MAX_NUM * PARTICLE_MAX_NUM;
	command.drawArguments.StartIndexLocation = 0;
	command.drawArguments.StartInstanceLocation = 0;
	command.drawArguments.BaseVertexLocation = 0;

	command.uav = buffers->GetGpuAddress(outputBufferHandle);
	buffers->TransData(drawCommandHandle, &command, sizeof(IndirectCommand) * DRAW_CALL);
	//転送-------------------------

	computeMemSize = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER);
	DescriptorHeapMgr::Instance()->CreateBufferView(
		computeMemSize.startSize + 0,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputBufferHandle).Get(),
		//buffers->GetBufferData(counterBufferHandle).Get()
		nullptr
	);

	DescriptorHeapMgr::Instance()->CreateBufferView(
		computeMemSize.startSize + 1,
		KazBufferHelper::SetUnorderedAccessView(sizeof(ParticleData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(particleDataHandle).Get(),
		nullptr
	);


	vertexBufferView = KazBufferHelper::SetVertexBufferView(buffers->GetGpuAddress(vertexBufferHandle), vertBuffSize, sizeof(lVertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(buffers->GetGpuAddress(indexBufferHandle), indexBuffSize);

}

BlockParticleStage::~BlockParticleStage()
{
	DescriptorHeapMgr::Instance()->Release(computeMemSize.startSize + 0);
	DescriptorHeapMgr::Instance()->Release(computeMemSize.startSize + 1);
}

void BlockParticleStage::Update()
{
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_BLOCKPARTICLE);

	{
		//UINT reset = 0;
		//buffers->TransData(counterBufferHandle, &reset, sizeof(UINT));
	}

	//共通用バッファのデータ送信
	{
		constBufferData.cameraMat = CameraMgr::Instance()->GetViewMatrix();
		constBufferData.projectionMat = CameraMgr::Instance()->GetPerspectiveMatProjection();
		constBufferData.bollboardMat = CameraMgr::Instance()->GetMatBillBoard();
		buffers->TransData(commonBufferHandle, &constBufferData, sizeof(CommonData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(commonBufferHandle));
	}

	{
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(computeMemSize.startSize + 0));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(computeMemSize.startSize + 1));
	}

	DirectX12CmdList::Instance()->cmdList->Dispatch(PARTICLE_MAX_NUM, 1, 1);
}

void BlockParticleStage::Draw()
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
		//buffers->GetBufferData(counterBufferHandle).Get(),
		0
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(drawCommandHandle).Get(),
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);

}