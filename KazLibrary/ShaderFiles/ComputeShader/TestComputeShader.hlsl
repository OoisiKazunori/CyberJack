struct InputData
{
    float3 pos;
};

struct OutPutData
{
    matrix mat;
};
// 入力データにアクセスするための変数
StructuredBuffer<InputData> inputData : register(t0);
// 出力先にアクセスするための変数
RWStructuredBuffer<OutPutData> outputData : register(u1);

cbuffer commonBuffer : register(b0)
{
    matrix cameraMat;
    matrix projectionMat;
};

[numthreads(1, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID)
{
    //float tmpInputData = inputData[0].data + 5.0f;
    //outputData[0].data2 = tmpInputData;
}