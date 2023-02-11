#include "InstanceMeshCollision.h"
#include"../KazLibrary/Render/GPUParticleRender.h"

InstanceMeshCollision::InstanceMeshCollision(const std::vector<InitMeshCollisionData> &INIT_DATA)
{
	//BB���쐬����
	for (int i = 0; i < INIT_DATA.size(); ++i)
	{
		//���b�V���p�[�e�B�N������
		meshData.emplace_back(MeshParticleData(INIT_DATA[i].vertData, INIT_DATA[i].vertNumArray, INIT_DATA[i].meshParticleData, i));
		//BB����
		meshData[i].bb.Compute();

		hitBoxData.emplace_back(INIT_DATA[i].hitBox);


		motherMatArray.push_back(INIT_DATA[i].motherMat);
	}

	cpuAndMeshCircleHitBox = std::make_unique<CollisionDetectionOfMeshCircleAndCPUHitBox>(hitBoxData);


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
}

void InstanceMeshCollision::Init()
{
	inputMeshCircleBufferHandle = meshMoveCompute.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(MeshHitBoxData) * 10000),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		sizeof(MeshHitBoxData),
		10000,
		true
	);

	outputMeshCircleBufferHandle = meshMoveCompute.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(MeshHitBoxData) * 10000),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		sizeof(MeshHitBoxData),
		10000,
		false
	);

	//���b�V���p�[�e�B�N���̓����蔻�萶���[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	for (int i = 0; i < meshData.size(); ++i)
	{
		//���b�V��������
		generateMeshHitBox.emplace_back(BBDuringEquallyCoordinatePlace(meshData[i].bb.GetBBBuffer(), meshData[i].bb.GetData(), meshMoveCompute.GetBufferData(inputMeshCircleBufferHandle)));

#ifdef DEBUG
		generateMeshHitBox[i].SetDebugDraw(GPUParticleRender::Instance()->GetStackBuffer());
#endif
		generateMeshHitBox[i].Compute();

		//�p�[�e�B�N���ƃ����N�t��
		linkMeshHitBoxAndParticle.emplace_back(GenerateCollisionOfParticle(generateMeshHitBox[i].GetHitBoxPosData(), meshData[i].meshParticle.GetBuffer(), particleAvoidParticle.GetStackParticleHitBoxBuffer()));
		linkMeshHitBoxAndParticle[i].Compute();
	}
	//���b�V���p�[�e�B�N���̓����蔻�萶���[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	//�ړ����buffer��n��
	cpuAndMeshCircleHitBox->SetStackMeshCircleBuffer(meshMoveCompute.GetBufferData(outputMeshCircleBufferHandle));

	//�����Ɠ����������̏��L�^
	particleAvoidParticle.SetHitIDBuffer(cpuAndMeshCircleHitBox->GetStackIDBuffer());



	updatePosCompute.SetBuffer(particleAvoidParticle.GetOutputParticleData(), GRAPHICS_PRAMTYPE_DATA);
	motherMatHandle = updatePosCompute.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(KazBufferHelper::GetBufferSize<UINT>(motherMatArray.size(), sizeof(DirectX::XMMATRIX))),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(DirectX::XMMATRIX),
		static_cast<UINT>(motherMatArray.size())
	);

	updatePosCompute.SetBuffer(GPUParticleRender::Instance()->GetStackBuffer(), GRAPHICS_PRAMTYPE_DATA3);

	scaleRotateBillboardMatHandle = updatePosCompute.CreateBuffer(
		sizeof(DirectX::XMMATRIX),
		GRAPHICS_RANGE_TYPE_CBV_VIEW,
		GRAPHICS_PRAMTYPE_DATA4,
		1,
		false);

	float lScale = 0.02f;
	scaleRotaMat = KazMath::CaluScaleMatrix({ lScale,lScale,lScale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });

	//�e�s��]���p
	motherMatrixBuffer.bufferWrapper.CreateBuffer(
		KazBufferHelper::BufferResourceData(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			CD3DX12_RESOURCE_DESC::Buffer(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(motherMatArray.size(), sizeof(DirectX::XMMATRIX))),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			"RAMmatData")
	);



	meshMoveCompute.SetBuffer(updatePosCompute.GetBufferData(motherMatHandle), GRAPHICS_PRAMTYPE_DATA2);

	//cpuAndMeshCircleHitBox
		//particleAvoidParticle
}

void InstanceMeshCollision::Compute()
{
	for (int i = 0; i < meshData.size(); ++i)
	{
		//meshData[i].meshParticle.Compute();

		//���b�V�����ƃp�[�e�B�N���̐e�q�֌W
		//linkMeshHitBoxAndParticle[i].Compute();
		//���b�V����
		//generateMeshHitBox[i].Compute();
	}

#pragma region �e�s��̓]��

	std::vector<DirectX::XMMATRIX>lMatArray(motherMatArray.size());
	for (int i = 0; i < lMatArray.size(); ++i)
	{
		lMatArray[i] = *motherMatArray[i];
	}
	motherMatrixBuffer.bufferWrapper.TransData(lMatArray.data(), sizeof(DirectX::XMMATRIX) * static_cast<int>(lMatArray.size()));



	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(updatePosCompute.GetBufferData(motherMatHandle).bufferWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_COPY_DEST
		)
	);

	DirectX12CmdList::Instance()->cmdList->CopyResource(updatePosCompute.GetBufferData(motherMatHandle).bufferWrapper.buffer.Get(), motherMatrixBuffer.bufferWrapper.buffer.Get());

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(updatePosCompute.GetBufferData(motherMatHandle).bufferWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		)
	);
#pragma endregion

	scaleRotBillBoardMat = scaleRotaMat * CameraMgr::Instance()->GetMatBillBoard();
	updatePosCompute.TransData(scaleRotateBillboardMatHandle, &scaleRotBillBoardMat, sizeof(DirectX::XMMATRIX));
	//���b�V���p�[�e�B�N���̍��W�ړ�
	updatePosCompute.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_UPDATE_MESHPARTICLE, { 1000,1,1 });


	//���b�V�����̈ړ�
	meshMoveCompute.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_MESHCIRCLE_MOVE, { 10,1,1 });


	//���b�V�����ƑΏۂ̓����蔻��
	cpuAndMeshCircleHitBox->Compute();

	//�Փ˔��肪��ꂽ�p�[�e�B�N���̋���(�����ŕ`��N���X�ɓn��)
	particleAvoidParticle.Compute();
}
