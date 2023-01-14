#include "InstanceMeshCollision.h"
#include"../KazLibrary/Render/GPUParticleRender.h"
InstanceMeshCollision::InstanceMeshCollision(const std::vector<ResouceBufferHelper::BufferData> &VERT_ARRAY, const std::vector<UINT> &VERT_NUM_ARRAY)
{
	//BB���쐬����
	for (int i = 0; i < VERT_ARRAY.size(); ++i)
	{
		bb.push_back(BoundingBox(VERT_ARRAY[i], VERT_NUM_ARRAY[i]));
		bb[i].Compute();
	}
}

void InstanceMeshCollision::Init()
{

	//���b�V���p�[�e�B�N���̓����蔻�萶���[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	for (int i = 0; i < bb.size(); ++i)
	{
		//���b�V��������
		generateMeshHitBox.push_back(BBDuringEquallyCoordinatePlace(bb[i].GetBBBuffer(), bb[i].GetData()));

#ifdef DEBUG
		generateMeshHitBox[i].SetDebugDraw(GPUParticleRender::Instance()->GetStackBuffer());
#endif
		generateMeshHitBox[i].Compute();

		//�p�[�e�B�N���ƃ����N�t��
		//linkMeshHitBoxAndParticle.push_back(GenerateCollisionOfParticle(generateMeshHitBox[i].GetHitBoxPosData(), {}));
		//linkMeshHitBoxAndParticle[i].Compute();
	}
	//���b�V���p�[�e�B�N���̓����蔻�萶���[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

}

void InstanceMeshCollision::Compute()
{
	//���b�V�����ƃp�[�e�B�N���̐e�q�֌W

	//���b�V�����ƑΏۂ̓����蔻��


	//�Փ˔��肪��ꂽ�p�[�e�B�N���̋���(�����ŕ`��N���X�ɓn��)

}
