#include "CameraMgr.h"
#include"../Math/KazMath.h"

CameraMgr::CameraMgr()
{
	perspectiveMat =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60.0f),
			(float)WIN_X / WIN_Y,
			0.1f,
			700.0f
		);


	//2D座標変換
	orthographicMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f,
		(float)WIN_X,
		(float)WIN_Y,
		0.0f,
		0.0f,
		1.0f
	);

	for (int i = 0; i < CAMERA_ARRAY_NUM; ++i)
	{
		viewArray[i] = XMMatrixIdentity();
		billBoardArray[i] = XMMatrixIdentity();
	}
}

void CameraMgr::CameraSetting(float VIEWING_ANGLE, float FAR_SIDE)
{
	perspectiveMat =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(VIEWING_ANGLE),
			(float)WIN_X / WIN_Y,
			0.1f,
			FAR_SIDE
		);
}

void CameraMgr::Camera(const KazMath::Vec3<float> &EYE_POS, const KazMath::Vec3<float> &TARGET_POS, const KazMath::Vec3<float> &UP, int CAMERA_INDEX)
{
	XMFLOAT3 eye = { EYE_POS.x,EYE_POS.y,EYE_POS.z };
	XMFLOAT3 target = { TARGET_POS.x,TARGET_POS.y ,TARGET_POS.z };
	XMFLOAT3 up = { UP.x,UP.y,UP.z };
	CameraAxis cameraAxis;


	bool eyeAll0 = (eye.x == 0 && eye.y == 0 && eye.z == 0);
	//bool targetAll0 = (target.x == 0 && target.y == 0 && target.z == 0);
	bool eyeEqualTarget = (eye.x == target.x && eye.y == target.y && eye.z == target.z);

	if (eyeAll0 || eyeEqualTarget)
	{
		return;
	}

#pragma region ビュー行列
	XMMATRIX matView;
	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// （仮の）上方向
	XMVECTOR upVector = XMLoadFloat3(&up);

	// カメラZ軸（視線方向）
	cameraAxis.z = XMVectorSubtract(targetPosition, eyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxis.z, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxis.z));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// ベクトルを正規化
	cameraAxis.z = XMVector3Normalize(cameraAxis.z);

	// カメラのX軸（右方向）
	// X軸は上方向→Z軸の外積で求まる
	cameraAxis.x = XMVector3Cross(upVector, cameraAxis.z);
	// ベクトルを正規化
	cameraAxis.x = XMVector3Normalize(cameraAxis.x);

	// カメラのY軸（上方向）
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxis.y = XMVector3Cross(cameraAxis.z, cameraAxis.x);

	cameraAxis.upVec = upVector;


	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxis.x;
	matCameraRot.r[1] = cameraAxis.y;
	matCameraRot.r[2] = cameraAxis.z;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);


	// 転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxis.x, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(cameraAxis.y, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxis.z, reverseEyePosition);	// Z成分
	const int x = 0;
	const int y = 1;
	const int z = 2;
	// 一つのベクトルにまとめる
	XMVECTOR translation = { tX.m128_f32[x], tY.m128_f32[y], tZ.m128_f32[z] };
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;
#pragma endregion

	//全方位ビルボード
	XMMATRIX matBillboard;
	matBillboard.r[0] = cameraAxis.x;
	matBillboard.r[1] = cameraAxis.y;
	matBillboard.r[2] = cameraAxis.z;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

#pragma region Y軸ビルボード行列
	// カメラX軸、Y軸、Z軸
	XMVECTOR billCameraAxisX, billCameraAxisY, billCameraAxisZ;

	// X軸は共通
	billCameraAxisX = cameraAxis.x;
	// Y軸はワールド座標系のY軸
	billCameraAxisY = XMVector3Normalize(cameraAxis.upVec);
	// Z軸はX軸→Y軸の外積で求まる
	billCameraAxisZ = XMVector3Cross(billCameraAxisX, billCameraAxisY);

	// Y軸回りビルボード行列
	//matBillboard.r[0] = billCameraAxisX;
	//matBillboard.r[1] = billCameraAxisY;
	//matBillboard.r[2] = billCameraAxisZ;
	//matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion

	viewArray[CAMERA_INDEX] = matView;
	billBoardArray[CAMERA_INDEX] = matBillboard;
	//view = matView;
	//billBoard = matBillboard;
}

XMMATRIX CameraMgr::CreateCamera(const KazMath::Vec3<float> &EYE_POS, const KazMath::Vec3<float> &TARGET_POS, const KazMath::Vec3<float> &UP)
{
	XMFLOAT3 eye = { EYE_POS.x,EYE_POS.y,EYE_POS.z };
	XMFLOAT3 target = { TARGET_POS.x, TARGET_POS.y, TARGET_POS.z };
	XMFLOAT3 up = { UP.x,UP.y,UP.z };
	CameraAxis cameraAxis;


	bool eyeAll0 = (eye.x == 0 && eye.y == 0 && eye.z == 0);
	//bool targetAll0 = (target.x == 0 && target.y == 0 && target.z == 0);
	bool eyeEqualTarget = (eye.x == target.x && eye.y == target.y && eye.z == target.z);

	if (eyeAll0 || eyeEqualTarget)
	{
		return XMMATRIX();
	}

#pragma region ビュー行列
	XMMATRIX matView;
	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// （仮の）上方向
	XMVECTOR upVector = XMLoadFloat3(&up);

	// カメラZ軸（視線方向）
	cameraAxis.z = XMVectorSubtract(targetPosition, eyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxis.z, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxis.z));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// ベクトルを正規化
	cameraAxis.z = XMVector3Normalize(cameraAxis.z);

	// カメラのX軸（右方向）
	// X軸は上方向→Z軸の外積で求まる
	cameraAxis.x = XMVector3Cross(upVector, cameraAxis.z);
	// ベクトルを正規化
	cameraAxis.x = XMVector3Normalize(cameraAxis.x);

	// カメラのY軸（上方向）
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxis.y = XMVector3Cross(cameraAxis.z, cameraAxis.x);

	cameraAxis.upVec = upVector;


	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxis.x;
	matCameraRot.r[1] = cameraAxis.y;
	matCameraRot.r[2] = cameraAxis.z;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);


	// 転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxis.x, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(cameraAxis.y, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxis.z, reverseEyePosition);	// Z成分
	const int x = 0;
	const int y = 1;
	const int z = 2;
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[x], tY.m128_f32[y], tZ.m128_f32[z], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;
#pragma endregion

	return matView;
}

const XMMATRIX &CameraMgr::GetViewMatrix(int CAMERA_INDEX)
{
	return viewArray[CAMERA_INDEX];
}

XMMATRIX *CameraMgr::GetViewMatrixPointer(int CAMERA_INDEX)
{
	return &viewArray[CAMERA_INDEX];
}

const XMMATRIX &CameraMgr::GetMatBillBoard(int CAMERA_INDEX)
{
	return billBoardArray[CAMERA_INDEX];
}

XMMATRIX *CameraMgr::GetMatBillBoardPointer(int CAMERA_INDEX)
{
	return &billBoardArray[CAMERA_INDEX];
}

const XMMATRIX &CameraMgr::GetPerspectiveMatProjection()
{
	return perspectiveMat;
}

XMMATRIX *CameraMgr::GetPerspectiveMatProjectionPointer()
{
	return &perspectiveMat;
}

XMMATRIX CameraMgr::GetPerspectiveMatProjectionAngle(float angle)
{
	XMMATRIX perspectiveMat =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(angle),
			(float)WIN_X / WIN_Y,
			0.1f,
			100000
		);

	return perspectiveMat;
}
