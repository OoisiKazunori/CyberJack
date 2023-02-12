#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"


//パーティクル当たり判定
RWStructuredBuffer<ParticleHitBoxData> hitBoxData : register(u0);
//当たったインデックス
RWStructuredBuffer<MeshSphereHitData> hitIndexData : register(u1);
//出力
AppendStructuredBuffer<ParticleData> inputGPUParticleData : register(u2);

RWStructuredBuffer<float3> larpPosData : register(u3);

//周りの当たり判定ともとる
bool CheckLinkHitBox(uint3 BASE_HIT_INDEX,uint3 CHECK_HIT_INDEX)
{
    bool linkHitFlagArray[7];
    linkHitFlagArray[0] = (bool)(CHECK_HIT_INDEX == BASE_HIT_INDEX);                    //真ん中
    linkHitFlagArray[1] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,1,0)));  //上
    linkHitFlagArray[2] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,-1,0))); //下
    linkHitFlagArray[3] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(-1,0,0))); //左
    linkHitFlagArray[4] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(1,0,0)));  //右
    linkHitFlagArray[5] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,0,1)));  //前
    linkHitFlagArray[6] = (bool)(CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,0,-1))); //後

    for(int i = 0;i < 7;++i)
    {
        if(linkHitFlagArray[i])
        {
            return true;
        }
    }
    return false;
}

//メッシュパーティクルの球判定を取ったインデックスを参考にパーティクルの挙動を決める
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    ParticleData particleData;
    particleData.pos = hitBoxData[index].pos;
    particleData.color = hitBoxData[index].color;
    particleData.id = hitBoxData[index].meshID;

    float larpVel = 0.1f;
    float3 basePos = hitBoxData[index].pos;
    //同じインデックスの場合、パーティクルを動かす処理を追加する
    for(int i = 0;i < 2; ++i)
    {
        //メッシュIDの確認
        if(particleData.id != hitIndexData[i].meshID)
        {
            continue;
        }
        //当たり判定+リンク付け込みの処理を取る
        if(CheckLinkHitBox(hitIndexData[i].id,hitBoxData[index].id))
        {    
            float3 posHitBoxVec = hitIndexData[i].circlePos - hitIndexData[i].meshPos;
            float3 particleHitPos = hitBoxData[index].pos - hitIndexData[i].meshPos;
            float3 posParticleVec = posHitBoxVec - particleHitPos;

            //なす角の判断
            float cos = dot(posParticleVec,posHitBoxVec) / (length(posParticleVec) * length(posHitBoxVec));
            float sin = acos(cos);
    
            int angle = RadianToAngle(sin) - 90;
            float rate = angle / 90.0f;

            posParticleVec.x = 0.0f;
            float3 vel = normalize(posParticleVec) * 5.5f * rate;
            larpVel = 0.1f;
            basePos = hitBoxData[index].pos + vel;
            
            //パーティクル情報の描画,当たったかどうかも表示する
            particleData.color = float4(1,0,0,1);
            break;            
        }
    }
    //行列計算ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
    
    larpPosData[index] = Larp(basePos,larpPosData[index],larpVel);
    if(isnan(larpPosData[index].x))
    {
    ///    larpPosData[index].x = particleData.pos.x;
    }
    if(isnan(larpPosData[index].y))
    {
    //    larpPosData[index].y = particleData.pos.y;
    }
    if(isnan(larpPosData[index].z))
    {
    //    larpPosData[index].z = particleData.pos.z;
    }
    //particleData.pos = larpPosData[index];
    particleData.pos = basePos;
    particleData.color = float4(1,1,1,1);
    inputGPUParticleData.Append(particleData);
}
