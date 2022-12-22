#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/Collision.hlsli"

cbuffer RootConstants : register(b0)
{
    uint meshHitBoxNum;
    float meshHitBoxRadius;
};

//メッシュ当たり判定
RWStructuredBuffer<float3> meshHitBoxPos : register(u0);
RWStructuredBuffer<uint3> meshHitBoxID : register(u1);

//パーティクル当たり判定
RWStructuredBuffer<float3> particleHitBoxPos : register(u2);
RWStructuredBuffer<uint3> particleHitBoxID : register(u3);

//メッシュパーティクルと球の判定
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);
    
    CircleData particleHitBox;
    particleHitBox.pos = particleHitBoxPos[index];
    const float PARTICLE_RADIUS = 1.0f;
    particleHitBox.radius = PARTICLE_RADIUS;

    CircleData meshHitBox;    
    meshHitBox.radius = meshHitBoxRadius;
    for(int i = 0;i < meshHitBoxNum; ++i)
    {
        meshHitBox.pos = meshHitBoxPos[i];

        if(CheckCircleAndCircle(meshHitBox,particleHitBox))
        {
            particleHitBoxID[index] = meshHitBoxID[i];
        }
    }
}
