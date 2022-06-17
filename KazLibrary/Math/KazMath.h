#pragma once
#include"../DirectXCommon/Base.h"

#pragma warning(push)
#pragma warning(disable:4996)
#include"fbxsdk.h"
#pragma warning(pop)

namespace KazMath
{

#pragma region Vec2
	template<typename T>
	struct Vec2
	{
		T x, y;

		Vec2()
		{
			x = 0;
			y = 0;
		};
		Vec2(T X, T Y) :x(X), y(Y) {};

		float Length()const
		{
			return sqrt(pow(x, 2) + pow(y, 2));
		};
		float Distance(const Vec2 &To)const
		{
			return sqrt(pow(To.x - x, 2) + pow(To.y - y, 2));
		};
		Vec2<float> GetNormal()const
		{
			float len = Length();
			if (len == 0.0f)return Vec2<float>(0.0, 0.0);
			return Vec2<float>(x / len, y / len);
		};
		void Normalize()
		{
			float len = Length();
			x /= len;
			y /= len;
		};
		XMFLOAT2 ConvertXMFLOAT2()
		{
			return DirectX::XMFLOAT2(x, y);
		};

		Vec2<int>Int()const
		{
			return Vec2<int>(x, y);
		}
		Vec2<float>Float()const
		{
			return Vec2<float>((float)x, (float)y);
		}

		float Dot(const Vec2<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.x + me.y * rhs.y;
		}
		float Cross(const Vec2<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.y - me.y * rhs.x;
		}

#pragma region �I�y���[�^�[���Z�q
		Vec2 operator-() const
		{
			return Vec2(-x, -y);
		}
		Vec2 operator+(const Vec2 &rhs) const
		{
			return Vec2(x + rhs.x, y + rhs.y);
		};
		Vec2 operator-(const Vec2 &rhs)const
		{
			return Vec2(x - rhs.x, y - rhs.y);
		};
		Vec2 operator*(const Vec2 &rhs)const
		{
			return Vec2(x * rhs.x, y * rhs.y);
		};
		Vec2 operator*(const float &rhs)const
		{
			return Vec2(x * rhs, y * rhs);
		};
		Vec2 operator/(const Vec2 &rhs)const
		{
			return Vec2(x / rhs.x, y / rhs.y);
		};
		Vec2 operator/(const float &rhs)const
		{
			return Vec2(x / rhs, y / rhs);
		};
		Vec2 operator%(const Vec2 &rhs) const
		{
			return Vec2(fmodf(x, rhs.x), fmodf(y, rhs.y));
		};
		void operator=(const Vec2 &rhs)
		{
			x = rhs.x;
			y = rhs.y;
		};
		bool operator==(const Vec2 &rhs)const
		{
			return (x == rhs.x && y == rhs.y);
		};
		bool operator!=(const Vec2 &rhs)const
		{
			return !(*this == rhs);
		};
		void operator+=(const Vec2 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
		};
		void operator-=(const Vec2 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		};
		void operator*=(const Vec2 &rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
		};
		void operator/=(const Vec2 &rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
		};
		void operator%=(const Vec2 &rhs)
		{
			x = fmodf(x, rhs.x);
			y = fmodf(y, rhs.y);
		};

		void operator+=(const float &rhs)
		{
			x += rhs;
			y += rhs;
		};
		void operator-=(const float &rhs)
		{
			x -= rhs;
			y -= rhs;
		};
		void operator*=(const float &rhs)
		{
			x *= rhs;
			y *= rhs;
		};
		void operator/=(const float &rhs)
		{
			x /= rhs;
			y /= rhs;
		};
		void operator%=(const float &rhs)
		{
			x = fmodf(x, rhs);
			y = fmodf(y, rhs);
		};

		bool operator<(const Vec2<T> &rhs)
		{
			return x < rhs.x &&y < rhs.y;
		}
#pragma endregion
	};
#pragma endregion

#pragma region Vec3
	template<typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3() 
		{
			x = 0;
			y = 0;
			z = 0;
		};
		Vec3(T X, T Y, T Z) :x(X), y(Y), z(Z) {};
		float Length() const
		{
			return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		};
		Vec3(Vec2<T>XY, T Z) :x(XY.x), y(XY.y), z(Z) {};

		float Distance(const Vec3 &To)const
		{
			return sqrt(pow(To.x - x, 2) + pow(To.y - y, 2) + pow(To.z - z, 2));
		};
		Vec3<float> GetNormal()const
		{
			float len = Length();
			if (len == 0.0f)return Vec3(0, 0, 0);
			return Vec3<float>(x / len, y / len, z / len);
		};
		void Normalize()
		{
			float len = Length();
			x /= len;
			y /= len;
			z /= len;
		};
		XMFLOAT3 ConvertXMFLOAT3()
		{
			return DirectX::XMFLOAT3(x, y, z);
		};
		XMVECTOR ConvertXMVECTOR()
		{
			return XMVECTOR(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.0f);
		};

		Vec3<int>Int()const
		{
			return Vec3<int>(x, y, z); 
		}
		Vec3<float>Float()const
		{ 
			return Vec3<float>((float)x, (float)y, (float)z);
		}

		float Dot(const Vec3<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.x + me.y * rhs.y + me.z * rhs.z;
		}
		Vec3<float>Cross(const Vec3<float> &rhs)const
		{
			auto me = Float();
			return Vec3<float>(
				me.y * rhs.z - rhs.y * me.z,
				me.z * rhs.x - rhs.z * me.x,
				me.x * rhs.y - rhs.x * me.y);
		}

#pragma region �I�y���[�^�[���Z�q
		Vec3 operator-()const
		{
			return Vec3(-x, -y, -z);
		}
		Vec3 operator+(const Vec3 &rhs)const
		{
			return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
		};
		Vec3 operator-(const Vec3 &rhs)const
		{
			return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
		};
		Vec3 operator*(const Vec3 &rhs) const
		{
			return Vec3(x * rhs.x, y * rhs.y, z * rhs.z);
		};
		Vec3 operator*(const float &rhs)const
		{
			return Vec3(x * rhs, y * rhs, z * rhs);
		};
		Vec3 operator/(const Vec3 &rhs)const
		{
			return Vec3(x / rhs.x, y / rhs.y, z / rhs.z);
		};
		Vec3 operator/(const float &rhs)const
		{
			return Vec3(x / rhs, y / rhs, z / rhs);
		};
		Vec3 operator%(const Vec3 &rhs)const
		{
			return Vec3(fmodf(x, rhs.x), fmodf(y, rhs.y), fmodf(z, rhs.z));
		};
		void operator=(const Vec3 &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		};
		bool operator==(const Vec3 &rhs)const
		{
			return (x == rhs.x && y == rhs.y && z == rhs.z);
		};
		bool operator!=(const Vec3 &rhs)const
		{
			return !(*this == rhs);
		};
		void operator+=(const Vec3 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		};
		void operator+=(const float &rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
		};
		void operator-=(const Vec3 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
		};
		void operator-=(const float &rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
		};
		void operator*=(const Vec3 &rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
		};
		void operator*=(const float &rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
		};
		void operator/=(const Vec3 &rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
		};
		void operator/=(const float &rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
		};
		void operator%=(const Vec3 &rhs)
		{
			x = fmodf(x, rhs.x);
			y = fmodf(y, rhs.y);
			z = fmodf(z, rhs.z);
		};
#pragma endregion
	};
#pragma endregion




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


	XMVECTOR LoadFloat3ToVector(XMFLOAT3 NUM_2);
	XMFLOAT3 LoadVecotrToXMFLOAT3(XMVECTOR NUM_1);


	XMVECTOR CalculateScreenToWorld(XMVECTOR pout, XMMATRIX View, XMMATRIX Prj);

	template<typename T>
	Vec2<T> CaluAngle(const Vec2<T> &POS, float Angle, const Vec2<T> &CPos);
	template<typename T>
	Vec3<T> CaluAngle3D(const Vec3<T> &POS, float ANGLE, const Vec3<T> &CENTRAL_POS);

	template<typename T>
	T Rand(T MAX_NUM, T MIN_NUM);

	void ConvertMatrixFromFbx(XMMATRIX *DST, const FbxAMatrix &SRC);

	/// <summary>
	/// �b�����t���[���ɒ����܂�
	/// </summary>
	/// <param name="SECOND">�b��</param>
	/// <returns>�t���[��</returns>
	int ConvertSecondToFlame(int SECOND);

	/// <summary>
	/// �ڕW�n�_�ɓ��B����܂ł̈ړ��ʂ��v�Z���܂�
	/// </summary>
	/// <param name="A_POINT">A�n�_</param>
	/// <param name="B_POINT">B�n�_</param>
	/// <param name="FLAME">��b������̎���</param>
	/// <param name="MAX_FLAME">���B����</param>
	/// <returns>�ڕW�n�_�܂ł̈ړ���</returns>
	template <typename T>
	Vec3<T> CaluAPointToBPointVel(const Vec3<T> &A_POINT, const Vec3<T> &B_POINT, int FLAME, int MAX_FLAME);

	/// <summary>
	/// �X�N���[�����W���烏�[���h���W�ɕϊ����܂�
	/// </summary>
	/// <param name="SCREEN_POS">�X�N���[�����W</param>
	/// <param name="VIEW_MAT">�r���[�s��</param>
	/// <param name="PROJECTION_MAT">�v���W�F�N�V�����s��</param>
	/// <returns>���[���h���W</returns>
	Vec3<float> ConvertScreenPosToWorldPos(const Vec3<float> &SCREEN_POS, XMMATRIX VIEW_MAT, XMMATRIX PROJECTION_MAT);

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


	bool MatrixEqualOrNot(const XMMATRIX &MAT_1, const XMMATRIX &MAT_2);


	XMMATRIX CaluSlopeMatrix(const XMVECTOR &Y, const XMVECTOR &Z);
	XMMATRIX CaluFrontMatrix(const XMVECTOR &Y, const XMVECTOR &Z);

	int RadianToAngle(float RADIAN);
	float AngleToRadian(float ANGLE);


	XMMATRIX CaluMat(const KazMath::Transform3D &TRANSFORM, const XMVECTOR &Y_VEC, const XMVECTOR &Z_VEC, const XMMATRIX &VIEW_MAT, const XMMATRIX &PROJECT_MAT);
	XMMATRIX CaluWorld(const KazMath::Transform3D &TRANSFORM, const XMVECTOR &Y_VEC, const XMVECTOR &Z_VEC);


	void Larp(const float &BASE_TRANSFORM, float *TRANSFORM, float MUL);

	template<typename T>
	float GetSinFloat(T VALUE)
	{
		return static_cast<float>(sin(static_cast<float>VALUE));
	};

	template<typename T>
	float GetCosFloat(T VALUE)
	{
		return static_cast<float>(cos(static_cast<float>VALUE));
	};


	/// <summary>
	/// �X�v���C���Ȑ�
	/// </summary>
	/// <param name="points"></param>
	/// <param name="startIndex"></param>
	/// <param name="t"></param>
	/// <param name="Loop"></param>
	/// <returns></returns>
	const XMVECTOR &SplinePosition(const std::vector<XMVECTOR> &points, size_t startIndex, float t, bool Loop);

}

