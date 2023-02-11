#include "InstanceMeshParticle.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Render/GPUParticleRender.h"

int InstanceMeshParticle::MESH_PARTICLE_GENERATE_NUM = 0;

InstanceMeshParticle::InstanceMeshParticle() :setCountNum(0)
{
	//メッシュパーティクルの初期化処理の出力情報
	meshParticleBufferData = computeInitMeshParticle.CreateAndGetBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(InitOutputData) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		sizeof(InitOutputData),
		PARTICLE_MAX_NUM,
		true);

	//パーティクルデータ
	computeUpdateMeshParticle.SetBuffer(meshParticleBufferData, GRAPHICS_PRAMTYPE_DATA);



	UINT lNum = 0;
	KazBufferHelper::BufferResourceData lBufferData
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(UINT)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		"CopyCounterBuffer"
	);

	copyBuffer.CreateBuffer(lBufferData);
	copyBuffer.TransData(&lNum, sizeof(UINT));

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(meshParticleBufferData.counterWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_COPY_DEST
		)
	);

	DirectX12CmdList::Instance()->cmdList->CopyResource(meshParticleBufferData.counterWrapper.buffer.Get(), copyBuffer.buffer.Get());

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(meshParticleBufferData.counterWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		)
	);





	KazBufferHelper::BufferResourceData lData(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(MOTHER_MAT_MAX, sizeof(DirectX::XMMATRIX))),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		"VRAMmatData"
	);
	lData.resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	//親行列
	particleMotherMatrixHandle = computeUpdateMeshParticle.CreateBuffer(
		lData,
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(DirectX::XMMATRIX),
		static_cast<UINT>(MOTHER_MAT_MAX),
		false
	);



	motherMatrixBuffer.CreateBuffer(
		KazBufferHelper::BufferResourceData(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			CD3DX12_RESOURCE_DESC::Buffer(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(MOTHER_MAT_MAX, sizeof(DirectX::XMMATRIX))),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			"RAMmatData")
	);



	//ワールド行列
	computeUpdateMeshParticle.SetBuffer(GPUParticleRender::Instance()->GetStackBuffer(), GRAPHICS_PRAMTYPE_DATA3);
	//色
	//computeUpdateMeshParticle.SetBuffer(GPUParticleRender::Instance()->GetStackColorBuffer(), GRAPHICS_PRAMTYPE_DATA4);
	//Transformを除いたワールド行列
	scaleRotateBillboardMatHandle = computeUpdateMeshParticle.CreateBuffer(
		sizeof(DirectX::XMMATRIX),
		GRAPHICS_RANGE_TYPE_CBV_VIEW,
		GRAPHICS_PRAMTYPE_DATA4,
		1,
		false);

	float lScale = 0.1f;
	scaleRotMat = KazMath::CaluScaleMatrix({ lScale,lScale,lScale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });
}

void InstanceMeshParticle::Init()
{
}

void InstanceMeshParticle::AddMeshData(const InitMeshParticleData &DATA)
{
#pragma region 初期化用のバッファ生成

	commonAndColorBufferData = computeInitMeshParticle.CreateAndGetBuffer(
		COMMON_BUFFER_SIZE,
		GRAPHICS_RANGE_TYPE_CBV_VIEW,
		GRAPHICS_PRAMTYPE_DATA,
		1,
		false
	);

	commonBufferData.push_back(
		computeInitMeshParticle.CreateAndGetBuffer(
			sizeof(CommonData),
			GRAPHICS_RANGE_TYPE_CBV_VIEW,
			GRAPHICS_PRAMTYPE_DATA,
			1,
			false
		));


	motherMatArray.push_back(DATA.motherMat);



	//何の情報を読み込むかでパイプラインの種類を変える
	computeInitMeshParticle.DeleteAllData();
	setCountNum = 0;

	IsSetBuffer(DATA.vertData);
	IsSetBuffer(DATA.uvData);


	ComputePipeLineNames lPipelineName = PIPELINE_COMPUTE_NAME_NONE;
	RESOURCE_HANDLE lCommonHandle;
	CommonWithColorData lCommonAndColorData;
	CommonData lCommonData;
	switch (setCountNum)
	{
	case 0:
		//メッシュパーティクルに必要な情報が何も入ってない
		assert(0);
		break;
	case 1:
		lCommonHandle = computeInitMeshParticle.SetBuffer(commonAndColorBufferData, GRAPHICS_PRAMTYPE_DATA3);

		lCommonAndColorData.meshData = DATA.triagnleData;
		lCommonAndColorData.color = DATA.color.ConvertXMFLOAT4();
		lCommonAndColorData.id = static_cast<UINT>(MESH_PARTICLE_GENERATE_NUM);
		computeInitMeshParticle.TransData(lCommonHandle, &lCommonAndColorData, sizeof(CommonWithColorData));

		lPipelineName = PIPELINE_COMPUTE_NAME_INIT_POS_MESHPARTICLE;
		break;
	case 2:
		lCommonHandle = computeInitMeshParticle.SetBuffer(commonBufferData[MESH_PARTICLE_GENERATE_NUM], GRAPHICS_PRAMTYPE_DATA4);

		lCommonData.meshData = DATA.triagnleData;
		lCommonData.id = static_cast<UINT>(MESH_PARTICLE_GENERATE_NUM);
		computeInitMeshParticle.TransData(lCommonHandle, &lCommonData, sizeof(CommonData));

		lPipelineName = PIPELINE_COMPUTE_NAME_INIT_POSUV_MESHPARTICLE;
		break;
	default:
		break;
	}

	meshParticleOutputHandle = computeInitMeshParticle.SetBuffer(meshParticleBufferData, static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + setCountNum));

	//テクスチャのセット
	if (DATA.textureHandle != -1)
	{
		ResouceBufferHelper::BufferData lData;
		lData.viewHandle = DATA.textureHandle;
		lData.rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
		lData.rootParamType = GRAPHICS_PRAMTYPE_TEX;
		computeInitMeshParticle.SetBuffer(lData, GRAPHICS_PRAMTYPE_TEX);
	}

	computeInitMeshParticle.StackToCommandListAndCallDispatch(lPipelineName, { 1000,1,1 });

	++MESH_PARTICLE_GENERATE_NUM;
#pragma endregion
}

void InstanceMeshParticle::Compute()
{
	scaleRotBillBoardMat = scaleRotMat * CameraMgr::Instance()->GetMatBillBoard();
	computeUpdateMeshParticle.TransData(scaleRotateBillboardMatHandle, &scaleRotBillBoardMat, sizeof(DirectX::XMMATRIX));

	std::vector<DirectX::XMMATRIX>lMatArray(motherMatArray.size());
	for (int i = 0; i < lMatArray.size(); ++i)
	{
		lMatArray[i] = *motherMatArray[i];
	}
	motherMatrixBuffer.TransData(lMatArray.data(), sizeof(DirectX::XMMATRIX) * static_cast<int>(lMatArray.size()));



	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(computeUpdateMeshParticle.GetBufferData(particleMotherMatrixHandle).bufferWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_COPY_DEST
		)
	);

	DirectX12CmdList::Instance()->cmdList->CopyResource(computeUpdateMeshParticle.GetBufferData(particleMotherMatrixHandle).bufferWrapper.buffer.Get(), motherMatrixBuffer.buffer.Get());

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(computeUpdateMeshParticle.GetBufferData(particleMotherMatrixHandle).bufferWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		)
	);

	computeUpdateMeshParticle.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_UPDATE_MESHPARTICLE, { 1000,1,1 });
}