#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/DirtyFlag.h"

enum CameraType
{
	CAMERA_TYPE_NONE = -1,
	CAMERA_TYPE_GAME,
	CAMERA_TYPE_DEBUG
};

struct CameraAxis
{
	XMVECTOR x;
	XMVECTOR y;
	XMVECTOR z;
	XMVECTOR upVec;
};

struct CameraData
{
	XMMATRIX view;
	XMMATRIX Perspectiveprojection;
	XMMATRIX orthographicMatProjection;
};


/// <summary>
/// 作られたビュー行列を保管し、指定されたビュー行列を描画クラスに渡します
/// </summary>
class CameraMgr :public ISingleton<CameraMgr>
{
public:
	CameraMgr();

	/// <summary>
	/// 視野角と描画距離を設定します
	/// </summary>
	/// <param name="VIEWING_ANGLE">視野角</param>
	/// <param name="FAR_SIDE">描画距離</param>
	void CameraSetting(float VIEWING_ANGLE = 60.0f, float FAR_SIDE = 700.0f);

	/// <summary>
	///  座標を受け取り、ビュー行列を計算します
	/// </summary>
	/// <param name="EYE_POS">視点座標</param>
	/// <param name="TARGET_POS">注視点</param>
	/// <param name="UP">上ベクトル</param>
	void Camera(const KazMath::Vec3<float> &EYE_POS, const KazMath::Vec3<float> &TARGET_POS, const KazMath::Vec3<float> &UP, int CAMERA_INDEX = 0);

	XMMATRIX CreateCamera(const KazMath::Vec3<float> &EYE_POS,const KazMath::Vec3<float> &TARGET_POS,const KazMath::Vec3<float> &UP);

	/// <summary>
	/// ビュー行列を渡します
	/// </summary>
	/// <returns>ビュー行列</returns>
	XMMATRIX GetViewMatrix(int CAMERA_INDEX = 0);

	XMMATRIX *GetViewMatrixPointer(int CAMERA_INDEX = 0);

	/// <summary>
	/// ビルボード行列を渡します
	/// </summary>
	/// <returns>ビルボード行列</returns>
	XMMATRIX GetMatBillBoard(int CAMERA_INDEX = 0);

	XMMATRIX *GetMatBillBoardPointer(int CAMERA_INDEX = 0);


	/// <summary>
	/// 透視投影変換行列を受け取ります
	/// </summary>
	/// <returns>透視投影変換行列</returns>
	XMMATRIX GetPerspectiveMatProjection();
	XMMATRIX *GetPerspectiveMatProjectionPointer();

	// 透視投影変換行列を視野角を指定して取得する。
	XMMATRIX GetPerspectiveMatProjectionAngle(float angle);


	bool Dirty(int CAMERA_INDEX = 0);
	void Record();

	XMMATRIX view;
	XMMATRIX billBoard;
	XMMATRIX perspectiveMat;
	XMMATRIX orthographicMatProjection;
private:
	static const int CAMERA_ARRAY_NUM = 2;
	std::array<XMMATRIX, CAMERA_ARRAY_NUM> viewArray;
	std::array<XMMATRIX, CAMERA_ARRAY_NUM> billBoardArray;

	std::array<std::unique_ptr<DirtySet>, CAMERA_ARRAY_NUM> viewDirtyFlagArray;
	std::array<std::unique_ptr<DirtySet>, CAMERA_ARRAY_NUM> billBoardDirtyFlagArray;
};

// ライトカメラ定数を送る用の構造体
struct LightCamera {
	XMMATRIX viewProj;
};

