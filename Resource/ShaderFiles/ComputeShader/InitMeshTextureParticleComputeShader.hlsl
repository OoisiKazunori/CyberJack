#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct OutputData
{
    float3 pos;
    float4 color;
};

struct InputData
{
    float3 pos;
    float2 uv;
};

//�X�V
RWStructuredBuffer<InputData> vertciesData : register(u0);
//�o��
RWStructuredBuffer<OutputData> worldPosData : register(u1);

cbuffer RootConstants : register(b0)
{
    float4 pos;
    uint vertMaxNum;
    uint bias;
};

Texture2D<float4> tex : register(t2);
SamplerState smp : register(s0);

float Cross(float3 VEC_1,float3 VEC_2)
{
    float3 result;
    result.x = VEC_1.y * VEC_2.z - VEC_2.y * VEC_1.z;
    result.y = VEC_1.z * VEC_2.x - VEC_2.z * VEC_1.x;
    result.z = VEC_1.x * VEC_2.y - VEC_2.x * VEC_1.y;
    return result;
}

float CalucurateTriangleArea(float3 P0,float3 P1,float3 P2)
{
    float3 p0p1Vec = P1 - P0;
    float3 p1p2Vec = P2 - P1;
    return Cross(p0p1Vec,p1p2Vec) / 2.0f;
}

float CalucurateUVW(float3 P0,float3 P1,float3 ATTACK_POINT,float TRIANGLE_AREA)
{
    float rate = CalucurateTriangleArea(P0,P1,ATTACK_POINT) / TRIANGLE_AREA;
    return P0 * rate;
}

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(2 <= index)
    {
        return;
    }

    //�C���f�b�N�X���ȓ��Ȃ珈������
    //�O�p�`���\������C���f�b�N�X�̎w��--------------------------------------------
    uint firstVertIndex = index * 3;
    uint secondVertIndex = index * 3 + 1;
    uint thirdVertIndex = index * 3 + 2;
    //�O�p�`���\������C���f�b�N�X�̎w��--------------------------------------------

    //���_���W���烏�[���h���W�ɕϊ���----------------------------------------------
    float4 firstVertWorldPos = GetPos(vertciesData[firstVertIndex].pos.xyz,pos.xyz);
    float4 secondVertWorldPos = GetPos(vertciesData[secondVertIndex].pos.xyz,pos.xyz);
    float4 thirdVertWorldPos = GetPos(vertciesData[thirdVertIndex].pos.xyz,pos.xyz);
    //���_���W���烏�[���h���W�ɕϊ�------------------------------------------------

    //�O�p�`�̖ʐς��v�Z
    float triangleArea = CalucurateTriangleArea(firstVertWorldPos.xyz,secondVertWorldPos.xyz,thirdVertWorldPos.xyz);

    //�O�p�`���\�����郌�C--------------------------------------------
    const int RAY_MAX_NUM = 3;
    const int RAY_POS_MAX_NUM = 2;
    float3 triangleRay[RAY_MAX_NUM][RAY_POS_MAX_NUM];
    triangleRay[0][0] = firstVertWorldPos.xyz;
    triangleRay[0][1] = secondVertWorldPos.xyz;
    triangleRay[1][0] = secondVertWorldPos.xyz;
    triangleRay[1][1] = thirdVertWorldPos.xyz;
    triangleRay[2][0] = thirdVertWorldPos.xyz;
    triangleRay[2][1] = firstVertWorldPos.xyz;

    //�O�p�`���\�����郌�C--------------------------------------------

    //�d�S���W
    float3 triangleCentralPos = (firstVertWorldPos.xyz + secondVertWorldPos.xyz + thirdVertWorldPos.xyz) / 3.0f;
 
    //�p�[�e�B�N���̔z�u--------------------------------------------
    const int PARTICLE_MAX_NUM = 1000;
    const int PER_PARTICLE_MAX_NUM = PARTICLE_MAX_NUM / 3;
    for(int rayIndex = 0; rayIndex < RAY_MAX_NUM; ++rayIndex)
    {
        //��ӂ̒���
        float3 distance = triangleRay[rayIndex][1] - triangleRay[rayIndex][0];

        for(int particleIndex = 0; particleIndex < PER_PARTICLE_MAX_NUM; ++particleIndex)
        {
            uint outputIndex = index * PARTICLE_MAX_NUM + rayIndex * PER_PARTICLE_MAX_NUM + particleIndex;
            float rate = RandVec3(outputIndex + 100,1,0).x;
            //�n�_�Z�o
            float3 startPos = triangleRay[rayIndex][0] + distance * rate;
            //�I�_�Z�o
            float3 endPos = triangleCentralPos;
            //��ӂ̂Ƃ���ʒu����d�S���W�܂ł̋���
            float3 resultDistance = endPos - startPos;

            //�p�[�e�B�N���̔z�u
            float3 resultPos;
            const int PARTICLE_MAX_BIAS = 100;
            const int RANDOM_NUMBER_BIAS = bias;
            
            if(RandVec3(outputIndex,PARTICLE_MAX_BIAS,0).x <= RANDOM_NUMBER_BIAS)
            {
                //�G�b�W���ӂɕ΂点��
                rate = RandVec3(outputIndex + 1000,10,0).x / 100.0f;
                resultPos = startPos + resultDistance * rate;

                //���W����UV�����
                float2 uv;
                uv.x = CalucurateUVW(firstVertWorldPos.xyz,secondVertWorldPos.xyz,resultPos,triangleArea);
                uv.y = CalucurateUVW(secondVertWorldPos.xyz,thirdVertWorldPos.xyz,resultPos,triangleArea);

                worldPosData[outputIndex].pos.xyz = resultPos;
                worldPosData[outputIndex].color = tex.SampleLevel(smp,uv,0);
            }
            else
            {
                //�ʎ��ӂɕ΂点��
                rate = RandVec3(startPos.y * 10.0f + outputIndex + 10000,1,0).x;
                resultPos = startPos + resultDistance * rate;

                //���W����UV�����
                float2 uv;
                uv.x = CalucurateUVW(firstVertWorldPos.xyz,secondVertWorldPos.xyz,resultPos,triangleArea);
                uv.y = CalucurateUVW(secondVertWorldPos.xyz,thirdVertWorldPos.xyz,resultPos,triangleArea);

                worldPosData[outputIndex].pos.xyz = resultPos;
                worldPosData[outputIndex].color = tex.SampleLevel(smp,uv,0);
            }
        }
    }
    //�p�[�e�B�N���̔z�u--------------------------------------------
}