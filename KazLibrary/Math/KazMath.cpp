#include "../Math/KazMath.h"

XMFLOAT3 KazMath::AddXMFLOAT3(XMFLOAT3 NUM_1, XMFLOAT3 NUM_2)
{
	XMFLOAT3 num;
	num.x = NUM_1.x + NUM_2.x;
	num.y = NUM_1.y + NUM_2.y;
	num.z = NUM_1.z + NUM_2.z;
	return num;
}

XMFLOAT2 KazMath::AddXMFLOAT2(XMFLOAT2 NUM_1, XMFLOAT2 NUM_2)
{
	XMFLOAT2 num;
	num.x = NUM_1.x + NUM_2.x;
	num.y = NUM_1.y + NUM_2.y;
	return num;
}

XMFLOAT3 KazMath::SubXMFLOAT3(XMFLOAT3 NUM_1, XMFLOAT3 NUM_2)
{
	XMFLOAT3 num;
	num.x = NUM_1.x - NUM_2.x;
	num.y = NUM_1.y - NUM_2.y;
	num.z = NUM_1.z - NUM_2.z;
	return num;
}

XMFLOAT2 KazMath::SubXMFLOAT2(XMFLOAT2 NUM_1, XMFLOAT2 NUM_2)
{
	XMFLOAT2 num;
	num.x = NUM_1.x - NUM_2.x;
	num.y = NUM_1.y - NUM_2.y;
	return num;
}

XMFLOAT3 KazMath::DivXMFLOAT3(XMFLOAT3 NUM_1, XMFLOAT3 NUM_2)
{
	XMFLOAT3 num;
	num.x = NUM_1.x / NUM_2.x;
	num.y = NUM_1.y / NUM_2.y;
	num.z = NUM_1.z / NUM_2.z;
	return num;
}

XMFLOAT3 KazMath::MulXMFLOAT3(XMFLOAT3 NUM_1, XMFLOAT3 NUM_2)
{
	XMFLOAT3 num;
	num.x = NUM_1.x * NUM_2.x;
	num.y = NUM_1.y * NUM_2.y;
	num.z = NUM_1.z * NUM_2.z;
	return num;
}

XMVECTOR KazMath::AddXMVECTOR(XMVECTOR NUM_1, XMVECTOR NUM_2)
{
	XMVECTOR num;
	num.m128_f32[0] = NUM_1.m128_f32[0] + NUM_2.m128_f32[0];
	num.m128_f32[1] = NUM_1.m128_f32[1] + NUM_2.m128_f32[1];
	num.m128_f32[2] = NUM_1.m128_f32[2] + NUM_2.m128_f32[2];
	return num;
}

XMVECTOR KazMath::SubXMVECTOR(XMVECTOR NUM_1, XMVECTOR NUM_2)
{
	XMVECTOR num;
	num.m128_f32[0] = NUM_1.m128_f32[0] - NUM_2.m128_f32[0];
	num.m128_f32[1] = NUM_1.m128_f32[1] - NUM_2.m128_f32[1];
	num.m128_f32[2] = NUM_1.m128_f32[2] - NUM_2.m128_f32[2];
	return num;
}

XMVECTOR KazMath::DivXMVECTOR(XMVECTOR NUM_1, XMVECTOR NUM_2)
{
	XMVECTOR num;
	num.m128_f32[0] = NUM_1.m128_f32[0] / NUM_2.m128_f32[0];
	num.m128_f32[1] = NUM_1.m128_f32[1] / NUM_2.m128_f32[1];
	num.m128_f32[2] = NUM_1.m128_f32[2] / NUM_2.m128_f32[2];
	return num;
}

XMVECTOR KazMath::MulXMVECTOR(XMVECTOR NUM_1, XMVECTOR NUM_2)
{
	XMVECTOR num;
	num.m128_f32[0] = NUM_1.m128_f32[0] * NUM_2.m128_f32[0];
	num.m128_f32[1] = NUM_1.m128_f32[1] * NUM_2.m128_f32[1];
	num.m128_f32[2] = NUM_1.m128_f32[2] * NUM_2.m128_f32[2];
	return num;
}

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

XMFLOAT3 KazMath::XMFLOAT3Normalize(XMFLOAT3 NUM_1)
{
	XMVECTOR a = KazMath::LoadFloat3ToVector(NUM_1);
	a = XMVector3Normalize(a);
	return KazMath::LoadVecotrToXMFLOAT3(a);
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
	VP.r[3].m128_f32[0] = WIN_X / 2.0f;
	VP.r[3].m128_f32[1] = WIN_Y / 2.0f;
	InvViewport = XMMatrixInverse(nullptr, VP);


	//逆変換
	XMMATRIX tmp = InvViewport * InvPrj * InvView;

	pout = XMVector3TransformCoord(pout, InvViewport);
	pout = XMVector3TransformCoord(pout, InvPrj);
	pout = XMVector3TransformCoord(pout, InvView);

	return pout;
}

XMFLOAT2 KazMath::CaluAngle(XMFLOAT2 POS, float Angle, XMFLOAT2 CPos)
{
	XMFLOAT2 V, oldV;
	XMFLOAT2 calu;
	double Radian;

	oldV = POS;


	Radian = Angle * PI_2F / 180;
	double Cos = cos(Radian);
	double Sin = sin(Radian);

	V = SubXMFLOAT2(POS, CPos);

	calu.x = V.x * Cos - V.y * Sin;
	calu.y = V.x * Sin + V.y * Cos;
	calu = AddXMFLOAT2(calu, CPos);


	XMFLOAT2 result = SubXMFLOAT2(oldV, calu);
	return result;
}

XMFLOAT3 KazMath::CaluAngle3D(XMFLOAT3 POS, float ANGLE, XMFLOAT3 CENTRAL_POS)
{
	XMFLOAT3 V, oldV;
	XMFLOAT3 calu;
	float Radian;

	oldV = POS;

	Radian = ANGLE * PI_2F / 180;
	float Cos = cos(Radian);
	float Sin = sin(Radian);

	V = SubXMFLOAT3(POS, CENTRAL_POS);

	calu.x = V.x * Cos - V.y * Sin;
	calu.y = V.x * Sin + V.y * Cos;
	calu = AddXMFLOAT3(calu, CENTRAL_POS);


	XMFLOAT3 result = SubXMFLOAT3(oldV, calu);

	result.z = 0;

	return result;
}

XMFLOAT2 KazMath::NewRota(float ANGLE, XMFLOAT2 POS)
{
	float radian = ANGLE * PI_2F / 180;
	float nSin = sin(radian);
	float nCos = cos(radian);


	XMFLOAT2 result;
	result.x = nCos * POS.x - nSin * POS.y;
	result.y = nSin * POS.x + nCos * POS.y;

	return result;
}

int KazMath::IntRand(int MAX_NUM, int MIN_NUM)
{
	float randNum = rand() % MAX_NUM;
	randNum += MIN_NUM;

	return randNum;
}

float KazMath::FloatRand(float MAX_NUM, float MIN_NUM)
{
	float result = (MAX_NUM - MIN_NUM) * rand() / RAND_MAX + MIN_NUM;
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

float KazMath::ConvertSecondToFlame(float SECOND)
{
	float result = SECOND * 60;
	return result;
}

XMFLOAT3 KazMath::CaluAPointToBPointVel(XMFLOAT3 A_POINT, XMFLOAT3 B_POINT, float FLAME, float MAX_FLAME)
{
	XMFLOAT3 distance = SubXMFLOAT3(A_POINT, B_POINT);
	XMFLOAT3 result;

	XMFLOAT3 mul = {
		FLAME / MAX_FLAME,
		FLAME / MAX_FLAME,
		FLAME / MAX_FLAME
	};
	result = MulXMFLOAT3(distance, mul);
	return result;
}

XMVECTOR KazMath::ConvertScreenPosToWorldPos(XMVECTOR SCREEN_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT)
{
	//各行列の逆行列を算出
	XMMATRIX InvView, InvPrj, InvViewport;
	InvView = XMMatrixInverse(nullptr, VIEW_MAT);
	InvPrj = XMMatrixInverse(nullptr, PROJECTION_MAT);

	XMMATRIX VP;
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = WIN_X / 2.0f;
	VP.r[1].m128_f32[1] = -WIN_Y / 2.0f;
	VP.r[3].m128_f32[0] = WIN_X / 2.0f;
	VP.r[3].m128_f32[1] = WIN_Y / 2.0f;
	InvViewport = XMMatrixInverse(nullptr, VP);

	//逆変換
	XMVECTOR result = SCREEN_POS;
	result = XMVector3TransformCoord(result, InvViewport);
	result = XMVector3TransformCoord(result, InvPrj);
	result = XMVector3TransformCoord(result, InvView);

	return result;
}

XMVECTOR KazMath::ConvertWorldPosToScreenPos(XMVECTOR WORLD_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT)
{
	XMMATRIX View = VIEW_MAT, Prj = PROJECTION_MAT, InvViewport, matWorld;

	XMMATRIX VP;
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] = WIN_X / 2.0f;
	VP.r[1].m128_f32[1] = -WIN_Y / 2.0f;
	VP.r[3].m128_f32[0] = WIN_X / 2.0f;
	VP.r[3].m128_f32[1] = WIN_Y / 2.0f;
	InvViewport = VP;

	XMMATRIX mat;
	XMVECTOR result = { -1,-1,-1 };


	XMMATRIX matRot = CaluRotaMatrix(XMFLOAT3(0.0f, 0.0f, 0.0f));
	XMMATRIX matTrans = CaluTransMatrix(WORLD_POS);
	XMMATRIX matScale = CaluScaleMatrix(XMFLOAT3(1.0f, 1.0f, 1.0f));

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;


	mat = matWorld * View * Prj;
	mat *= InvViewport;

	result = XMVector3TransformCoord(result, mat);
	return result;
}

XMMATRIX KazMath::CaluRotaMatrix(XMFLOAT3 ROTATION)
{
	XMMATRIX matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(ROTATION.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(ROTATION.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(ROTATION.y));

	return matRot;
}

XMMATRIX KazMath::CaluScaleMatrix(XMFLOAT3 SCALE)
{
	XMMATRIX matScale = XMMatrixIdentity();
	matScale = XMMatrixScaling(SCALE.x, SCALE.y, SCALE.z);

	return matScale;
}

XMMATRIX KazMath::CaluTransMatrix(XMFLOAT3 POS)
{
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(POS.x, POS.y, POS.z);

	return matTrans;
}

XMMATRIX KazMath::CaluRotaMatrix(XMVECTOR ROTATION)
{
	XMMATRIX matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(ROTATION.m128_f32[2]));
	matRot *= XMMatrixRotationX(XMConvertToRadians(ROTATION.m128_f32[0]));
	matRot *= XMMatrixRotationY(XMConvertToRadians(ROTATION.m128_f32[1]));

	return matRot;
}

XMMATRIX KazMath::CaluScaleMatrix(XMVECTOR SCALE)
{
	XMMATRIX matScale = XMMatrixIdentity();
	matScale = XMMatrixScaling(SCALE.m128_f32[0], SCALE.m128_f32[1], SCALE.m128_f32[2]);

	return matScale;
}

XMMATRIX KazMath::CaluTransMatrix(XMVECTOR POS)
{
	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(POS.m128_f32[0], POS.m128_f32[1], POS.m128_f32[2]);

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

XMFLOAT3 KazMath::CaluEyePosForDebug(XMFLOAT3 EYE_POS, XMFLOAT3 MOVE, XMFLOAT2 ANGLE)
{
	XMFLOAT3 resultPos = EYE_POS;

	float rad = ANGLE.x * 3.14 / 180.0f;
	float rad_2 = ANGLE.y * 3.14 / 180.0f;


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

XMFLOAT3 KazMath::CaluTargetPosForDebug(XMFLOAT3 EYE_POS, float ANGLE)
{
	float rad = ANGLE * 3.14 / 180.0f;

	XMFLOAT3 target;
	target.x = EYE_POS.x + 5 * sin(rad);
	target.y = EYE_POS.y;
	target.z = EYE_POS.z + 5 * cos(rad);

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

XMMATRIX KazMath::CaluSlopeMatrix(const XMVECTOR &Y, const XMVECTOR &Z)
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
	XMVECTOR axisX, axisY, axisZ;

	axisZ = Z;
	axisY = Y;

	if (Y.m128_f32[2] == 1.0f)
	{
		axisZ = { 0,-1,0 };
	}
	else if (Y.m128_f32[2] == -1.0f)
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

XMMATRIX KazMath::CaluFrontMatrix(const XMVECTOR &Y, const XMVECTOR &Z)
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
	XMVECTOR axisX, axisY, axisZ;

	axisZ = Z;
	axisY = Y;

	//普段要素
	if (Z.m128_f32[2] == 1.0f)
	{
		axisY = { 0,1,0 };
	}
	else if (Z.m128_f32[2] == -1.0f)
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

float KazMath::AngleToRadian(float ANGLE)
{
	float radian = static_cast<float>(ANGLE) * (PI_2F / 180.0f);
	return radian;
}

XMMATRIX KazMath::CaluMat(const KazMath::Transform3D &TRANSFORM, const XMVECTOR &Y_VEC, const XMVECTOR &Z_VEC, const XMMATRIX &VIEW_MAT, const XMMATRIX &PROJECT_MAT)
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
	if (Y_VEC.m128_f32[0] != 0.0f ||
		Y_VEC.m128_f32[1] != 1.0f ||
		Y_VEC.m128_f32[2] != 0.0f)
	{
		baseMatWorldData.matWorld *= KazMath::CaluSlopeMatrix(Y_VEC, { 0.0f,0.0f,1.0f });
	}
	if (Z_VEC.m128_f32[0] != 0.0f ||
		Z_VEC.m128_f32[1] != 0.0f ||
		Z_VEC.m128_f32[2] != 1.0f)
	{
		baseMatWorldData.matWorld *= KazMath::CaluFrontMatrix({ 0.0f,1.0f,0.0f }, Z_VEC);
	}
	baseMatWorldData.matWorld *= baseMatWorldData.matTrans;

	return baseMatWorldData.matWorld * VIEW_MAT * PROJECT_MAT;
}

XMMATRIX KazMath::CaluWorld(const KazMath::Transform3D &TRANSFORM, const XMVECTOR &Y_VEC, const XMVECTOR &Z_VEC)
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
	if (Y_VEC.m128_f32[0] != 0.0f ||
		Y_VEC.m128_f32[1] != 1.0f ||
		Y_VEC.m128_f32[2] != 0.0f)
	{
		baseMatWorldData.matWorld *= KazMath::CaluSlopeMatrix(Y_VEC, { 0.0f,0.0f,1.0f });
	}
	if (Z_VEC.m128_f32[0] != 0.0f ||
		Z_VEC.m128_f32[1] != 0.0f ||
		Z_VEC.m128_f32[2] != 1.0f)
	{
		baseMatWorldData.matWorld *= KazMath::CaluFrontMatrix({ 0.0f,1.0f,0.0f }, Z_VEC);
	}
	baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
	return baseMatWorldData.matWorld;
}

void KazMath::Larp(const float &BASE_TRANSFORM, float *TRANSFORM, float MUL)
{
	float distance = BASE_TRANSFORM - *TRANSFORM;
	*TRANSFORM += distance * MUL;
}

const XMVECTOR &KazMath::SplinePosition(const std::vector<XMVECTOR> &points, size_t startIndex, float t, bool Loop)
{
	if (startIndex < 1)
	{
		return points[1];
	}
	XMVECTOR p0 = points[startIndex - 1];
	XMVECTOR p1 = points[startIndex];
	XMVECTOR p2;
	XMVECTOR p3;
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
		if (startIndex > points.size() - 3)return points[points.size() - 3];
		p2 = points[startIndex + 1];
		p3 = points[startIndex + 2];
	}
	XMVECTOR anser = 0.5 * ((2 * p1 + (-p0 + p2) * t) +
		(2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) +
		(-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t)
	);

	return anser;
}

bool KazMath::CheckRayAndCircle(const XMVECTOR &RAY_START_POS, const XMVECTOR &RAY_END_POS, const XMVECTOR &CIRCLE_CENTRAL_POS, float RADIUS)
{
	//線の長さと、それぞれの始点と終点から円の中心までの長さ
	XMVECTOR startToCircle = CIRCLE_CENTRAL_POS - RAY_START_POS;
	XMVECTOR rayLength = RAY_END_POS- RAY_START_POS;
	XMVECTOR endToCircle = CIRCLE_CENTRAL_POS - RAY_END_POS;

	//単位化しそのベクトルと線分の視点と円のベクトルとで外積を計算する
	XMVECTOR normalRay = XMVector2Normalize(rayLength);
	XMVECTOR cross = XMVector2Cross(normalRay, startToCircle);
	
	if (cross.m128_f32[0] <= RADIUS)
	{
		XMVECTOR dot1 = XMVector2Dot(startToCircle, rayLength);
		XMVECTOR dot2 = XMVector2Dot(endToCircle, rayLength);

		if (dot1.m128_f32[0] * dot2.m128_f32[0] <= 0.0f)
		{
			return true;
		}

		XMVECTOR startToCircleLength = XMVector2Length(startToCircle);
		XMVECTOR endToCircleLength = XMVector2Length(endToCircle);
		if (startToCircleLength.m128_f32[0] <= RADIUS || endToCircleLength.m128_f32[0] <= RADIUS)
		{
			return true;
		}

	}
	else
	{
		return false;
	}

	return false;
}
;
