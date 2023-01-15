#pragma once
#include"../KazLibrary/Helper/ResouceBufferHelper.h"

/// <summary>
/// メッシュ球判定とパーティクルを紐づける
/// </summary>
class GenerateCollisionOfParticle
{
public:
	GenerateCollisionOfParticle(
		const ResouceBufferHelper::BufferData &HITBOX_DATA,
		const ResouceBufferHelper::BufferData &PARTICLE_DATA
	);
	void Compute();

private:
	ResouceBufferHelper computeHelper;
	struct ParticleHitBoxData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		UINT meshID;
		DirectX::XMUINT3 id;
	};

	struct CommonData
	{
		float particleRadius;
		float meshHitBoxRadius;
		UINT meshHitBoxMaxNum;
	};

	static const int PARTICLE_MAX_NUM = 100000;
};