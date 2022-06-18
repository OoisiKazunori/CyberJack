#include "../Math/KazMath.h"
#include<cmath>//sqrt

XMVECTOR KazMath::LoadFloat3ToVector(XMFLOAT3 NUM_2)
{
	XMVECTOR num;
	num.m128_f32[0] = NUM_2.x;
	num.m128_f32[1] = NUM_2.y;
	num.m128_f32[2] = NUM_2.z;
	return num;
}

XMFLOAT3 KazMath::LoadVecotrToXMFLOAT3(XMVECTOR NUM_1)
{
	XMFLOAT3 num;
	num.x = NUM_1.m128_f32[0];
	num.y = NUM_1.m128_f32[1];
	num.z = NUM_1.m128_f32[2];
	return num;
}

XMVECTOR KazMath::CalculateScreenToWorld(XMVECTOR pout, XMMATRIX View, XMMATRIX Prj)
{
	//各行列の逆行列を算出
	XMMATRIX InvView, InvPrj, InvViewport;
	InvView = XMMatrixInverse(nullptr, View);
	InvPrj = XMMatrixInverse(nullptr, Prj);

	XMMATRIX VP;
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = WIN_X / 2.0f;
	VP.r[1].m128_f32[1] = -WIN_Y / 2.0f;
	VP.r[2].m128_f32[2] = WIN_X / 2.0f;
	VP.r[3].m128_f32[3] = WIN_Y / 2.0f;
	InvViewport = XMMatrixInverse(nullptr, VP);


	//逆変換
	XMMATRIX tmp = InvViewport * InvPrj * InvView;

	pout = XMVector3TransformCoord(pout, InvViewport);
	pout = XMVector3TransformCoord(pout, InvPrj);
	pout = XMVector3TransformCoord(pout, InvView);

	return pout;
}

template<typename T>
inline KazMath::Vec2<T> KazMath::CaluAngle(const KazMath::Vec2<T> &POS, float Angle, const KazMath::Vec2<T> &CPos)
{
	KazMath::Vec2<T> V, oldV;
	KazMath::Vec2<T> calu;
	double Radian;

	oldV = POS;


	Radian = Angle * PI_2F / static_cast<T>(180);
	double Cos = cos(Radian);
	double Sin = sin(Radian);

	V = POS - CPos;

	calu.x = V.x * Cos - V.y * Sin;
	calu.y = V.x * Sin + V.y * Cos;
	calu = calu + CPos;


	Vec2<float> result = oldV - calu;
	return result;
}

template<typename T>
inline KazMath::Vec3<T> CaluAngle3D(const KazMath::Vec3<T> &POS, float ANGLE, const KazMath::Vec3<T> &CENTRAL_POS)
{
	KazMath::Vec3<T> V, oldV;
	KazMath::Vec3<T> calu;
	float Radian;

	oldV = POS;
	Radian = ANGLE * PI_2F / 180.0f;

	T Cos = static_cast<T>(cos(Radian));
	T Sin = static_cast<T>(sin(Radian));

	V = POS - CENTRAL_POS;

	calu.x = V.x * Cos - V.y * Sin;
	calu.y = V.x * Sin + V.y * Cos;
	calu = calu + CENTRAL_POS;


	KazMath::Vec3<T> result = oldV - calu;
	result.z = static_cast<T>(0);

	return result;
}

void KazMath::ConvertMatrixFromFbx(XMMATRIX *DST, const FbxAMatrix &SRC)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			DST->r[i].m128_f32[j] = (float)SRC.Get(i, j);
		}
	}
}

int KazMath::ConvertSecondToFlame(int SECOND)
{
	return SECOND * 60;
}

template <typename T>
inline KazMath::Vec3<T> KazMath::CaluAPointToBPointVel(const KazMath::Vec3<T> &A_POINT, const KazMath::Vec3<T> &B_POINT, int FLAME, int MAX_FLAME)
{
	Vec3<T> distance = A_POINT - B_POINT;
	Vec3<T> mul = static_cast<float>(FLAME) / static_cast<float>(MAXFLAME);
	return distance * mul;
}

KazMath::Vec3<float> KazMath::ConvertScreenPosToWorldPos(const KazMath::Vec3<float> &SCREEN_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT)
{
	//各行列の逆行列を算出
	XMMATRIX InvView, InvPrj, InvViewport;
	InvView = XMMatrixInverse(nullptr, VIEW_MAT);
	InvPrj = XMMatrixInverse(nullptr, PROJECTION_MAT);

	XMMATRIX VP;
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] =  WIN_X / 2.0f;
	VP.r[1].m128_f32[1] = -WIN_Y / 2.0f;
	VP.r[3].m128_f32[0] =  WIN_X / 2.0f;
	VP.r[3].m128_f32[1] =  WIN_Y / 2.0f;
	InvViewport = XMMatrixInverse(nullptr, VP);

	//逆変換
	XMVECTOR result = { SCREEN_POS.x,SCREEN_POS.y,SCREEN_POS.z };
	result = XMVector3TransformCoord(result, InvViewport);
	result = XMVector3TransformCoord(result, InvPrj);
	result = XMVector3TransformCoord(result, InvView);

	return KazMath::CastXMVECTOR<float>(result);
}

XMMATRIX KazMath::CaluRotaMatrix(const Vec3<float> &ROTATION)
{
	XMMATRIX matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(ROTATION.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(ROTATION.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(ROTATION.y));
	return matRot;
}

XMMATRIX KazMath::CaluScaleMatrix(const Vec3<float> &SCALE)
{
	XMMATRIX matScale = XMMatrixIdentity();
	matScale = XMMatrixScaling(SCALE.x, SCALE.y, SCALE.z);
	return matScale;
}

XMMATRIX KazMath::CaluTransMatrix(const Vec3<float> &POS)
{
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(POS.x, POS.y, POS.z);

	return matTrans;
}

vector<float> KazMath::GetGaussianWeights(size_t count, float s)
{
	//ウェイト配列返却用
	std::vector<float> weights(count);
	float x = 0.0f;
	float total = 0.0f;

	for (auto &wgt : weights) {
		wgt = expf(-(x * x) / (2 * s * s));
		total += wgt;
		x += 1.0f;
	}

	total = total * 2.0f - 1;

	for (auto &wgt : weights) {
		wgt /= total;
	}

	return weights;
}

unsigned int KazMath::AligmentedValue(unsigned int size, unsigned int alignment)
{
	return (size + alignment - (size % alignment));
}

void KazMath::CalcWeightsTableFromGaussian(float *weightsTbl, int sizeOfWeightsTbl, float sigma)
{
	// 重みの合計を記録する変数を定義する
	float total = 0;

	// ここからガウス関数を用いて重みを計算している
	// ループ変数のxが基準テクセルからの距離
	for (int x = 0; x < sizeOfWeightsTbl; x++)
	{
		weightsTbl[x] = expf(-0.5f * (float)(x * x) / sigma);
		total += 2.0f * weightsTbl[x];
	}

	// 重みの合計で除算することで、重みの合計を1にしている
	for (int i = 0; i < sizeOfWeightsTbl; i++)
	{
		weightsTbl[i] /= total;
	}
}

KazMath::Vec3<float> KazMath::CaluEyePosForDebug(const Vec3<float> &EYE_POS, const Vec3<float> &MOVE, const Vec2<float> &ANGLE)
{
	Vec3<float> resultPos = EYE_POS;

	float rad = ANGLE.x * 3.14f / 180.0f;
	float rad_2 = ANGLE.y * 3.14f / 180.0f;


	float speed = 1;
	//左
	if (MOVE.x < 0)
	{
		resultPos.x += speed * cos(rad);
		resultPos.z -= speed * sin(rad);
	}
	//右
	if (0 < MOVE.x)
	{
		resultPos.x -= speed * cos(rad);
		resultPos.z += speed * sin(rad);
	}
	//上
	if (MOVE.y < 0)
	{
		resultPos.x -= speed * sin(rad);
		resultPos.z -= speed * cos(rad);
	}
	//下
	if (0 < MOVE.y)
	{
		resultPos.x += speed * sin(rad);
		resultPos.z += speed * cos(rad);
	}

	resultPos.y = ANGLE.y;

	return resultPos;
}

KazMath::Vec3<float> KazMath::CaluTargetPosForDebug(const Vec3<float> &EYE_POS, float ANGLE)
{
	float rad = ANGLE * 3.14f / 180.0f;

	Vec3<float> target;
	target.x = EYE_POS.x + static_cast<float>(5.0f * sin(rad));
	target.y = EYE_POS.y;
	target.z = EYE_POS.z + static_cast<float>(5.0f * cos(rad));

	return target;
}

bool KazMath::MatrixEqualOrNot(const XMMATRIX &MAT_1, const XMMATRIX &MAT_2)
{
	bool tupleDirtyFlag = false;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (MAT_1.r[y].m128_f32[x] != MAT_2.r[y].m128_f32[x])
			{
				tupleDirtyFlag = true;
			}
		}
	}

	return tupleDirtyFlag;
}

XMMATRIX KazMath::CaluSlopeMatrix(const Vec3<float> &Y, const Vec3<float> &Z)
{
	XMMATRIX matSlope;
	/*
		axisX...外積で求まる
		axisY...上ベクトル.....Distanceとかを代入するよ
		axisZ...前ベクトル
		↓
		上ベクトル基準
		axisX * axisYで外積
		↓
		matSlopeに組み込む
	*/
	XMVECTOR resultX;
	XMVECTOR resultY;
	XMVECTOR resultZ;
	XMVECTOR axisY, axisZ;

	axisZ = Z.ConvertXMVECTOR();
	axisY = Y.ConvertXMVECTOR();

	if (Y.z == 1.0f)
	{
		axisZ = { 0,-1,0 };
	}
	else if (Y.z == -1.0f)
	{
		axisZ = { 0,1,0 };
	}

	//一回目の外積.......Xを求める
	{
		XMVECTOR y, z;
		y = axisY;
		z = axisZ;
		resultX = XMVector3Cross(y, z);
	}
	resultX = XMVector3Normalize(resultX);

	//二回目の外積........Zを求める(上ベクトル基準)
	{
		XMVECTOR x, y;
		x = resultX;
		y = axisY;
		resultZ = XMVector3Cross(x, y);
	}
	resultY = axisY;

	//代入
	matSlope.r[0] = resultX;
	matSlope.r[1] = resultY;
	matSlope.r[2] = resultZ;
	matSlope.r[3] = XMVectorSet(0, 0, 0, 1);

	return matSlope;
}

XMMATRIX KazMath::CaluFrontMatrix(const Vec3<float> &Y, const Vec3<float> &Z)
{
	XMMATRIX matSlope;
	/*
		axisX...外積で求まる
		axisY...上ベクトル.....Distanceとかを代入するよ
		axisZ...前ベクトル
		↓
		上ベクトル基準
		axisX * axisYで外積
		↓
		matSlopeに組み込む
	*/
	XMVECTOR resultX;
	XMVECTOR resultY;
	XMVECTOR resultZ;
	XMVECTOR axisY, axisZ;

	axisZ = Z.ConvertXMVECTOR();
	axisY = Y.ConvertXMVECTOR();

	//普段要素
	if (Z.z == 1.0f)
	{
		axisY = { 0,1,0 };
	}
	else if (Z.z == -1.0f)
	{
		axisY = { 0,1,0 };
	}

	//一回目の外積.......Xを求める
	{
		XMVECTOR z, y;
		y = axisY;
		z = axisZ;
		resultX = XMVector3Cross(y, z);
	}
	resultX = XMVector3Normalize(resultX);

	//二回目の外積........Yを求める(上ベクトル基準)
	{
		XMVECTOR z, x;
		x = resultX;
		z = axisZ;
		resultY = XMVector3Cross(z, x);
	}
	resultZ = axisZ;

	//代入
	matSlope.r[0] = resultX;
	matSlope.r[1] = resultY;
	matSlope.r[2] = resultZ;
	matSlope.r[3] = XMVectorSet(0, 0, 0, 1);

	return matSlope;
}


int KazMath::RadianToAngle(float RADIAN)
{
	float angle = RADIAN * (180.0f / PI_2F);
	return static_cast<int>(angle);
}

float KazMath::AngleToRadian(int ANGLE)
{
	float radian = static_cast<float>(ANGLE) * (PI_2F / 180.0f);
	return radian;
}

template <typename T>
inline XMMATRIX KazMath::CaluMat(const KazMath::Transform3D &TRANSFORM, const Vec3<T> &Y_VEC, const Vec3<T> &Z_VEC, const XMMATRIX &VIEW_MAT, const XMMATRIX &PROJECT_MAT)
{
	BaseMatWorldData baseMatWorldData;
	baseMatWorldData.matWorld = XMMatrixIdentity();
	baseMatWorldData.matScale = KazMath::CaluScaleMatrix(TRANSFORM.scale);
	baseMatWorldData.matTrans = KazMath::CaluTransMatrix(TRANSFORM.pos);
	baseMatWorldData.matRota = KazMath::CaluRotaMatrix(TRANSFORM.rotation);
	//ワールド行列の計算
	baseMatWorldData.matWorld = XMMatrixIdentity();
	baseMatWorldData.matWorld *= baseMatWorldData.matScale;
	baseMatWorldData.matWorld *= baseMatWorldData.matRota;
	if (Y_VEC.x != 0.0f ||
		Y_VEC.y != 1.0f ||
		Y_VEC.z != 0.0f)
	{
		baseMatWorldData.matWorld *= KazMath::CaluSlopeMatrix(Y_VEC, { 0.0f,0.0f,1.0f });
	}
	if (Z_VEC.x != 0.0f ||
		Z_VEC.y != 0.0f ||
		Z_VEC.z != 1.0f)
	{
		baseMatWorldData.matWorld *= KazMath::CaluFrontMatrix({ 0.0f,1.0f,0.0f }, Z_VEC);
	}
	baseMatWorldData.matWorld *= baseMatWorldData.matTrans;

	return baseMatWorldData.matWorld * VIEW_MAT * PROJECT_MAT;
}

template<typename T>
inline XMMATRIX KazMath::CaluWorld(const KazMath::Transform3D &TRANSFORM, const Vec3<T> &Y_VEC, const Vec3<T> &Z_VEC)
{
	BaseMatWorldData baseMatWorldData;
	baseMatWorldData.matWorld = XMMatrixIdentity();
	baseMatWorldData.matScale = KazMath::CaluScaleMatrix(TRANSFORM.scale);
	baseMatWorldData.matTrans = KazMath::CaluTransMatrix(TRANSFORM.pos);
	baseMatWorldData.matRota = KazMath::CaluRotaMatrix(TRANSFORM.rotation);
	//ワールド行列の計算
	baseMatWorldData.matWorld = XMMatrixIdentity();
	baseMatWorldData.matWorld *= baseMatWorldData.matScale;
	baseMatWorldData.matWorld *= baseMatWorldData.matRota;
	if (Y_VEC.x != 0.0f ||
		Y_VEC.y != 1.0f ||
		Y_VEC.z != 0.0f)
	{
		baseMatWorldData.matWorld *= KazMath::CaluSlopeMatrix(Y_VEC, { 0.0f,0.0f,1.0f });
	}
	if (Z_VEC.x != 0.0f ||
		Z_VEC.y != 0.0f ||
		Z_VEC.z != 1.0f)
	{
		baseMatWorldData.matWorld *= KazMath::CaluFrontMatrix({ 0.0f,1.0f,0.0f }, Z_VEC);
	}
	baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
	return baseMatWorldData.matWorld;
}

void KazMath::Larp(float BASE_TRANSFORM, float *TRANSFORM, float MUL)
{
	float distance = BASE_TRANSFORM - *TRANSFORM;
	*TRANSFORM += distance * MUL;
}

KazMath::Vec3<float> KazMath::SplinePosition(const std::vector<Vec3<float>> &points, size_t startIndex, float t, bool Loop)
{
	if (startIndex < 1)
	{
		return points[1];
	}
	KazMath::Vec3<float> p0 = points[startIndex - 1];
	KazMath::Vec3<float> p1 = points[startIndex];
	KazMath::Vec3<float> p2;
	KazMath::Vec3<float> p3;
	if (Loop == true)
	{
		if (startIndex > points.size() - 3)
		{
			p2 = points[1];
			p3 = points[2];
		}
		else
		{
			p2 = points[startIndex + 1];
			p3 = points[startIndex + 2];
		}
	}
	else
	{
		int size = static_cast<int>(points.size());
		if (startIndex > size - 3)return points[size - 3];
		p2 = points[startIndex + 1];
		p3 = points[startIndex + 2];
	}

	KazMath::Vec3<float> a((p1 * 2.0f + (-p0 + p2) * t) * 0.5f);
	KazMath::Vec3<float> b((p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3) * ((t * t)));
	KazMath::Vec3<float> c((-p0 + p1 * 3.0f - p2 * 3.0f + p3) * (t * t * t));

	KazMath::Vec3<float> anser = a + b + c;
	return anser;
};

KazMath::Vec3<float> KazMath::ConvertWorldPosToScreenPos(const KazMath::Vec3<float> &WORLD_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT)
{
	XMMATRIX View = VIEW_MAT, Prj = PROJECTION_MAT, InvViewport, matWorld;

	XMMATRIX VP;
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = static_cast<float>(WIN_X) / 2.0f;
	VP.r[1].m128_f32[1] = -static_cast<float>(WIN_Y) / 2.0f;
	VP.r[3].m128_f32[0] = static_cast<float>(WIN_X) / 2.0f;
	VP.r[3].m128_f32[1] = static_cast<float>(WIN_Y) / 2.0f;
	InvViewport = VP;

	XMMATRIX mat;
	XMVECTOR result = { -1.0f,-1.0f,-1.0f,-1.0f };

	XMMATRIX matRot = KazMath::CaluRotaMatrix(KazMath::Vec3<float>(0.0f, 0.0f, 0.0f));
	XMMATRIX matTrans = KazMath::CaluTransMatrix(WORLD_POS);
	XMMATRIX matScale = KazMath::CaluScaleMatrix(KazMath::Vec3<float>(1.0f, 1.0f, 1.0f));

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;


	mat = matWorld * View * Prj;
	mat *= InvViewport;

	result = XMVector3TransformCoord(result, mat);
	return KazMath::CastXMVECTOR<float>(result);
};