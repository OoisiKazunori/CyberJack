#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/Collision.hlsli"

cbuffer RootConstants : register(b0)
{
    float3 pos;
    float radius;
    float particleRadius;
    uint xMax;
    uint xyMax;
};

//メッシュ当たり判定
RWStructuredBuffer<MeshHitBox> meshHitBoxPos : register(u0);
RWStructuredBuffer<uint3> meshHitBoxID : register(u1);

//何処の球と判定を取ったか
AppendStructuredBuffer<uint3> hitBoxData : register(u1);

//メッシュパーティクルと球の判定
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    CircleData meshHitBox;
    meshHitBox.pos = meshHitBoxPos[index];
    meshHitBox.radius = particleRadius;

    CircleData hitBox;
    hitBox.pos = pos;
    hitBox.radius = radius;

    //当たり判定が取れたら当たったインデックスを保存し、衝突後の処理に使う
    if(CheckCircleAndCircle(meshHitBox,hitBox))
    {
        hitBoxData.Append(meshHitBoxID[index]);
    }
}
