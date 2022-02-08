#include"../Collision/CollisionManager.h"
#include "../Math/KazMath.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::CheckSphereAndPlane(const Sphere& SPHERE, const Plane& PLANE, XMVECTOR* INTER)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(SPHERE.center, PLANE.normal);

	//平面の原点距離を減算する事で、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - PLANE.distance;

	//距離の絶対値が半径より大きければ当たってない
	if (fabs(dist) > SPHERE.radius)
	{
		return false;
	}


	//疑似光点を計算
	if (INTER)
	{
		//平面上の疑似接点を、疑似交点とする
		*INTER = -dist * PLANE.normal + SPHERE.center;
	}
	return true;
}

bool CollisionManager::CheckSphereAndTriangle(const Sphere& SPHERE, const Triangle& TRIANGLE, XMVECTOR* INTER)
{
	XMVECTOR p;

	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(SPHERE.center, TRIANGLE, &p);

	//点pと球の中心の差分ベクトル
	XMVECTOR v = p - SPHERE.center;

	//距離の二乗を求める
	//(同じベクトル同士の内積は三平方の定理のルート内部の式と一致する)
	v = XMVector3Dot(v, v);

	//球と三角形の距離が半径以下なら当たっていない
	if (v.m128_f32[0] > SPHERE.radius * SPHERE.radius)
	{
		return false;
	}

	if (INTER)
	{
		//三角形上の最近接点
		*INTER = p;
	}

}

bool CollisionManager::CheckRayAndPlane(const Ray& RAY, const Plane& PLANE, float* DISTANCE, XMVECTOR* INTER)
{
	//イプシオン...誤差吸収用の0に近い微小な値
	const float epsion = 1.0 - 5.0f;
	//面法線とレイの方向ベクトルの内積
	float d1 = XMVector3Dot(PLANE.normal, RAY.dir).m128_f32[0];
	//裏面に当たらない
	if (d1 > -epsion)
	{
		return false;
	}

	//始点と平面の距離(平面の法線方向)
	//面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = XMVector3Dot(PLANE.normal, RAY.dir).m128_f32[0];

	//始点と平面の距離(平面の法線方向)
	float dist = d2 - PLANE.distance;

	//始点と平面の距離(レイ方向)
	float t = dist / -d1;

	//交点が始点より後ろにあるので、当たらない
	if (t < 0)
	{
		return false;
	}

	//距離を書き込む
	if (DISTANCE)
	{
		*DISTANCE = t;
	}

	//交点を計算
	if (INTER)
	{
		*INTER = RAY.start + t * RAY.dir;
	}

	return true;
}

bool CollisionManager::CheckRayAndTriange(const Ray& RAY, const Triangle& TRIANGLE, float* DISTANCE, XMVECTOR* INTER)
{
	//三角形が載っている平面を算出
	Plane plane;
	XMVECTOR interPlane;

	plane.normal = TRIANGLE.normal;
	plane.distance = XMVector3Dot(TRIANGLE.normal, TRIANGLE.p0).m128_f32[0];

	//Rayと平面が当たっていなければ、当たっていない
	if (!CheckRayAndPlane(RAY, plane, DISTANCE, &interPlane))
	{
		return false;
	}

	//Rayと平面が当たっていたので、距離と交点が書き込まれた
	//Rayと平面の好転が三角形の内側にあるか判定
	const float epsilon = 1.0 - 5.0f;
	XMVECTOR m;

	//辺p0_p1について
	XMVECTOR pt_p0 = TRIANGLE.p0 - interPlane;
	XMVECTOR p0_p1 = TRIANGLE.p1 - TRIANGLE.p0;
	m = XMVector3Cross(pt_p0, p0_p1);

	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, TRIANGLE.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}


	//辺p1_p2について
	XMVECTOR pt_p1 = TRIANGLE.p1 - interPlane;
	XMVECTOR p1_p2 = TRIANGLE.p1 - TRIANGLE.p2;
	m = XMVector3Cross(pt_p1, p1_p2);

	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, TRIANGLE.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}


	//辺p2_p0について
	XMVECTOR pt_p2 = TRIANGLE.p2 - interPlane;
	XMVECTOR p2_p0 = TRIANGLE.p2 - TRIANGLE.p0;
	m = XMVector3Cross(pt_p2, p2_p0);

	//辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, TRIANGLE.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}


	if (INTER)
	{
		*INTER = interPlane;
	}

	return true;
}

bool CollisionManager::CheckRayAndSphere(const Ray& RAY, const Sphere& SPHERE, float* DISTANCE, XMVECTOR* INTER)
{
	XMVECTOR m = RAY.start - SPHERE.center;
	float b = XMVector3Dot(m, RAY.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - SPHERE.radius * SPHERE.radius;

	//Rayの始点がSphereの外側にあり(c>0),RayがSphereから離れていく方向を指している場合(b>0),当たらない
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	//負の判別式はRayが球を外れていることに一致
	if (discr < 0.0f)
	{
		return false;
	}


	//Rayは球と交差している
	//交差する最小の値Tを計算
	float t = -b - sqrtf(discr);

	//tが負である場合、Rayは球の内側から開始しているのでtをゼロにクランプ
	//クランプ(値を範囲内に入れる)
	if (t < 0)
	{
		t = 0.0f;
	}

	if (DISTANCE)
	{
		*DISTANCE = t;
	}

	if (INTER)
	{
		*INTER = RAY.start + t * RAY.dir;
	}
	return true;
}

bool CollisionManager::CheckSphereAndSphere(const Sphere& SPHERE_1, const Sphere& SPHERE_2)
{
	XMVECTOR tmp;
	tmp.m128_f32[0] = SPHERE_1.center.m128_f32[0] - SPHERE_2.center.m128_f32[0];
	tmp.m128_f32[1] = SPHERE_1.center.m128_f32[1] - SPHERE_2.center.m128_f32[1];
	tmp.m128_f32[2] = SPHERE_1.center.m128_f32[2] - SPHERE_2.center.m128_f32[2];


	float distance = sqrtf(pow(tmp.m128_f32[0], 2) + pow(tmp.m128_f32[1], 2) + pow(tmp.m128_f32[2], 2));
	float sumRadist = SPHERE_1.radius + SPHERE_2.radius;

	return (distance <= sumRadist);
}

bool CollisionManager::CheckSquareAndSquare(const Square& SQUARE_1, const Square& SQUARE_2)
{
	XMVECTOR adjPos[2] = { SQUARE_1.size / 2.0f,SQUARE_2.size / 2.0f };
	int x = 0, y = 2;
	bool isHit = fabs(SQUARE_1.center.m128_f32[x] - SQUARE_2.center.m128_f32[x]) <= adjPos[0].m128_f32[x] + adjPos[1].m128_f32[x]
		&& fabs(SQUARE_1.center.m128_f32[y] - SQUARE_2.center.m128_f32[y]) <= adjPos[0].m128_f32[y] + adjPos[1].m128_f32[y];
	return isHit;
}

bool CollisionManager::CheckThicklineAndSphere(const Sphere &SPHERE, const ModiRectangle &RECTANGLE)
{
	/*

	〇 判定の流れ

	・ 矩形を円の半径分、隣接する頂点から延びる方向に拡張する。

		 ↑			 ↑
	  ← p0___________p1 →
		 |			 |
		 |			 |
	  ← p3___________p2 →
		 ↓			 ↓

	・ 円の中心点と矩形とで外積を使った左右判定を使用して、当たり判定を行う。


	*注意 …　矩形は左上をp0にして、時計回りの順番にデータを入れてください。

	*/



	// Z軸の情報をY軸に入れる。

	Sphere sphereBuff = SPHERE;
	ModiRectangle rectangleBuff = RECTANGLE;

	sphereBuff.center.m128_f32[1] = SPHERE.center.m128_f32[2];
	sphereBuff.center.m128_f32[2] = SPHERE.center.m128_f32[1];

	rectangleBuff.p0.m128_f32[1] = RECTANGLE.p0.m128_f32[2];
	rectangleBuff.p0.m128_f32[2] = RECTANGLE.p0.m128_f32[1];

	rectangleBuff.p1.m128_f32[1] = RECTANGLE.p1.m128_f32[2];
	rectangleBuff.p1.m128_f32[2] = RECTANGLE.p1.m128_f32[1];

	rectangleBuff.p2.m128_f32[1] = RECTANGLE.p2.m128_f32[2];
	rectangleBuff.p2.m128_f32[2] = RECTANGLE.p2.m128_f32[1];

	rectangleBuff.p3.m128_f32[1] = RECTANGLE.p3.m128_f32[2];
	rectangleBuff.p3.m128_f32[2] = RECTANGLE.p3.m128_f32[1];


	// 矩形データを保存する。
	ModiRectangle rectangleBuffBuff = rectangleBuff;

	// 円の半径を保存する変数
	XMVECTOR sphereRadius = { sphereBuff.radius, sphereBuff.radius, sphereBuff.radius };

	// 頂点を計算する際にデータを一時保存する変数
	XMVECTOR dataBuff = {};
	XMVECTOR dataBuff2 = {};


	/*-- 矩形を拡張する。 --*/

	// p0に関して拡張処理を行う。
	{

		// p1からp0までの方向ベクトルを求め、正規化させる。
		dataBuff = KazMath::SubXMVECTOR(rectangleBuffBuff.p0, rectangleBuffBuff.p1);
		dataBuff = XMVector2Normalize(dataBuff);
		// p3からp0までの方向ベクトルを求め、正規化させる。
		dataBuff2 = KazMath::SubXMVECTOR(rectangleBuffBuff.p0, rectangleBuffBuff.p3);
		dataBuff2 = XMVector2Normalize(dataBuff2);

		//// 足して正規化する
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p0 += KazMath::MulXMVECTOR(dataBuff, sphereRadius);
		rectangleBuffBuff.p0 += KazMath::MulXMVECTOR(dataBuff2, sphereRadius);

	}

	// p1に関して拡張処理を行う。
	{

		// p2からp1までの方向ベクトルを求め、正規化させる。
		dataBuff = KazMath::SubXMVECTOR(rectangleBuffBuff.p1, rectangleBuffBuff.p2);
		dataBuff = XMVector2Normalize(dataBuff);
		// p0からp1までの方向ベクトルを求め、正規化させる。
		dataBuff2 = KazMath::SubXMVECTOR(rectangleBuffBuff.p1, rectangleBuffBuff.p0);
		dataBuff2 = XMVector2Normalize(dataBuff2);

		//// 足して正規化する
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p1 += KazMath::MulXMVECTOR(dataBuff, sphereRadius);
		rectangleBuffBuff.p1 += KazMath::MulXMVECTOR(dataBuff2, sphereRadius);

	}

	// p2に関して拡張処理を行う。
	{

		// p3からp2までの方向ベクトルを求め、正規化させる。
		dataBuff = KazMath::SubXMVECTOR(rectangleBuffBuff.p2, rectangleBuffBuff.p3);
		dataBuff = XMVector2Normalize(dataBuff);
		// p1からp2までの方向ベクトルを求め、正規化させる。
		dataBuff2 = KazMath::SubXMVECTOR(rectangleBuffBuff.p2, rectangleBuffBuff.p1);
		dataBuff2 = XMVector2Normalize(dataBuff2);

		//// 足して正規化する
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p2 += KazMath::MulXMVECTOR(dataBuff, sphereRadius);
		rectangleBuffBuff.p2 += KazMath::MulXMVECTOR(dataBuff2, sphereRadius);

	}

	// p3に関して拡張処理を行う。
	{

		// p0からp1までの方向ベクトルを求め、正規化させる。
		dataBuff = KazMath::SubXMVECTOR(rectangleBuffBuff.p3, rectangleBuffBuff.p0);
		dataBuff = XMVector2Normalize(dataBuff);
		// p2からp3までの方向ベクトルを求め、正規化させる。
		dataBuff2 = KazMath::SubXMVECTOR(rectangleBuffBuff.p3, rectangleBuffBuff.p2);
		dataBuff2 = XMVector2Normalize(dataBuff2);

		//// 足して正規化する
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p3 += KazMath::MulXMVECTOR(dataBuff, sphereRadius);
		rectangleBuffBuff.p3 += KazMath::MulXMVECTOR(dataBuff2, sphereRadius);

	}

	/*-- 拡張した矩形と円の中心点を外積の左右判定を使って当たり判定を行う。 --*/

	// 判定を保存するようの変数
	vector<bool> isHit;

	// 外積判定をする際にベクトルを保存するための変数
	XMVECTOR vTvVec = {};	// 頂点と頂点のベクトル
	XMVECTOR vTPVec = {};	// 頂点と中心点のベクトル

	// 結果を保存する変数
	float crossResultBuff = 0;

	// p0->p1での左右判定
	{

		// 頂点と頂点ベクトルを求めて正規化
		vTvVec = KazMath::SubXMVECTOR(rectangleBuffBuff.p1, rectangleBuffBuff.p0);
		vTvVec = XMVector2Normalize(vTvVec);
		// 頂点と中心点のベクトルを求めて正規化
		vTPVec = KazMath::SubXMVECTOR(sphereBuff.center, rectangleBuffBuff.p0);
		vTPVec = XMVector2Normalize(vTPVec);

		// 外積を計算 +の場合右判定
		float a = XMVector2Cross(vTvVec, vTPVec).m128_f32[0];
		isHit.push_back(XMVector2Cross(vTvVec, vTPVec).m128_f32[0] < 0);

	}

	// p1->p2での左右判定
	{

		// 頂点と頂点ベクトルを求めて正規化
		vTvVec = KazMath::SubXMVECTOR(rectangleBuffBuff.p2, rectangleBuffBuff.p1);
		vTvVec = XMVector2Normalize(vTvVec);
		// 頂点と中心点のベクトルを求めて正規化
		vTPVec = KazMath::SubXMVECTOR(sphereBuff.center, rectangleBuffBuff.p1);
		vTPVec = XMVector2Normalize(vTPVec);

		// 外積を計算 +の場合右判定
		isHit.push_back(XMVector2Cross(vTvVec, vTPVec).m128_f32[0] < 0);

	}

	// p2->p3での左右判定
	{

		// 頂点と頂点ベクトルを求めて正規化
		vTvVec = KazMath::SubXMVECTOR(rectangleBuffBuff.p3, rectangleBuffBuff.p2);
		vTvVec = XMVector2Normalize(vTvVec);
		// 頂点と中心点のベクトルを求めて正規化
		vTPVec = KazMath::SubXMVECTOR(sphereBuff.center, rectangleBuffBuff.p2);
		vTPVec = XMVector2Normalize(vTPVec);

		// 外積を計算 +の場合右判定
		isHit.push_back(XMVector2Cross(vTvVec, vTPVec).m128_f32[0] < 0);

	}

	// p3->p0での左右判定
	{

		// 頂点と頂点ベクトルを求めて正規化
		vTvVec = KazMath::SubXMVECTOR(rectangleBuffBuff.p0, rectangleBuffBuff.p3);
		vTvVec = XMVector2Normalize(vTvVec);
		// 頂点と中心点のベクトルを求めて正規化
		vTPVec = KazMath::SubXMVECTOR(sphereBuff.center, rectangleBuffBuff.p3);
		vTPVec = XMVector2Normalize(vTPVec);

		// 外積を計算 +の場合右判定
		isHit.push_back(XMVector2Cross(vTvVec, vTPVec).m128_f32[0] < 0);

	}

	// 結果を計算
	bool isHitResult = true;
	for (int i = 0; i < 4; ++i) {

		isHitResult = isHit[i] && isHitResult;

	}


	return isHitResult;
}
void CollisionManager::ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}
