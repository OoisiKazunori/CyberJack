
struct ParticleData
{
    float3 pos;
    float4 color;
    uint id;
};

struct GPUParticleInput
{
    matrix worldMat;
    float4 color;
};
