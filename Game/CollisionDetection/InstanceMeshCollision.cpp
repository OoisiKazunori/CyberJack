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
	//���b�V���p�[�e�B�N���̓����蔻�萶���[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	for (int i = 0; i < meshData.size(); ++i)
	{
		//���b�V��������
		generateMeshHitBox.emplace_back(BBDuringEquallyCoordinatePlace(meshData[i].bb.GetBBBuffer(), meshData[i].bb.GetData(), cpuAndMeshCircleHitBox->GetStackMeshCircleBuffer()));

#ifdef DEBUG
		generateMeshHitBox[i].SetDebugDraw(GPUParticleRender::Instance()->GetStackBuffer());
#endif
		generateMeshHitBox[i].Compute();

		//�p�[�e�B�N���ƃ����N�t��
		linkMeshHitBoxAndParticle.emplace_back(GenerateCollisionOfParticle(generateMeshHitBox[i].GetHitBoxPosData(), meshData[i].meshParticle.GetBuffer(), particleAvoidParticle.GetStackParticleHitBoxBuffer()));
		linkMeshHitBoxAndParticle[i].Compute();
	}
	//���b�V���p�[�e�B�N���̓����蔻�萶���[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	particleAvoidParticle.SetHitIDBuffer(cpuAndMeshCircleHitBox->GetStackIDBuffer());
}

void InstanceMeshCollision::Compute()
{
	//meshData[0].meshParticle.Compute();

	//linkMeshHitBoxAndParticle[0].Compute();
	//���b�V�����ƃp�[�e�B�N���̐e�q�֌W

	//���b�V�����ƑΏۂ̓����蔻��
	cpuAndMeshCircleHitBox->Compute();

	//���b�V����
	//generateMeshHitBox[0].Compute();

	//�Փ˔��肪��ꂽ�p�[�e�B�N���̋���(�����ŕ`��N���X�ɓn��)
	particleAvoidParticle.Compute();
}
