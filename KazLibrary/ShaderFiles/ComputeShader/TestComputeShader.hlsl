struct SceneConstantBuffer
{
    float4 velocity;
    float4 offset;
    float4 color;
    float4x4 projection;
    float4 padding[9];
};

struct IndirectCommand
{
    uint2 cbvAddress;
    uint4 drawArguments;
};

cbuffer RootConstants : register(b0)
{
    float xOffset;        // Half the width of the triangles.
    float zOffset;        // The z offset for the triangle vertices.
    float cullOffset;    // The culling plane offset in homogenous space.
    float commandCount;    // The number of commands to be processed.
};

//�萔�o�b�t�@
StructuredBuffer<SceneConstantBuffer> inputData : register(t0);
//�C���_�C���N�g�R�}���h
StructuredBuffer<IndirectCommand> indirectData : register(t1);
//���ʕۑ�
AppendStructuredBuffer<IndirectCommand> outputCommands : register(u0);

[numthreads(128, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = (groupId.x * 128) + groupIndex;
    outputCommands.Append(indirectData[index]);
}