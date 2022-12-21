#include "InstanceMeshParticle.h"

InstanceMeshParticle::InstanceMeshParticle(std::vector<InitMeshParticleData> &INIT_DATA)
{
#pragma region 初期化用のバッファ生成
	//メッシュパーティクルの初期化処理の入力情報---------------------------------------
	vertHandle = computeInitMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(VERT_BUFFER_SIZE * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		VERT_BUFFER_SIZE,
		PARTICLE_MAX_NUM);

	uvHandle = computeInitMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(UV_BUFFER_SIZE * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		UV_BUFFER_SIZE,
		PARTICLE_MAX_NUM
	);

	commonHandle = computeInitMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(COMMON_BUFFER_SIZE * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		COMMON_BUFFER_SIZE,
		PARTICLE_MAX_NUM
	);

	//メッシュパーティクルの初期化処理の出力情報
	meshParticleOutputHandle = computeInitMeshParticle.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(InitOutputData) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA4,
		sizeof(InitOutputData),
		PARTICLE_MAX_NUM);

	meshParticleIDHandle = computeInitMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMUINT2) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA5,
		sizeof(DirectX::XMUINT2),
		PARTICLE_MAX_NUM);

	//単色のみの対応
	ComputeBufferHelper::BufferData lData;
	lData.viewHandle = INIT_DATA[0].textureViewHandle;
	lData.rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
	computeInitMeshParticle.SetBuffer(lData, GRAPHICS_PRAMTYPE_TEX);

#pragma endregion


#pragma region 更新用のバッファ生成
	//パーティクル更新処理
	computeUpdateMeshParticle.SetBuffer(computeInitMeshParticle.GetBufferData(meshParticleOutputHandle), GRAPHICS_PRAMTYPE_DATA);


	motherMatrixHandle = computeUpdateMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMMATRIX) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_VIEW,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(DirectX::XMMATRIX),
		PARTICLE_MAX_NUM);



	//パーティクル座標出力
	particlePosHandle = computeUpdateMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(VERT_BUFFER_SIZE * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		VERT_BUFFER_SIZE,
		PARTICLE_MAX_NUM);


	//パーティクルカラー出力
	particleColorHandle = computeUpdateMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMFLOAT4) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA4,
		sizeof(DirectX::XMFLOAT4),
		PARTICLE_MAX_NUM);


	//パーティクルに合わせてリンクする親行列
	particleMotherMatrixHandle = computeUpdateMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMMATRIX) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA5,
		sizeof(DirectX::XMMATRIX),
		PARTICLE_MAX_NUM);
#pragma endregion



	UINT64 lVertBufferSize = 0;
	UINT64 lUvBufferSize = 0;
	//初期化時のバッファをどんどんスタックしていく
	for (int i = 0; i < INIT_DATA.size(); ++i)
	{
		DirectX12CmdList::Instance()->cmdList->CopyBufferRegion(
			computeInitMeshParticle.GetBufferData(vertHandle).bufferWrapper.buffer.Get(),
			lVertBufferSize,										//恐らくコピー先始まりの位置
			INIT_DATA[i].vertData->bufferWrapper.buffer.Get(),
			0,													//恐らくコピー元始まりの位置
			INIT_DATA[i].vertData->bufferSize					//恐らくコピー元終わりの位置
		);
		lVertBufferSize += INIT_DATA[i].vertData->bufferSize;


		DirectX12CmdList::Instance()->cmdList->CopyBufferRegion(
			computeInitMeshParticle.GetBufferData(uvHandle).bufferWrapper.buffer.Get(),
			lUvBufferSize,										//恐らくコピー先始まりの位置
			INIT_DATA[i].uvData->bufferWrapper.buffer.Get(),
			0,													//恐らくコピー元始まりの位置
			INIT_DATA[i].uvData->bufferSize					//恐らくコピー元終わりの位置
		);
		lUvBufferSize += INIT_DATA[i].uvData->bufferSize;

		computeInitMeshParticle.TransData(commonHandle, &INIT_DATA[i].triagnleData, 0);

		motherMatArray.emplace_back(INIT_DATA[i].motherMat);
	}

	computeUpdateMeshParticle.TransData(particleMotherMatrixHandle, motherMatArray.data(), 0);



	computeInitMeshParticle.Compute(PIPELINE_COMPUTE_NAME_INIT_MESHPARTICLE, { 1000,1,1 });
}

void InstanceMeshParticle::Compute()
{
	computeUpdateMeshParticle.TransData(particleMotherMatrixHandle, motherMatArray.data(), 0);
	computeUpdateMeshParticle.Compute(PIPELINE_COMPUTE_NAME_UPDATE_MESHPARTICLE, { 1000,1,1 });
}