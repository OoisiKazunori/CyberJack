#include "InstanceMeshParticle.h"

InstanceMeshParticle::InstanceMeshParticle(std::vector<InitMeshParticleData> &INIT_DATA)
{
#pragma region �������p�̃o�b�t�@����
	//���b�V���p�[�e�B�N���̏����������̓��͏��---------------------------------------
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

	//���b�V���p�[�e�B�N���̏����������̏o�͏��
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

	//�P�F�݂̂̑Ή�
	ComputeBufferHelper::BufferData lData;
	lData.viewHandle = INIT_DATA[0].textureViewHandle;
	lData.rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
	computeInitMeshParticle.SetBuffer(lData, GRAPHICS_PRAMTYPE_TEX);

#pragma endregion


#pragma region �X�V�p�̃o�b�t�@����
	//�p�[�e�B�N���X�V����
	computeUpdateMeshParticle.SetBuffer(computeInitMeshParticle.GetBufferData(meshParticleOutputHandle), GRAPHICS_PRAMTYPE_DATA);


	motherMatrixHandle = computeUpdateMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMMATRIX) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_VIEW,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(DirectX::XMMATRIX),
		PARTICLE_MAX_NUM);



	//�p�[�e�B�N�����W�o��
	particlePosHandle = computeUpdateMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(VERT_BUFFER_SIZE * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		VERT_BUFFER_SIZE,
		PARTICLE_MAX_NUM);


	//�p�[�e�B�N���J���[�o��
	particleColorHandle = computeUpdateMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMFLOAT4) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA4,
		sizeof(DirectX::XMFLOAT4),
		PARTICLE_MAX_NUM);


	//�p�[�e�B�N���ɍ��킹�ă����N����e�s��
	particleMotherMatrixHandle = computeUpdateMeshParticle.CreateBuffer(
		KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMMATRIX) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA5,
		sizeof(DirectX::XMMATRIX),
		PARTICLE_MAX_NUM);
#pragma endregion



	UINT64 lVertBufferSize = 0;
	UINT64 lUvBufferSize = 0;
	//���������̃o�b�t�@���ǂ�ǂ�X�^�b�N���Ă���
	for (int i = 0; i < INIT_DATA.size(); ++i)
	{
		DirectX12CmdList::Instance()->cmdList->CopyBufferRegion(
			computeInitMeshParticle.GetBufferData(vertHandle).bufferWrapper.buffer.Get(),
			lVertBufferSize,										//���炭�R�s�[��n�܂�̈ʒu
			INIT_DATA[i].vertData->bufferWrapper.buffer.Get(),
			0,													//���炭�R�s�[���n�܂�̈ʒu
			INIT_DATA[i].vertData->bufferSize					//���炭�R�s�[���I���̈ʒu
		);
		lVertBufferSize += INIT_DATA[i].vertData->bufferSize;


		DirectX12CmdList::Instance()->cmdList->CopyBufferRegion(
			computeInitMeshParticle.GetBufferData(uvHandle).bufferWrapper.buffer.Get(),
			lUvBufferSize,										//���炭�R�s�[��n�܂�̈ʒu
			INIT_DATA[i].uvData->bufferWrapper.buffer.Get(),
			0,													//���炭�R�s�[���n�܂�̈ʒu
			INIT_DATA[i].uvData->bufferSize					//���炭�R�s�[���I���̈ʒu
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