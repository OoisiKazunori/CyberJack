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
/// ���ꂽ�r���[�s���ۊǂ��A�w�肳�ꂽ�r���[�s���`��N���X�ɓn���܂�
/// </summary>
class CameraMgr :public ISingleton<CameraMgr>
{
public:
	CameraMgr();

	/// <summary>
	/// ����p�ƕ`�拗����ݒ肵�܂�
	/// </summary>
	/// <param name="VIEWING_ANGLE">����p</param>
	/// <param name="FAR_SIDE">�`�拗��</param>
	void CameraSetting(float VIEWING_ANGLE = 60.0f, float FAR_SIDE = 700.0f);

	/// <summary>
	///  ���W���󂯎��A�r���[�s����v�Z���܂�
	/// </summary>
	/// <param name="EYE_POS">���_���W</param>
	/// <param name="TARGET_POS">�����_</param>
	/// <param name="UP">��x�N�g��</param>
	void Camera(const KazMath::Vec3<float> &EYE_POS, const KazMath::Vec3<float> &TARGET_POS, const KazMath::Vec3<float> &UP, int CAMERA_INDEX = 0);

	XMMATRIX CreateCamera(const KazMath::Vec3<float> &EYE_POS,const KazMath::Vec3<float> &TARGET_POS,const KazMath::Vec3<float> &UP);

	/// <summary>
	/// �r���[�s���n���܂�
	/// </summary>
	/// <returns>�r���[�s��</returns>
	XMMATRIX GetViewMatrix(int CAMERA_INDEX = 0);

	XMMATRIX *GetViewMatrixPointer(int CAMERA_INDEX = 0);

	/// <summary>
	/// �r���{�[�h�s���n���܂�
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
	XMMATRIX GetMatBillBoard(int CAMERA_INDEX = 0);

	XMMATRIX *GetMatBillBoardPointer(int CAMERA_INDEX = 0);


	/// <summary>
	/// �������e�ϊ��s����󂯎��܂�
	/// </summary>
	/// <returns>�������e�ϊ��s��</returns>
	XMMATRIX GetPerspectiveMatProjection();
	XMMATRIX *GetPerspectiveMatProjectionPointer();

	// �������e�ϊ��s�������p���w�肵�Ď擾����B
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

// ���C�g�J�����萔�𑗂�p�̍\����
struct LightCamera {
	XMMATRIX viewProj;
};

