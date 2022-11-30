#pragma once
#include"../Scene/SceneBase.h"
#include"../Render/BackGroundForDebug.h"
#include"../Render/KazRender.h"
#include"../Game/Helper/CameraWork.h"
#include"../Game/GameRenderTarget.h"
#include"../Game/Effect/MeshParticleEmitter.h"
#include"../Game/Effect/DeadParticle.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../Game/Effect/TextureParticle.h"
#include"../Game/Effect/SplineParticle.h"

#include"../Game/Tutorial/DirectX12MoviePlayer.h"

class DebugMeshParticleScene :public SceneBase
{
public:
	DebugMeshParticleScene();
	~DebugMeshParticleScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec2<float> angle;

	CameraWork cameraWork;

	static const int MESH_MAX_NUM = 6;
	std::array<std::unique_ptr<MeshParticleEmitter>, MESH_MAX_NUM> meshEmitter;
	std::array<std::string, MESH_MAX_NUM>filePass;

	int meshIndex,prevMeshIndex;
	bool cpuCheckParticleFlag;
	bool gpuCheckParticleFlag;
	bool textureParticleFlag;
	bool splineParticleFlag;
	bool perlinNoizeFlag;

	bool deadParticleFlag, prevDeadParticleFlag;

	
	std::unique_ptr<DeadParticle> deadParticle;

	//デバック用--------------------------------------------
	//三角形描画
	std::array<KazMath::Vec3<float>, 3>triangelPosArray;
	std::array<LineRender, 3>triangelLine;
	std::array<LineRender, 3>clossTriangelLine;

	//パーティクルの数
	static const int PARTICLE_MAX_NUM = 1000;
	std::array<BoxPolygonRender, PARTICLE_MAX_NUM>particle;
	int drawParticleNum;
	int particleBias;//パーティクルをエッジ周辺に偏よらせる値
	static const int PARTICLE_MAX_BIAS = 100;//パーティクルをエッジ周辺に偏よらせる値

	//計算
	bool initFlag;
	BackGroundForDebug debugDraw;

	float CalucurateTriangleArea(KazMath::Vec3<float> P0, KazMath::Vec3<float> P1, KazMath::Vec3<float> P2)
	{
		KazMath::Vec3<float> p0p1Vec = P1 - P0;
		KazMath::Vec3<float> p1p2Vec = P2 - P1;
		KazMath::Vec3<float> r = p0p1Vec.Cross(p1p2Vec);
		return r.z / 2.0f;
	}

	float CalucurateUVW(KazMath::Vec3<float> P0, KazMath::Vec3<float> P1, KazMath::Vec3<float> ATTACK_POINT, float TRIANGLE_AREA)
	{
		KazMath::Vec3<float> p0p1Vec = ATTACK_POINT - P0;
		KazMath::Vec3<float> p1p2Vec = ATTACK_POINT - P1;
		KazMath::Vec3<float> r = p0p1Vec.Cross(p1p2Vec);
		float area = r.z / 2.0f;
		float rate = area / TRIANGLE_AREA;
		return rate;
	}
	//デバック用--------------------------------------------

	//GPUでのパーティクル--------------------------------------------
	DirectX::XMMATRIX motherMat;
	FbxModelRender model;
	//GPUでのパーティクル--------------------------------------------



	std::unique_ptr<GameRenderTarget>rendertarget;
	Sprite2DRender mainRenderTarget;


	//テクスチャをコンピュートシェーダーで読み込む--------------------------------------------
	std::unique_ptr<TextureParticle> texParticle;
	RESOURCE_HANDLE handle;
	KazMath::Transform3D texTransform;
	DirectX::XMMATRIX texMotherMat;
	//テクスチャをコンピュートシェーダーで読み込む--------------------------------------------

	//スプラインパーティクル--------------------------------------------
	std::unique_ptr<SplineParticle> splineParticle;
	std::vector<KazMath::Vec3<float>>limitPos;
	std::array<BoxPolygonRender, 6> controlPointR;
	bool initSplineFlag;
	//スプラインパーティクル--------------------------------------------



	//パーリンノイズの確認--------------------------------------------
	std::array<std::array<BoxPolygonRender, 10>, 10>perlinDebugBox;
	std::array<std::array<KazMath::Vec2<float>, 10>, 10>noiseData;
	BoxPolygonRender moveNoiseBlock;
	KazMath::Vec2<float>uv;
	KazMath::Vec2<float>prevUv;
	bool initNoizeFlag;

	float PerlinNoize(const KazMath::Vec2<float> UV, int SEED);

	KazMath::Vec3<float>PNoise(const KazMath::Vec3<float> &POS)
	{
		KazMath::Vec3<float>vec =
		{
			PerlinNoize({POS.x,POS.y,},1),
			PerlinNoize({POS.x,POS.y,},1),
			PerlinNoize({POS.x,POS.y,},1)
		};
		return vec;
	}

	KazMath::Vec3<float>SamplePotential(const KazMath::Vec3<float> &POS)
	{
		return PNoise(POS / 10.0f);
	}

	KazMath::Vec3<float> CurlNoise(const KazMath::Vec3<float> &POS, int SEED)
	{
		const float e = 0.0009765625;
		const float e2 = 2.0 * e;
		const float invE2 = 1.0 / e2;
		int s = SEED;
		s = 0;

		const KazMath::Vec3<float> dx = KazMath::Vec3<float>(e, 0.0, 0.0);
		const KazMath::Vec3<float> dy = KazMath::Vec3<float>(0.0, e, 0.0);
		const KazMath::Vec3<float> dz = KazMath::Vec3<float>(0.0, 0.0, e);

		KazMath::Vec3<float> pos = POS;

		KazMath::Vec3<float> p_x0 = SamplePotential(pos - dx);
		KazMath::Vec3<float> p_x1 = SamplePotential(pos + dx);
		KazMath::Vec3<float> p_y0 = SamplePotential(pos - dy);
		KazMath::Vec3<float> p_y1 = SamplePotential(pos + dy);
		KazMath::Vec3<float> p_z0 = SamplePotential(pos - dz);
		KazMath::Vec3<float> p_z1 = SamplePotential(pos + dz);

		float x = (p_y1.z - p_y0.z) - (p_z1.y - p_z0.y);
		float y = (p_z1.x - p_z0.x) - (p_x1.z - p_x0.z);
		float z = (p_x1.y - p_x0.y) - (p_y1.x - p_y0.x);

		KazMath::Vec3<float>result(x, y, z);
		result *= invE2;
		return result;
	}


	float Frac(float LEN)
	{
		float lM;
		if (LEN < 0.1f)
		{
			return LEN;
		}
		else
		{
			lM = Frac(LEN - 1.0f);
			return LEN * lM;
		}
	}

	float Larp(float a, float b, float t)
	{
		return a + t * (b - a);
	}
	//パーリンノイズの確認--------------------------------------------

	RESOURCE_HANDLE modelHandle;
	BackGroundForDebug debug;
};

