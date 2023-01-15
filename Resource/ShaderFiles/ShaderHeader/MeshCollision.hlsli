
//BBを形づくる為のデータ
struct BoundingBoxData
{
    float3 minPos;
    float3 maxPos;
};

//パーティクルの当たり判定情報
struct ParticleHitBoxData
{
    float3 pos;
    float4 color;
    uint meshID;
    uint3 id;
};

struct MeshHitBox
{
    float3 pos;
    uint meshID;
    uint3 id;
};

struct HitIDData
{
    uint meshID;
    uint3 id;
};