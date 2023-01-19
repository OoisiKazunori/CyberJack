#include "InstanceMeshCollision.h"
#include"../KazLibrary/Render/GPUParticleRender.h"

InstanceMeshCollision::InstanceMeshCollision(const std::vector<InitMeshCollisionData> &INIT_DATA)
{
	//BBを作成する
	for (int i = 0; i < INIT_DATA.size(); ++i)
	{
		//メッシュパーティクル生成
		meshData.emplace_back(MeshParticleData(INIT_DATA[i].vertData, INIT_DATA[i].vertNumArray, INIT_DATA[i].meshParticleData, i));
		//BB生成
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
	//メッシュパーティクルの当たり判定生成ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	for (int i = 0; i < meshData.size(); ++i)
	{
		//メッシュ球生成
		generateMeshHitBox.emplace_back(BBDuringEquallyCoordinatePlace(meshData[i].bb.GetBBBuffer(), meshData[i].bb.GetData(), cpuAndMeshCircleHitBox->GetStackMeshCircleBuffer()));

#ifdef DEBUG
		generateMeshHitBox[i].SetDebugDraw(GPUParticleRender::Instance()->GetStackBuffer());
#endif
		generateMeshHitBox[i].Compute();

		//パーティクルとリンク付け
		linkMeshHitBoxAndParticle.emplace_back(GenerateCollisionOfParticle(generateMeshHitBox[i].GetHitBoxPosData(), meshData[i].meshParticle.GetBuffer(), particleAvoidParticle.GetStackParticleHitBoxBuffer()));
		linkMeshHitBoxAndParticle[i].Compute();
	}
	//メッシュパーティクルの当たり判定生成ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

	particleAvoidParticle.SetHitIDBuffer(cpuAndMeshCircleHitBox->GetStackIDBuffer());
}

void InstanceMeshCollision::Compute()
{
	//meshData[0].meshParticle.Compute();

	//linkMeshHitBoxAndParticle[0].Compute();
	//メッシュ球とパーティクルの親子関係

	//メッシュ球と対象の当たり判定
	cpuAndMeshCircleHitBox->Compute();

	//メッシュ球
	//generateMeshHitBox[0].Compute();

	//衝突判定が取れたパーティクルの挙動(ここで描画クラスに渡す)
	particleAvoidParticle.Compute();
}
