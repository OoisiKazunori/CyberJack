#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct OutputData
{
    matrix mat;
    float4 color;
};

struct UpdateData
{
    float4 pos;
    float4 vel;
    float4 color;
    uint timer;
};

cbuffer RootConstants : register(b0)
{
    matrix view;        
    matrix projection;
    float4 emmittPos;
    uint increSize;     //�s��ƐF���o�͂���\���̂̃T�C�Y
    uint64_t gpuAddress; //�s��ƐF���o�͂���o�b�t�@��GPU�A�h���X
    int seed;
};

//�o��
AppendStructuredBuffer<OutputData> matrixData : register(u0);
//�X�V
RWStructuredBuffer<UpdateData> updateData : register(u1);

static const int NUM = 1;

[numthreads(NUM, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * NUM) + groupIndex;
    //uint index = 9000;

    float4 initPos = emmittPos;

    //��������p�[�e�B�N���̔��f-------------------------
    //�ړ��ʂƐ������Ԃ̏�����
    if(updateData[index].timer <= 0)
    {
        updateData[index].timer = 1;

        float4 generatePos = initPos + 
        float4
        (
            Rand(seed / 2,index * 2,400,-400),
            Rand(seed / 3,index * 5,0, -50),
            Rand(seed / 2,index * 6,500,-10),
            0.0f
        );
        updateData[index].pos = generatePos;
        updateData[index].color = float4(Rand(seed,index,1,0),Rand(seed/2,index,1,0),Rand(seed*2,index,1,0),1.0f);
        updateData[index].vel = float4(0.0f,0.0f,0.0f,0.0f);
    }    
    //��������p�[�e�B�N���̔��f-------------------------




    //�s��v�Z-------------------------
    matrix pMatTrans = Translate(updateData[index].pos.xyz);
    matrix pMatRot = Rotate(float3(0.0f,0.0f,0.0f));
    float scale=1.5f;
    matrix pMatScale = Scale(float3(scale, scale, scale));
    
    matrix pMatWorld = MatrixIdentity();
    pMatWorld = mul(pMatScale, pMatWorld);
    pMatWorld = mul(pMatRot, pMatWorld);
    pMatWorld = mul(pMatTrans, pMatWorld);
    //�s��v�Z-------------------------
    
    
    //�o�͗p-------------------------
    OutputData outputMat;
    matrix lView = view;
    matrix lproj = projection;

    outputMat.mat = MatrixIdentity();
    outputMat.mat = mul(pMatWorld,outputMat.mat);
    outputMat.mat = mul(lView,    outputMat.mat);
    outputMat.mat = mul(lproj,    outputMat.mat);
    outputMat.color = updateData[index].color;
    matrixData.Append(outputMat);    
    //�o�͗p-------------------------

}