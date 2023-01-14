#include "InstanceMeshCollision.h"
#include"../KazLibrary/Render/GPUParticleRender.h"
InstanceMeshCollision::InstanceMeshCollision(const std::vector<ResouceBufferHelper::BufferData> &VERT_ARRAY, const std::vector<UINT> &VERT_NUM_ARRAY)
{
	//BBを作成する
	for (int i = 0; i < VERT_ARRAY.size(); ++i)
	{
		bb.push_back(BoundingBox(VERT_ARRAY[i], VERT_NUM_ARRAY[i]));
		bb[i].Compute();
	}
}

void InstanceMeshCollision::Init()
{

	//メッシュパーティクルの当たり判定生成ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	for (int i = 0; i < bb.size(); ++i)
	{
		//メッシュ球生成
		generateMeshHitBox.push_back(BBDuringEquallyCoordinatePlace(bb[i].GetBBBuffer(), bb[i].GetData()));

#ifdef DEBUG
		generateMeshHitBox[i].SetDebugDraw(GPUParticleRender::Instance()->GetStackBuffer());
#endif
		generateMeshHitBox[i].Compute();

		//パーティクルとリンク付け
		//linkMeshHitBoxAndParticle.push_back(GenerateCollisionOfParticle(generateMeshHitBox[i].GetHitBoxPosData(), {}));
		//linkMeshHitBoxAndParticle[i].Compute();
	}
	//メッシュパーティクルの当たり判定生成ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー

}

void InstanceMeshCollision::Compute()
{
	//メッシュ球とパーティクルの親子関係

	//メッシュ球と対象の当たり判定


	//衝突判定が取れたパーティクルの挙動(ここで描画クラスに渡す)

}
