#pragma once
#include"../DirectXCommon/Base.h"
#include"fbxsdk.h"

namespace KazMath
{
	/// <summary>
	/// 3D��ԏ�ŃI�u�W�F�N�g�𓮂����ۂ̃f�[�^�ł�
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
	/// 2D��ԏ�ŃI�u�W�F�N�g�𓮂����ۂ̃f�[�^�ł�
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
	/// �ڕW�n�_�ɓ��B����܂ł̈ړ��ʂ��v�Z���܂�
	/// </summary>
	/// <param name="A_POINT">A�n�_</param>
	/// <param name="B_POINT">B�n�_</param>
	/// <param name="FLAME">��b������̎���</param>
	/// <param name="MAX_FLAME">���B����</param>
	/// <returns>�ڕW�n�_�܂ł̈ړ���</returns>
	XMFLOAT3 CaluAPointToBPointVel(XMFLOAT3 A_POINT, XMFLOAT3 B_POINT, float FLAME, float MAX_FLAME);

	/// <summary>
	/// �X�N���[�����W���烏�[���h���W�ɕϊ����܂�
	/// </summary>
	/// <param name="SCREEN_POS">�X�N���[�����W</param>
	/// <param name="VIEW_MAT">�r���[�s��</param>
	/// <param name="PROJECTION_MAT">�v���W�F�N�V�����s��</param>
	/// <returns>���[���h���W</returns>
	XMVECTOR ConvertScreenPosToWorldPos(XMVECTOR SCREEN_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT);

	/// <summary>
	/// ���[���h���W����X�N���[�����W�ɕϊ����܂�
	/// </summary>
	/// <param name="SCREEN_POS">���[���h���W</param>
	/// <param name="VIEW_MAT">�r���[�s��</param>
	/// <param name="PROJECTION_MAT">�v���W�F�N�V�����s��</param>
	/// <returns>�X�N���[�����W</returns>
	XMVECTOR ConvertWorldPosToScreenPos(XMVECTOR WORLD_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT);


	/// <summary>
	/// ��]�s����v�Z���܂�
	/// </summary>
	/// <param name="ROTATION">�p�x</param>
	/// <returns>��]�s��</returns>
	XMMATRIX CaluRotaMatrix(XMFLOAT3 ROTATION);

	/// <summary>
	/// �X�P�[�����O�s����v�Z���܂�
	/// </summary>
	/// <param name="SCALE">�傫��</param>
	/// <returns>�X�P�[�����O�s��</returns>
	XMMATRIX CaluScaleMatrix(XMFLOAT3 SCALE);

	/// <summary>
	/// ���s�ړ��s����v�Z���܂�
	/// </summary>
	/// <param name="POS">���W</param>
	/// <returns>���s�ړ��s��</returns>
	XMMATRIX CaluTransMatrix(XMFLOAT3 POS);

	/// <summary>
	/// ��]�s����v�Z���܂�
	/// </summary>
	/// <param name="ROTATION">�p�x</param>
	/// <returns>��]�s��</returns>
	XMMATRIX CaluRotaMatrix(XMVECTOR ROTATION);

	/// <summary>
	/// �X�P�[�����O�s����v�Z���܂�
	/// </summary>
	/// <param name="SCALE">�傫��</param>
	/// <returns>�X�P�[�����O�s��</returns>
	XMMATRIX CaluScaleMatrix(XMVECTOR SCALE);

	/// <summary>
	/// ���s�ړ��s����v�Z���܂�
	/// </summary>
	/// <param name="POS">���W</param>
	/// <returns>���s�ړ��s��</returns>
	XMMATRIX CaluTransMatrix(XMVECTOR POS);


	/// <summary>
	/// �K�E�V�A���̑傫�����v�Z����
	/// </summary>
	/// <param name="count"></param>
	/// <param name="s"></param>
	/// <returns></returns>
	vector<float> GetGaussianWeights(size_t count, float s);

	/// <summary>
	/// �o�b�t�@�̃T�C�Y���A���C�����g����
	/// </summary>
	/// <param name="size">���X�̃T�C�Y</param>
	/// <param name="alignment">���o�C�g�ɂ�������</param>
	/// <returns>�o�b�t�@�̃T�C�Y</returns>
	unsigned int AligmentedValue(unsigned int size, unsigned int alignment);

	/// <summary>
	/// �K�E�V�A���̑傫�����v�Z����
	/// </summary>
	/// <param name="weightsTbl">�z��̐擪�A�h���X</param>
	/// <param name="sizeOfWeightsTbl">�z��̑傫��</param>
	/// <param name="sigma">�ǂꂭ�炢�ڂ������B�������傫����Α傫���قǂڂ�����</param>
	void CalcWeightsTableFromGaussian(float *weightsTbl, int sizeOfWeightsTbl, float sigma);


	/// <summary>
	/// �f�o�b�N�p�̎��_���W���v�Z���܂�
	/// �㉺���E���R�ɃJ�����𓮂��������\�ɂȂ�܂�
	/// </summary>
	/// <param name="EYE_POS">���_���W</param>
	/// <param name="ANGLE">�p�x</param>
	/// <returns>�f�o�b�N�p�̎��_���W</returns>
	XMFLOAT3 CaluEyePosForDebug(XMFLOAT3 EYE_POS, XMFLOAT3 MOVE, XMFLOAT2 ANGLE);

	/// <summary>
	/// �f�o�b�N�p�̒����_���v�Z���܂�
	/// </summary>
	/// <param name="EYE_POS">���_���W</param>
	/// <param name="ANGLE">�p�x</param>
	/// <returns>�f�o�b�N�p�̒����_</returns>
	XMFLOAT3 CaluTargetPosForDebug(XMFLOAT3 EYE_POS, float ANGLE);


	bool MatrixEqualOrNot(const XMMATRIX &MAT_1,const XMMATRIX &MAT_2);


	XMMATRIX CaluSlopeMatrix(const XMVECTOR &Y,const XMVECTOR &Z);
	XMMATRIX CaluFrontMatrix(const XMVECTOR &Y,const XMVECTOR &Z);

	int RadianToAngle(float RADIAN);
	float AngleToRadian(int ANGLE);


	XMMATRIX CaluMat(const KazMath::Transform3D &TRANSFORM, const XMVECTOR &Y_VEC, const XMVECTOR &Z_VEC, const XMMATRIX &VIEW_MAT, const XMMATRIX &PROJECT_MAT);
	XMMATRIX CaluWorld(const KazMath::Transform3D &TRANSFORM, const XMVECTOR &Y_VEC, const XMVECTOR &Z_VEC);

}

