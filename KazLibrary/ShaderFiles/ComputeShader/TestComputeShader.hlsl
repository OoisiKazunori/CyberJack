#define INSTANCE_FIRST_LEVEL 6
#define INSTANCE_SECOND_LEVEL 1

struct InputElement
{
    float3 pos[INSTANCE_SECOND_LEVEL];
};

struct OutPutElement
{
    matrix mat[INSTANCE_SECOND_LEVEL];
};

struct InputData
{
    InputElement data[INSTANCE_FIRST_LEVEL];
};

struct OutPutData
{
    OutPutElement test[INSTANCE_FIRST_LEVEL];
};

// ���̓f�[�^�ɃA�N�Z�X���邽�߂̕ϐ�
StructuredBuffer<InputData> inputData : register(t0);
// �o�͐�ɃA�N�Z�X���邽�߂̕ϐ�
RWStructuredBuffer<OutPutData> outputData : register(u1);

cbuffer commonBuffer : register(b2)
{
    matrix cameraMat;
    matrix projectionMat;
};

[numthreads(1, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID)
{
    //float tmpInputData = inputData[0].data + 5.0f;
    //outputData[0].data2 = tmpInputData;
    uint firstLevel = DTid.r;
    uint secondLevel = DTid.g;
    
    
    matrix da = cameraMat;
    inputData[0].data[firstLevel].pos[secondLevel];
    outputData[0].test[firstLevel].mat[secondLevel] = da;
}