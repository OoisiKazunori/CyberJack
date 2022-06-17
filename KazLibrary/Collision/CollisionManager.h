#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include"../Render/KazRender.h"

struct Triangle
{
	XMVECTOR p0;
	XMVECTOR p1;
	XMVECTOR p2;
	XMVECTOR normal;

	void ComputeNormal()
	{
		XMVECTOR p0_p1 = p1 - p0;
		XMVECTOR p0_p2 = p2 - p0;

		//�O�ςɂ��A2�ӂɐ����ȃx�N�g�����Z�o����
		normal = XMVector3Cross(p0_p1, p0_p2);
		normal = XMVector3Normalize(normal);
	}
};

struct Ray
{
	//�n�_���W
	XMVECTOR start{ 0,0,0,1 };
	//����
	XMVECTOR dir{ 1,0,0,0 };

	void NormlizeDir()
	{
		float length = static_cast<float>(pow(
			(start.m128_f32[0] * start.m128_f32[0]) +
			(start.m128_f32[1] * start.m128_f32[1]) +
			(start.m128_f32[2] * start.m128_f32[2])
			, 0.5
		));

		XMVECTOR normal;
		normal = start / length;
		dir = XMVectorSet(normal.m128_f32[0], start.m128_f32[1], start.m128_f32[2], start.m128_f32[3]);
	}
};

struct Sphere
{
	// ���S���W
	XMVECTOR* center;
	// ���a
	float radius = 1.0f;
};

struct Plane
{
	// �@���x�N�g��
	XMVECTOR normal = { 0,1,0 };
	// ���_(0,0,0)����̋���
	float distance = 0.0f;
};

struct Square
{
	XMVECTOR center;
	XMVECTOR size;
};

struct ModiRectangle
{
	/*

		�f�[�^�͉��̊G�̂悤�ɍ���(p0)���玞�v���ɓ���Ă��������B

	   p0___________p1
		|			|
		|			|
	   p3___________p2

	*/

	XMVECTOR p0;
	XMVECTOR p1;
	XMVECTOR p2;
	XMVECTOR p3;
};

class CollisionManager :public ISingleton<CollisionManager> {
public:
	CollisionManager();
	~CollisionManager();

	/// <summary>
	/// �Ƌ��̔�������܂�
	/// </summary>
	/// <param name="SPHERE">���f�[�^</param>
	/// <param name="PLANE">�f�[�^</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckSphereAndPlane(const Sphere& SPHERE, const Plane& PLANE, XMVECTOR* INTER = nullptr);

	/// <summary>
	/// ���ƎO�p�`�̔�������܂�
	/// </summary>
	/// <param name="SPHERE">���f�[�^</param>
	/// <param name="TRIANGLE">�O�p�`�f�[�^</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckSphereAndTriangle(const Sphere& SPHERE, const Triangle& TRIANGLE, XMVECTOR* INTER = nullptr);

	/// <summary>
	/// ���Ɣ̔�������܂�
	/// </summary>
	/// <param name="RAY">���f�[�^</param>
	/// <param name="PLANE">�f�[�^</param>
	/// <param name="DISTANCE">����</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckRayAndPlane(const Ray& RAY, const Plane& PLANE, float* DISTANCE, XMVECTOR* INTER);

	/// <summary>
	/// ���ƎO�p�`�̔�������܂�
	/// </summary>
	/// <param name="RAY">���f�[�^</param>
	/// <param name="TRIANGLE">�O�p�`�f�[�^</param>
	/// <param name="DISTANCE">����</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckRayAndTriange(const Ray& RAY, const Triangle& TRIANGLE, float* DISTANCE = nullptr, XMVECTOR* INTER = nullptr);

	/// <summary>
	/// ���Ƌ��̔�������܂�
	/// </summary>
	/// <param name="RAY">���f�[�^</param>
	/// <param name="SPHERE">���f�[�^</param>
	/// <param name="DISTANCE">����</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckRayAndSphere(const Ray& RAY, const Sphere& SPHERE, float* DISTANCE = nullptr, XMVECTOR* INTER = nullptr);


	/// <summary>
	/// ���Ƌ��̔�������܂�
	/// </summary>
	/// <param name="SPHERE_1">���f�[�^1</param>
	/// <param name="SPHERE_2">���f�[�^2</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckSphereAndSphere(const Sphere& SPHERE_1, const Sphere& SPHERE_2);

	/// <summary>
	/// �����`�ƒ����`�̔�������܂�
	/// </summary>
	/// <param name="SQUARE_1">�l�p�f�[�^1</param>
	/// <param name="SQUARE_2">�l�p�f�[�^2</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckSquareAndSquare(const Square& SQUARE_1, const Square& SQUARE_2);

	/// <summary>
	/// ��]������`�Ƌ��̔�������܂�
	/// </summary>
	/// <param name="SPHERE"></param>
	/// <param name="RAY"></param>
	/// <returns></returns>
	bool CheckThicklineAndSphere(const Sphere& SPHERE, const ModiRectangle& RECTANGLE);

	friend ISingleton<CollisionManager>;
private:

	//�_�ƎO�p�`�̍ŋߐړ_
	void ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest);


	float CaluCross(XMFLOAT2 a, XMFLOAT2 b, XMFLOAT2 c)
	{
		return (b.x - a.x) * ((c.y * -1) - (a.y * -1)) - (c.x - a.x) * ((b.y * -1) - (a.y * -1));
	}
};

