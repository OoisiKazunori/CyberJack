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


	//2D���W�ϊ�
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

#pragma region �r���[�s��
	XMMATRIX matView;
	// ���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// �����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// �i���́j�����
	XMVECTOR upVector = XMLoadFloat3(&up);

	// �J����Z���i���������j
	cameraAxis.z = XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxis.z, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxis.z));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// �x�N�g���𐳋K��
	cameraAxis.z = XMVector3Normalize(cameraAxis.z);

	// �J������X���i�E�����j
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxis.x = XMVector3Cross(upVector, cameraAxis.z);
	// �x�N�g���𐳋K��
	cameraAxis.x = XMVector3Normalize(cameraAxis.x);

	// �J������Y���i������j
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxis.y = XMVector3Cross(cameraAxis.z, cameraAxis.x);

	cameraAxis.upVec = upVector;


	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxis.x;
	matCameraRot.r[1] = cameraAxis.y;
	matCameraRot.r[2] = cameraAxis.z;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);


	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxis.x, reverseEyePosition);	// X����
	XMVECTOR tY = XMVector3Dot(cameraAxis.y, reverseEyePosition);	// Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxis.z, reverseEyePosition);	// Z����
	const int x = 0;
	const int y = 1;
	const int z = 2;
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = { tX.m128_f32[x], tY.m128_f32[y], tZ.m128_f32[z] };
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;
#pragma endregion

	//�S���ʃr���{�[�h
	XMMATRIX matBillboard;
	matBillboard.r[0] = cameraAxis.x;
	matBillboard.r[1] = cameraAxis.y;
	matBillboard.r[2] = cameraAxis.z;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);

#pragma region Y���r���{�[�h�s��
	// �J����X���AY���AZ��
	XMVECTOR billCameraAxisX, billCameraAxisY, billCameraAxisZ;

	// X���͋���
	billCameraAxisX = cameraAxis.x;
	// Y���̓��[���h���W�n��Y��
	billCameraAxisY = XMVector3Normalize(cameraAxis.upVec);
	// Z����X����Y���̊O�ςŋ��܂�
	billCameraAxisZ = XMVector3Cross(billCameraAxisX, billCameraAxisY);

	// Y�����r���{�[�h�s��
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

#pragma region �r���[�s��
	XMMATRIX matView;
	// ���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// �����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	// �i���́j�����
	XMVECTOR upVector = XMLoadFloat3(&up);

	// �J����Z���i���������j
	cameraAxis.z = XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxis.z, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxis.z));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// �x�N�g���𐳋K��
	cameraAxis.z = XMVector3Normalize(cameraAxis.z);

	// �J������X���i�E�����j
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxis.x = XMVector3Cross(upVector, cameraAxis.z);
	// �x�N�g���𐳋K��
	cameraAxis.x = XMVector3Normalize(cameraAxis.x);

	// �J������Y���i������j
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxis.y = XMVector3Cross(cameraAxis.z, cameraAxis.x);

	cameraAxis.upVec = upVector;


	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxis.x;
	matCameraRot.r[1] = cameraAxis.y;
	matCameraRot.r[2] = cameraAxis.z;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);


	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxis.x, reverseEyePosition);	// X����
	XMVECTOR tY = XMVector3Dot(cameraAxis.y, reverseEyePosition);	// Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxis.z, reverseEyePosition);	// Z����
	const int x = 0;
	const int y = 1;
	const int z = 2;
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[x], tY.m128_f32[y], tZ.m128_f32[z], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
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
