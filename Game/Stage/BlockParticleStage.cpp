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
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"
#include"../Game/Effect/InstanceMeshParticle.h"
#include"../KazLibrary/Loader/MeshParticleLoader.h"

const float BlockParticleStage::PILLAR_PARTICLE_INTERVAL_NUM = 15000.0f;

BlockParticleStage::BlockParticleStage()
{
	buffers = std::make_unique<CreateGpuBuffer>();

	highFlag = false;
	prepareFlag = true;
	prepareTimer = 0;
	flashTimer = 0;

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
	const float lSize = 10.0f;
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

	CommonData lConstBufferData;
	//正面左上
	lConstBufferData.vertices[FRONT_LEFT_UP] = { lVertices[1].pos.x, lVertices[1].pos.y, lVertices[1].pos.z,0.0f };
	//正面左下
	lConstBufferData.vertices[FRONT_LEFT_DOWN] = { lVertices[0].pos.x, lVertices[0].pos.y, lVertices[0].pos.z,0.0f };
	//正面右上
	lConstBufferData.vertices[FRONT_RIGHT_UP] = { lVertices[3].pos.x, lVertices[3].pos.y, lVertices[3].pos.z,0.0f };
	//正面右下
	lConstBufferData.vertices[FRONT_RIGHT_DOWN] = { lVertices[2].pos.x, lVertices[2].pos.y, lVertices[2].pos.z,0.0f };

	//後面左上
	lConstBufferData.vertices[BACK_LEFT_UP] = { lVertices[5].pos.x, lVertices[5].pos.y, lVertices[5].pos.z,0.0f };
	//後面左下
	lConstBufferData.vertices[BACK_LEFT_DOWN] = { lVertices[4].pos.x, lVertices[4].pos.y, lVertices[4].pos.z,0.0f };
	//後面右上
	lConstBufferData.vertices[BACK_RIGHT_UP] = { lVertices[7].pos.x, lVertices[7].pos.y, lVertices[7].pos.z,0.0f };
	//後面右下
	lConstBufferData.vertices[BACK_RIGHT_DOWN] = { lVertices[6].pos.x, lVertices[6].pos.y, lVertices[6].pos.z, 0.0f };


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
	commonBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonMoveData)));
	particleDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(ParticleData) * PARTICLE_MAX_NUM));
	outputInitBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputInitData) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
	outputBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputData) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
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

		std::string lObjectName = "BlockIndex3";
		blockFileMgr.LoadFile(KazFilePathName::StageParamPath + "blockPosData.json");
		int lCountNum = 0;
		for (int i = 0; i < 4000; ++i)
		{
			std::string name = lObjectName + "_" + std::to_string(i);
			KazMath::Vec3<int> lNum(
				blockFileMgr.doc[name.c_str()]["X"].GetInt(),
				blockFileMgr.doc[name.c_str()]["Y"].GetInt(),
				blockFileMgr.doc[name.c_str()]["Z"].GetInt()
			);

			lData[i].pos = {
				static_cast<float>(lNum.x) * (lSize * 2.0f) - lSize * 30.0f,
				static_cast<float>(lNum.y) * (lSize * 2.0f) - lSize * 30.0f,
				static_cast<float>(lNum.z) * (lSize * 2.0f),
				0.0f
			};

			if (lData[i].pos.x == 0.0f && lData[i].pos.y == 0.0f && lData[i].pos.z == 0.0f)
			{
				lData[i].pos.x = -1000000.0f;
			}
			if (lNum.x != 0.0f || lNum.y != 0.0f || lNum.z != 0.0f)
			{
				++lCountNum;
			}
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

	outputInitViewHandle = UavViewHandleMgr::Instance()->GetHandle();

	computeMemSize = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER);
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputInitViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputInitData), PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputInitBufferHandle).Get(),
		nullptr
	);

	particleDataViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		particleDataViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(ParticleData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(particleDataHandle).Get(),
		nullptr
	);

	outputViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputData), PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputBufferHandle).Get(),
		nullptr
	);


	vertexBufferView = KazBufferHelper::SetVertexBufferView(buffers->GetGpuAddress(vertexBufferHandle), vertBuffSize, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(buffers->GetGpuAddress(indexBufferHandle), indexBuffSize);

	constBufferData.flash.x = 0.0f;


	//パーティクル初期化処理--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_BLOCKPARTICLE);

	{
		lConstBufferData.cameraMat = CameraMgr::Instance()->GetViewMatrix();
		lConstBufferData.projectionMat = CameraMgr::Instance()->GetPerspectiveMatProjection();
		lConstBufferData.billboardMat = CameraMgr::Instance()->GetMatBillBoard();
		buffers->TransData(commonInitBufferHandle, &lConstBufferData, sizeof(CommonData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(commonInitBufferHandle));
	}

	{
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputInitViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(particleDataViewHandle));
	}

	DirectX12CmdList::Instance()->cmdList->Dispatch(PARTICLE_MAX_NUM, 1, 1);
	//パーティクル初期化処理--------------------------------------------


	for (int i = 0; i < splineParticle.size(); ++i)
	{
		splineParticle[i] = std::make_unique<SplineParticle>(2.5f);
	}

	//大渦初期化--------------------------------------------


	v = { 40.0f,800.0f,100.0f };
	{
		KazMath::Vec3<float>level = { 100.0f,300.0f,0.0f };
		std::vector<KazMath::Vec3<float>> limitPosArray;
		for (int i = 0; i < 150; ++i)
		{
			level.x = cosf(KazMath::AngleToRadian(i * static_cast<int>(v.x))) * v.y;
			level.y = sinf(KazMath::AngleToRadian(i * static_cast<int>(v.x))) * v.y;
			limitPosArray.push_back(KazMath::Vec3<float>(level.x, level.y, -3000.0f + static_cast<float>(i) * v.z));
		}
		splineParticle[0]->Init(limitPosArray, false);
	}
	//大渦初期化--------------------------------------------
	{
		for (int i = 1; i < splineParticle.size(); ++i)
		{
			std::vector<KazMath::Vec3<float>> limitPosArray;
			KazMath::Vec3<float>lStartPos;
			lStartPos.y = KazMath::Rand(5000.0f, -5000.0f);

			//左から右
			if (KazMath::Rand(3, 0) < 2)
			{
				lStartPos.x = KazMath::Rand(7000.0f, 2000.0f);
			}
			else
			{
				lStartPos.x = -KazMath::Rand(7000.0f, 2000.0f);
			}
			bool lupDownFlag = false;
			//上に行くかどうか
			if (KazMath::Rand(3, 0) < 2)
			{
				lupDownFlag = true;
			}

			for (int i = 0; i < 150; ++i)
			{
				lStartPos.z = -5000.0f + static_cast<float>(i) * v.z;
				limitPosArray.push_back(lStartPos);
			}
			splineParticle[i]->Init(limitPosArray, false);
		}
	}


	floorResourceHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::StagePath + "lambert1_Base_color.png");

	std::vector<VertexUv> lV = GetPlaneData(floorResourceHandle);

	std::vector<DirectX::XMFLOAT3>lPos;
	std::vector<DirectX::XMFLOAT2>lUv;
	for (int i = 0; i < lV.size(); ++i)
	{
		lPos.push_back(lV[i].pos);
		lUv.push_back(lV[i].uv);
	}


	meshBuffer = std::make_unique<CreateMeshBuffer>(lPos, lUv);
	InitMeshParticleData lFloorData;
	lFloorData.vertData = meshBuffer->GetBufferData(CreateMeshBuffer::DATA_VERT);
	lFloorData.uvData = meshBuffer->GetBufferData(CreateMeshBuffer::DATA_UV);
	lFloorData.textureHandle = floorResourceHandle;
	lFloorData.triagnleData.x = 6;
	lFloorData.triagnleData.y = 0;
	lFloorData.triagnleData.z = 10000;
	lFloorData.triagnleData.w = 2;

	for (int i = 0; i < floorParticleModel.size(); ++i)
	{
		floorParticleTransform[i].pos = { 0.0f,-300.0f,500.0f + static_cast<float>(i) * 700.0f };
		floorParticleTransform[i].rotation = { 90.0f,0.0f,0.0f };

		floorParticleModel[i] = std::make_unique<TextureParticle>(GetPlaneData(floorResourceHandle), &floorParticleMotherMat[i], floorResourceHandle, 5.0f, 10000, 2);

		lFloorData.motherMat = &floorParticleMotherMat[i];
		InstanceMeshParticle::Instance()->AddMeshData(lFloorData);
	}
	MeshParticleLoadData lData{};
	lData.bias = 0;
	lData.perTriangleNum = 100;
	lData.faceCountNum = 250;

	pillarHandle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "house/" + "House_01.fbx", true);
	for (int i = 0; i < pillarParticleTransform.size() / 2; ++i)
	{
		pillarParticleTransform[i].pos = { -5000.0f,0.0f,-1000.0f + static_cast<float>(i) * PILLAR_PARTICLE_INTERVAL_NUM };
		pillarParticleTransform[i].scale = { 23.5f,25.0f,23.5f };
		RESOURCE_HANDLE lHandle = FbxModelResourceMgr::Instance()->GetResourceData(pillarHandle)->textureHandle[0];
		UINT lFaceCountNum = FbxModelResourceMgr::Instance()->GetResourceData(pillarHandle)->faceCountNum;
		pillarParticleModel[i] = std::make_unique<TextureParticle>(FbxModelResourceMgr::Instance()->GetResourceData(pillarHandle)->vertUvData, &pillarParticleMotherMat[i], lHandle, 1.5f, 200, 2000);


		InstanceMeshParticle::Instance()->AddMeshData(
			MeshParticleLoader::Instance()->Load(KazFilePathName::StagePath + "house/" + "House_01.fbx", true, &pillarParticleMotherMat[i], lData)
		);
	}

	const int L_HALF_NUM = static_cast<int>(pillarParticleTransform.size()) / 2;
	for (int i = L_HALF_NUM; i < pillarParticleTransform.size(); ++i)
	{
		pillarParticleTransform[i].pos = { 5000.0f,0.0f,-1000.0f + static_cast<float>(i) * PILLAR_PARTICLE_INTERVAL_NUM - L_HALF_NUM * PILLAR_PARTICLE_INTERVAL_NUM };
		pillarParticleTransform[i].scale = { 23.5f,25.0f,23.5f };
		RESOURCE_HANDLE lHandle = FbxModelResourceMgr::Instance()->GetResourceData(pillarHandle)->textureHandle[0];
		UINT lFaceCountNum = FbxModelResourceMgr::Instance()->GetResourceData(pillarHandle)->faceCountNum;
		pillarParticleModel[i] = std::make_unique<TextureParticle>(FbxModelResourceMgr::Instance()->GetResourceData(pillarHandle)->vertUvData, &pillarParticleMotherMat[i], lHandle, 1.5f, 200, 2000);

		InstanceMeshParticle::Instance()->AddMeshData(
			MeshParticleLoader::Instance()->Load(KazFilePathName::StagePath + "house/" + "House_01.fbx", true, &pillarParticleMotherMat[i], lData)
		);
	}




	MeshParticleLoadData lParticleData;
	lParticleData.bias = 70;
	lParticleData.faceCountNum = 21935;
	lParticleData.perTriangleNum = 5;

	InitMeshParticleData lStageMeshParticleData = MeshParticleLoader::Instance()->Load(KazFilePathName::StagePath + "Dungeon_Wall.fbx", false, &transformArrayData[0].GetMat(), lParticleData);
	RESOURCE_HANDLE lHandle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "Dungeon_Wall.fbx");
	lStageMeshParticleData.color = { 0.2f,0.2f,0.2f,1.0f };


	collisionArrrayData.emplace_back(InitMeshCollisionData());
	collisionArrrayData[0].vertData = lStageMeshParticleData.vertData;
	collisionArrrayData[0].vertNumArray = FbxModelResourceMgr::Instance()->GetResourceData(lHandle)->vertNum;
	collisionArrrayData[0].meshParticleData = lStageMeshParticleData;
	collisionArrrayData[0].motherMat = &transformMatArray[0];
	collisionArrrayData[0].colorData = &colorArrayData[0];

	collisionArrrayData.emplace_back(InitMeshCollisionData());
	collisionArrrayData[1].vertData = lStageMeshParticleData.vertData;
	collisionArrrayData[1].vertNumArray = FbxModelResourceMgr::Instance()->GetResourceData(lHandle)->vertNum;
	collisionArrrayData[1].meshParticleData = lStageMeshParticleData;
	collisionArrrayData[1].motherMat = &transformMatArray[1];
	collisionArrrayData[1].colorData = &colorArrayData[1];

	for (int i = 0; i < colorArrayData.size(); ++i)
	{
		colorArrayData[i].alpha = 1.0f;
		colorArrayData[i].lightData = { 0,0 };
	}

	for (int i = 0; i < transformArrayData.size(); ++i)
	{
		transformArrayData[i].pos = { 0.0f,0.0f,static_cast<float>(i) * 500.0f + 0.0f };
	}

	galacticParticle = std::make_unique<GalacticParticle>();
}

BlockParticleStage::~BlockParticleStage()
{
	DescriptorHeapMgr::Instance()->Release(outputInitViewHandle);
	DescriptorHeapMgr::Instance()->Release(outputViewHandle);
	DescriptorHeapMgr::Instance()->Release(particleDataViewHandle);
}

void BlockParticleStage::Update()
{
	//GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_BLOCKPARTICLE_MOVE);

	////共通用バッファのデータ送信
	//{
	//	constBufferData.viewProjectionMat = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
	//	constBufferData.scaleRotateBillboardMat = KazMath::CaluScaleMatrix({ 1.0f,1.0f,1.0f }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f }) * CameraMgr::Instance()->GetMatBillBoard();

	//	buffers->TransData(commonBufferHandle, &constBufferData, sizeof(CommonMoveData));
	//	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(commonBufferHandle));
	//	constBufferData.flash.y = 0;
	//}

	//{
	//	//初期化値
	//	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputInitViewHandle));
	//	//結果値
	//	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
	//}

	//DirectX12CmdList::Instance()->cmdList->Dispatch((PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM) / 1000, 1, 1);


	if (highFlag)
	{
		constBufferData.flash.x += 5.0f;
		if (650.0f <= constBufferData.flash.x)
		{
			highFlag = false;
			prepareFlag = true;
		}
	}

	if (prepareFlag)
	{
		++prepareTimer;
		if (120 <= prepareTimer)
		{
			highFlag = true;
			constBufferData.flash.x = -610.0f;

			prepareTimer = 0;
			prepareFlag = false;
		}
	}

	//++flashTimer;
	if (!highFlag && 360 <= flashTimer)
	{
		constBufferData.flash.y = 1;
		//flashTimer = 0;
	}
	else
	{
		constBufferData.flash.y = 0;
	}


	galacticParticle->Update();

	//KazMath::Vec3<float>level = { 100.0f,300.0f,0.0f };
	//std::vector<KazMath::Vec3<float>> limitPosArray;
	//for (int i = 0; i < 150; ++i)
	//{
	//	level.x = cosf(KazMath::AngleToRadian(i * static_cast<int>(v.x))) * v.y;
	//	level.y = sinf(KazMath::AngleToRadian(i * static_cast<int>(v.x))) * v.y;
	//	limitPosArray.push_back(KazMath::Vec3<float>(level.x, level.y, -1000.0f + static_cast<float>(i) * v.z));
	//}
	//for (int i = 0; i < splineParticle.size(); ++i)
	//{
	//	splineParticle[i]->Init(limitPosArray);
	//}

	for (int i = 0; i < splineParticle.size(); ++i)
	{
		splineParticle[i]->Update();
	}

	for (int i = 0; i < floorParticleModel.size(); ++i)
	{
		floorParticleTransform[i].pos.z += -5.0f;
		if (floorParticleTransform[i].pos.z <= -1000.0f)
		{
			floorParticleTransform[i].pos.z = (500.0f + static_cast<float>(FLOOR_PARTICLE_MAX_NUM) * 700.0f) - 1200.0f;
		}
		floorParticleMotherMat[i] = floorParticleTransform[i].GetMat();


		floorParticleModel[i]->updateCommonData.flash.x = flash.x;
		floorParticleModel[i]->updateCommonData.flash.y = flash.y;
		floorParticleModel[i]->Update(false, true);
	}

	//ImGui::Begin("Block");
	//KazImGuiHelper::InputVec2("Flash", &flash);
	//ImGui::End();

	flash.x += 200.0f;
	if (100000.0f <= flash.x)
	{
		flash.x = -10000.0f;
	}
	if (hitFlag)
	{
		flash.y = 1.0f;
	}
	else
	{
		flash.y = 0.0f;
	}

	for (int i = 0; i < pillarParticleTransform.size(); ++i)
	{
		pillarParticleModel[i]->updateCommonData.flash.x = flash.x;
		pillarParticleModel[i]->updateCommonData.flash.y = flash.y;


		pillarParticleTransform[i].pos.z += -5.0f;
		if (pillarParticleTransform[i].pos.z <= -10000.0f)
		{
			pillarParticleTransform[i].pos.z = 12500.0f;
		}

		pillarParticleMotherMat[i] = pillarParticleTransform[i].GetMat();
		pillarParticleModel[i]->Update(false);
	}


	for (int i = 0; i < transformArrayData.size(); ++i)
	{
		transformArrayData[i].pos.z += -0.5f;
		transformMatArray[i] = transformArrayData[i].GetMat();

		//一定の座標になったら透明になる。それ以外は実体化させる
		if (transformArrayData[i].pos.z <= -100.0f)
		{
			colorArrayData[i].alpha += -0.01f;
		}
		else
		{
			colorArrayData[i].alpha += 0.01f;
		}

		//透明になったらループさせる
		if (colorArrayData[i].alpha <= 0.0f)
		{
			transformArrayData[i].pos.z = 500.0f;
		}
		//アルファ固定
		if (1.0f <= colorArrayData[i].alpha)
		{
			colorArrayData[i].alpha = 1.0f;
		}


		colorArrayData[i].lightData.y++;
		if (100 <= colorArrayData[i].lightData.y)
		{
			colorArrayData[i].lightData.y = -100;
		}

		if (KazMath::ConvertSecondToFlame(5) <= flashTimer)
		{
			colorArrayData[i].lightData.x = 1;
			flashTimer = 0;
		}
		else
		{
			++flashTimer;
			colorArrayData[i].lightData.x = 0;
		}
	}



}

void BlockParticleStage::Draw()
{
	//GraphicsPipeLineMgr::Instance()->SetPipeLineAndRootSignature(PIPELINE_NAME_GPUPARTICLE);
	//DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	//DirectX12CmdList::Instance()->cmdList->IASetIndexBuffer(&indexBufferView);

	//RenderTargetStatus::Instance()->ChangeBarrier(
	//	buffers->GetBufferData(drawCommandHandle).Get(),
	//	D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
	//	D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT
	//);

	//DirectX12CmdList::Instance()->cmdList->ExecuteIndirect
	//(
	//	commandSig.Get(),
	//	1,
	//	buffers->GetBufferData(drawCommandHandle).Get(),
	//	0,
	//	nullptr,
	//	0
	//);

	//RenderTargetStatus::Instance()->ChangeBarrier(
	//	buffers->GetBufferData(drawCommandHandle).Get(),
	//	D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
	//	D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	//);

	for (int i = 0; i < pillarParticleModel.size(); ++i)
	{
		//pillarParticleModel[i]->Draw();
	}
	for (int i = 0; i < floorParticleModel.size(); ++i)
	{
		//floorParticleModel[i]->Draw();
	}
	for (int i = 0; i < splineParticle.size(); ++i)
	{
		//splineParticle[i]->Draw();
	}


	//galacticParticle->Draw();

}