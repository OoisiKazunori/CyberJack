#define INSTANCE_DATA_MAX 800

struct InstanceOutPut
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
    uint id : SV_InstanceID;
};

struct InstanceMat
{
    matrix matData;
};

struct InstanceConstBufferData
{
    float4 color; //�F
    matrix mat; //3D�ϊ��s��
};

struct MaterialData
{
    float3 m_ambient;
    float3 m_diffuse;
    float3 m_specular;
    float m_alpha;
};


cbuffer instanceBuff : register(b1)
{
    InstanceMat matrixData[INSTANCE_DATA_MAX];
};

//�s����
cbuffer instanceConstBuff : register(b0)
{
    InstanceConstBufferData constData[INSTANCE_DATA_MAX];
};

//�}�e���A�����
cbuffer instanceConstBuffMaterial : register(b0)
{
    MaterialData materialData;
};

struct InstanceIn
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@���x�N�g��
    float2 uv : TEXCOORD; //uv�l
    float id : SV_InstanceID;
};

struct InstancePosOut
{
    float4 svpos : SV_POSITION;
    uint id : SV_InstanceID;
};



struct lineData
{
    float4 pos[2];
};

cbuffer lineVertexData : register(b1)
{
    lineData VertexLine[INSTANCE_DATA_MAX];
};