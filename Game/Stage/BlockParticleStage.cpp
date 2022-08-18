#include "BlockParticleStage.h"
#include"../KazLibrary/DirectXCommon/DirectX12Device.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Pipeline/GraphicsRootSignature.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

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
	constBufferData.vertices[FRONT_RIGHT_UP] = { lVertices[3].pos.x, lVertices[3].pos.y, lVertices[3].pos.z,0.0f };
	//正面右下
	constBufferData.vertices[FRONT_RIGHT_DOWN] = { lVertices[2].pos.x, lVertices[2].pos.y, lVertices[2].pos.z,0.0f };

	//後面左上
	constBufferData.vertices[BACK_LEFT_UP] = { lVertices[5].pos.x, lVertices[5].pos.y, lVertices[5].pos.z,0.0f };
	//後面左下
	constBufferData.vertices[BACK_LEFT_DOWN] = { lVertices[4].pos.x, lVertices[4].pos.y, lVertices[4].pos.z,0.0f };
	//後面右上
	constBufferData.vertices[BACK_RIGHT_UP] = { lVertices[7].pos.x, lVertices[7].pos.y, lVertices[7].pos.z,0.0f };
	//後面右下
	constBufferData.vertices[BACK_RIGHT_DOWN] = { lVertices[6].pos.x, lVertices[6].pos.y, lVertices[6].pos.z, 0.0f };


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

	commonBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonData)));
	particleDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(ParticleData) * PARTICLE_MAX_NUM));
	outputBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer((sizeof(OutputData) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
	drawCommandHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(IndirectCommand) * DRAW_CALL));
	//counterBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT)));
	//バッファ生成-------------------------

	//転送-------------------------
	buffers->TransData(vertexBufferHandle, vertices.data(), vertBuffSize);
	buffers->TransData(indexBufferHandle, indices.data(), indexBuffSize);
	//UINT reset = 0;
	//buffers->TransData(counterBufferHandle, &reset, sizeof(UINT));


	{
		std::array<ParticleData, PARTICLE_MAX_NUM>lData;
	/*	int arrayIndex = 0;
		const int xMax = 100;

		for (int y = 0; y < 10; ++y)
		{
			for (int z = 0; z < xMax; ++z)
			{
				float side = 2.0f;
				int adjNum = 0;
				if (xMax / 2 <= z)
				{
					side *= -1.0f;
					adjNum = xMax / 2;
				}


				lData[arrayIndex].pos =
				{
					2.0f * side,
					static_cast<float>(y) * 2.0f,
					0.0f + z - adjNum,
					0.0f
				};
				++arrayIndex;
			}
		}
		*/
		for (int i = 0; i < lData.size(); ++i)
		{
			lData[i].pos = { static_cast<float>(i) * 2.0f,0.0f,0.0f,0.0f };
		}
		buffers->TransData(particleDataHandle, lData.data(), sizeof(ParticleData) * PARTICLE_MAX_NUM);
	}


	IndirectCommand command;
	command.drawArguments.IndexCountPerInstance = static_cast<UINT>(indices.size());
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
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputData), PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM),
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


	vertexBufferView = KazBufferHelper::SetVertexBufferView(buffers->GetGpuAddress(vertexBufferHandle), vertBuffSize, sizeof(vertices[0]));
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
		//buffers->GetBufferData(counterBufferHandle).Get(),
		nullptr,
		0
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(drawCommandHandle).Get(),
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);

}