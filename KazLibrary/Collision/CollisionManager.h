#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include"../Render/KazRender.h"

struct Triangle
{
	XMVECTOR p0;
	XMVECTOR p1;
	XMVECTOR p2;
	XMVECTOR normal;

	void ComputeNormal()
	{
		XMVECTOR p0_p1 = p1 - p0;
		XMVECTOR p0_p2 = p2 - p0;

		//外積により、2辺に垂直なベクトルを算出する
		normal = XMVector3Cross(p0_p1, p0_p2);
		normal = XMVector3Normalize(normal);
	}
};

struct Ray
{
	//始点座標
	XMVECTOR start{ 0,0,0,1 };
	//方向
	XMVECTOR dir{ 1,0,0,0 };

	void NormlizeDir()
	{
		float length = static_cast<float>(pow(
			(start.m128_f32[0] * start.m128_f32[0]) +
			(start.m128_f32[1] * start.m128_f32[1]) +
			(start.m128_f32[2] * start.m128_f32[2])
			, 0.5
		));

		XMVECTOR normal;
		normal = start / length;
		dir = XMVectorSet(normal.m128_f32[0], start.m128_f32[1], start.m128_f32[2], start.m128_f32[3]);
	}
};

struct Sphere
{
	// 中心座標
	XMVECTOR* center;
	// 半径
	float radius = 1.0f;
};

struct Plane
{
	// 法線ベクトル
	XMVECTOR normal = { 0,1,0 };
	// 原点(0,0,0)からの距離
	float distance = 0.0f;
};

struct Square
{
	XMVECTOR center;
	XMVECTOR size;
};

struct ModiRectangle
{
	/*

		データは下の絵のように左上(p0)から時計回りに入れてください。

	   p0___________p1
		|			|
		|			|
	   p3___________p2

	*/

	XMVECTOR p0;
	XMVECTOR p1;
	XMVECTOR p2;
	XMVECTOR p3;
};

class CollisionManager :public ISingleton<CollisionManager> {
public:
	CollisionManager();
	~CollisionManager();

	/// <summary>
	/// 板と球の判定を取ります
	/// </summary>
	/// <param name="SPHERE">球データ</param>
	/// <param name="PLANE">板データ</param>
	/// <param name="INTER">交点</param>
	/// <returns>true...衝突する,false...衝突していない</returns>
	bool CheckSphereAndPlane(const Sphere& SPHERE, const Plane& PLANE, XMVECTOR* INTER = nullptr);

	/// <summary>
	/// 球と三角形の判定を取ります
	/// </summary>
	/// <param name="SPHERE">球データ</param>
	/// <param name="TRIANGLE">三角形データ</param>
	/// <param name="INTER">交点</param>
	/// <returns>true...衝突する,false...衝突していない</returns>
	bool CheckSphereAndTriangle(const Sphere& SPHERE, const Triangle& TRIANGLE, XMVECTOR* INTER = nullptr);

	/// <summary>
	/// 線と板の判定を取ります
	/// </summary>
	/// <param name="RAY">線データ</param>
	/// <param name="PLANE">板データ</param>
	/// <param name="DISTANCE">距離</param>
	/// <param name="INTER">交点</param>
	/// <returns>true...衝突する,false...衝突していない</returns>
	bool CheckRayAndPlane(const Ray& RAY, const Plane& PLANE, float* DISTANCE, XMVECTOR* INTER);

	/// <summary>
	/// 線と三角形の判定を取ります
	/// </summary>
	/// <param name="RAY">線データ</param>
	/// <param name="TRIANGLE">三角形データ</param>
	/// <param name="DISTANCE">距離</param>
	/// <param name="INTER">交点</param>
	/// <returns>true...衝突する,false...衝突していない</returns>
	bool CheckRayAndTriange(const Ray& RAY, const Triangle& TRIANGLE, float* DISTANCE = nullptr, XMVECTOR* INTER = nullptr);

	/// <summary>
	/// 線と球の判定を取ります
	/// </summary>
	/// <param name="RAY">線データ</param>
	/// <param name="SPHERE">球データ</param>
	/// <param name="DISTANCE">距離</param>
	/// <param name="INTER">交点</param>
	/// <returns>true...衝突する,false...衝突していない</returns>
	bool CheckRayAndSphere(const Ray& RAY, const Sphere& SPHERE, float* DISTANCE = nullptr, XMVECTOR* INTER = nullptr);


	/// <summary>
	/// 球と球の判定を取ります
	/// </summary>
	/// <param name="SPHERE_1">球データ1</param>
	/// <param name="SPHERE_2">球データ2</param>
	/// <returns>true...衝突する,false...衝突していない</returns>
	bool CheckSphereAndSphere(const Sphere& SPHERE_1, const Sphere& SPHERE_2);

	/// <summary>
	/// 長方形と長方形の判定を取ります
	/// </summary>
	/// <param name="SQUARE_1">四角データ1</param>
	/// <param name="SQUARE_2">四角データ2</param>
	/// <returns>true...衝突する,false...衝突していない</returns>
	bool CheckSquareAndSquare(const Square& SQUARE_1, const Square& SQUARE_2);

	/// <summary>
	/// 回転した矩形と球の判定を取ります
	/// </summary>
	/// <param name="SPHERE"></param>
	/// <param name="RAY"></param>
	/// <returns></returns>
	bool CheckThicklineAndSphere(const Sphere& SPHERE, const ModiRectangle& RECTANGLE);

	friend ISingleton<CollisionManager>;
private:

	//点と三角形の最近接点
	void ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest);


	float CaluCross(XMFLOAT2 a, XMFLOAT2 b, XMFLOAT2 c)
	{
		return (b.x - a.x) * ((c.y * -1) - (a.y * -1)) - (c.x - a.x) * ((b.y * -1) - (a.y * -1));
	}
};

