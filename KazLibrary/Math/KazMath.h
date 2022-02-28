#pragma once
#include"../DirectXCommon/Base.h"
#include"fbxsdk.h"

namespace KazMath
{
	/// <summary>
	/// 3D空間上でオブジェクトを動かす際のデータです
	/// </summary>
	struct Transform3D
	{
		XMVECTOR pos;
		XMVECTOR scale;
		XMVECTOR rotation;

		Transform3D()
		{
			pos = { 0,0,0 };
			scale = { 1,1,1 };
			rotation = { 0,0,0 };
		};
	};

	/// <summary>
	/// 2D空間上でオブジェクトを動かす際のデータです
	/// </summary>
	struct Transform2D
	{
		XMVECTOR pos;
		XMVECTOR scale;
		float rotation;
		Transform2D()
		{
			pos = { 0,0 };
			scale = { 1,1 };
			rotation = 0;
		};
	};

	struct BaseMatWorldData
	{
		XMMATRIX matWorld;
		XMMATRIX matScale;
		XMMATRIX matRota;
		XMMATRIX matTrans;
	};


	static const float COLOR_MAX_VALUE = 255;



	static const float PI_2F = 3.1415926540f;
	static const XMMATRIX MAT_IDENTITY = XMMatrixIdentity();

	XMFLOAT3 AddXMFLOAT3(XMFLOAT3 NUM_1, XMFLOAT3 NUM_2);
	XMFLOAT2 AddXMFLOAT2(XMFLOAT2 NUM_1, XMFLOAT2 NUM_2);
	XMFLOAT3 SubXMFLOAT3(XMFLOAT3 NUM_1, XMFLOAT3 NUM_2);
	XMFLOAT2 SubXMFLOAT2(XMFLOAT2 NUM_1, XMFLOAT2 NUM_2);
	XMFLOAT3 DivXMFLOAT3(XMFLOAT3 NUM_1, XMFLOAT3 NUM_2);
	XMFLOAT3 MulXMFLOAT3(XMFLOAT3 NUM_1, XMFLOAT3 NUM_2);

	XMVECTOR AddXMVECTOR(XMVECTOR NUM_1, XMVECTOR NUM_2);
	XMVECTOR SubXMVECTOR(XMVECTOR NUM_1, XMVECTOR NUM_2);
	XMVECTOR DivXMVECTOR(XMVECTOR NUM_1, XMVECTOR NUM_2);
	XMVECTOR MulXMVECTOR(XMVECTOR NUM_1, XMVECTOR NUM_2);

	XMVECTOR LoadFloat3ToVector(XMFLOAT3 NUM_2);
	XMFLOAT3 LoadVecotrToXMFLOAT3(XMVECTOR NUM_1);

	XMFLOAT3 XMFLOAT3Normalize(XMFLOAT3 NUM_1);

	XMVECTOR CalculateScreenToWorld(XMVECTOR pout, XMMATRIX View, XMMATRIX Prj);

	XMFLOAT2 CaluAngle(XMFLOAT2 POS, float Angle, XMFLOAT2 CPos);
	XMFLOAT3 CaluAngle3D(XMFLOAT3 POS, float ANGLE, XMFLOAT3 CENTRAL_POS);
	XMFLOAT2 NewRota(float ANGLE, XMFLOAT2 POS);

	int IntRand(int MAX_NUM, int MIN_NUM);
	float FloatRand(float MAX_NUM, float MIN_NUM);
	void ConvertMatrixFromFbx(XMMATRIX *DST, const FbxAMatrix &SRC);

	float ConvertSecondToFlame(float SECOND);

	/// <summary>
	/// 目標地点に到達するまでの移動量を計算します
	/// </summary>
	/// <param name="A_POINT">A地点</param>
	/// <param name="B_POINT">B地点</param>
	/// <param name="FLAME">一秒あたりの時間</param>
	/// <param name="MAX_FLAME">到達時間</param>
	/// <returns>目標地点までの移動量</returns>
	XMFLOAT3 CaluAPointToBPointVel(XMFLOAT3 A_POINT, XMFLOAT3 B_POINT, float FLAME, float MAX_FLAME);

	/// <summary>
	/// スクリーン座標からワールド座標に変換します
	/// </summary>
	/// <param name="SCREEN_POS">スクリーン座標</param>
	/// <param name="VIEW_MAT">ビュー行列</param>
	/// <param name="PROJECTION_MAT">プロジェクション行列</param>
	/// <returns>ワールド座標</returns>
	XMVECTOR ConvertScreenPosToWorldPos(XMVECTOR SCREEN_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT);

	/// <summary>
	/// ワールド座標からスクリーン座標に変換します
	/// </summary>
	/// <param name="SCREEN_POS">ワールド座標</param>
	/// <param name="VIEW_MAT">ビュー行列</param>
	/// <param name="PROJECTION_MAT">プロジェクション行列</param>
	/// <returns>スクリーン座標</returns>
	XMVECTOR ConvertWorldPosToScreenPos(XMVECTOR WORLD_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT);


	/// <summary>
	/// 回転行列を計算します
	/// </summary>
	/// <param name="ROTATION">角度</param>
	/// <returns>回転行列</returns>
	XMMATRIX CaluRotaMatrix(XMFLOAT3 ROTATION);

	/// <summary>
	/// スケーリング行列を計算します
	/// </summary>
	/// <param name="SCALE">大きさ</param>
	/// <returns>スケーリング行列</returns>
	XMMATRIX CaluScaleMatrix(XMFLOAT3 SCALE);

	/// <summary>
	/// 平行移動行列を計算します
	/// </summary>
	/// <param name="POS">座標</param>
	/// <returns>平行移動行列</returns>
	XMMATRIX CaluTransMatrix(XMFLOAT3 POS);

	/// <summary>
	/// 回転行列を計算します
	/// </summary>
	/// <param name="ROTATION">角度</param>
	/// <returns>回転行列</returns>
	XMMATRIX CaluRotaMatrix(XMVECTOR ROTATION);

	/// <summary>
	/// スケーリング行列を計算します
	/// </summary>
	/// <param name="SCALE">大きさ</param>
	/// <returns>スケーリング行列</returns>
	XMMATRIX CaluScaleMatrix(XMVECTOR SCALE);

	/// <summary>
	/// 平行移動行列を計算します
	/// </summary>
	/// <param name="POS">座標</param>
	/// <returns>平行移動行列</returns>
	XMMATRIX CaluTransMatrix(XMVECTOR POS);


	/// <summary>
	/// ガウシアンの大きさを計算する
	/// </summary>
	/// <param name="count"></param>
	/// <param name="s"></param>
	/// <returns></returns>
	vector<float> GetGaussianWeights(size_t count, float s);

	/// <summary>
	/// バッファのサイズをアライメントする
	/// </summary>
	/// <param name="size">元々のサイズ</param>
	/// <param name="alignment">何バイトにしたいか</param>
	/// <returns>バッファのサイズ</returns>
	unsigned int AligmentedValue(unsigned int size, unsigned int alignment);

	/// <summary>
	/// ガウシアンの大きさを計算する
	/// </summary>
	/// <param name="weightsTbl">配列の先頭アドレス</param>
	/// <param name="sizeOfWeightsTbl">配列の大きさ</param>
	/// <param name="sigma">どれくらいぼかすか。数字が大きければ大きいほどぼかせる</param>
	void CalcWeightsTableFromGaussian(float *weightsTbl, int sizeOfWeightsTbl, float sigma);


	/// <summary>
	/// デバック用の視点座標を計算します
	/// 上下左右自由にカメラを動かす事が可能になります
	/// </summary>
	/// <param name="EYE_POS">視点座標</param>
	/// <param name="ANGLE">角度</param>
	/// <returns>デバック用の視点座標</returns>
	XMFLOAT3 CaluEyePosForDebug(XMFLOAT3 EYE_POS, XMFLOAT3 MOVE, XMFLOAT2 ANGLE);

	/// <summary>
	/// デバック用の注視点を計算します
	/// </summary>
	/// <param name="EYE_POS">視点座標</param>
	/// <param name="ANGLE">角度</param>
	/// <returns>デバック用の注視点</returns>
	XMFLOAT3 CaluTargetPosForDebug(XMFLOAT3 EYE_POS, float ANGLE);


	bool MatrixEqualOrNot(const XMMATRIX &MAT_1,const XMMATRIX &MAT_2);


	XMMATRIX CaluSlopeMatrix(const XMVECTOR &Y,const XMVECTOR &Z);
	XMMATRIX CaluFrontMatrix(const XMVECTOR &Y,const XMVECTOR &Z);

	int RadianToAngle(float RADIAN);
	float AngleToRadian(int ANGLE);


	XMMATRIX CaluMat(const KazMath::Transform3D &TRANSFORM, const XMVECTOR &Y_VEC, const XMVECTOR &Z_VEC, const XMMATRIX &VIEW_MAT, const XMMATRIX &PROJECT_MAT);
	XMMATRIX CaluWorld(const KazMath::Transform3D &TRANSFORM, const XMVECTOR &Y_VEC, const XMVECTOR &Z_VEC);

}

