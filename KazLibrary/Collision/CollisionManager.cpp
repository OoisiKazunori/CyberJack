#include"../Collision/CollisionManager.h"
#include "../Math/KazMath.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::CheckSphereAndPlane(const Sphere& SPHERE, const Plane& PLANE, XMVECTOR* INTER)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = XMVector3Dot(SPHERE.center, PLANE.normal);

	//���ʂ̌��_���������Z���鎖�ŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - PLANE.distance;

	//�����̐�Βl�����a���傫����Γ������ĂȂ�
	if (fabs(dist) > SPHERE.radius)
	{
		return false;
	}


	//�^�����_���v�Z
	if (INTER)
	{
		//���ʏ�̋^���ړ_���A�^����_�Ƃ���
		*INTER = -dist * PLANE.normal + SPHERE.center;
	}
	return true;
}

bool CollisionManager::CheckSphereAndTriangle(const Sphere& SPHERE, const Triangle& TRIANGLE, XMVECTOR* INTER)
{
	XMVECTOR p;

	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(SPHERE.center, TRIANGLE, &p);

	//�_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - SPHERE.center;

	//�����̓������߂�
	//(�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	v = XMVector3Dot(v, v);

	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > SPHERE.radius * SPHERE.radius)
	{
		return false;
	}

	if (INTER)
	{
		//�O�p�`��̍ŋߐړ_
		*INTER = p;
	}

}

bool CollisionManager::CheckRayAndPlane(const Ray& RAY, const Plane& PLANE, float* DISTANCE, XMVECTOR* INTER)
{
	//�C�v�V�I��...�덷�z���p��0�ɋ߂������Ȓl
	const float epsion = 1.0 - 5.0f;
	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = XMVector3Dot(PLANE.normal, RAY.dir).m128_f32[0];
	//���ʂɓ�����Ȃ�
	if (d1 > -epsion)
	{
		return false;
	}

	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	//�ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = XMVector3Dot(PLANE.normal, RAY.dir).m128_f32[0];

	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - PLANE.distance;

	//�n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;

	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0)
	{
		return false;
	}

	//��������������
	if (DISTANCE)
	{
		*DISTANCE = t;
	}

	//��_���v�Z
	if (INTER)
	{
		*INTER = RAY.start + t * RAY.dir;
	}

	return true;
}

bool CollisionManager::CheckRayAndTriange(const Ray& RAY, const Triangle& TRIANGLE, float* DISTANCE, XMVECTOR* INTER)
{
	//�O�p�`���ڂ��Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;

	plane.normal = TRIANGLE.normal;
	plane.distance = XMVector3Dot(TRIANGLE.normal, TRIANGLE.p0).m128_f32[0];

	//Ray�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRayAndPlane(RAY, plane, DISTANCE, &interPlane))
	{
		return false;
	}

	//Ray�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	//Ray�ƕ��ʂ̍D�]���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0 - 5.0f;
	XMVECTOR m;

	//��p0_p1�ɂ���
	XMVECTOR pt_p0 = TRIANGLE.p0 - interPlane;
	XMVECTOR p0_p1 = TRIANGLE.p1 - TRIANGLE.p0;
	m = XMVector3Cross(pt_p0, p0_p1);

	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, TRIANGLE.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}


	//��p1_p2�ɂ���
	XMVECTOR pt_p1 = TRIANGLE.p1 - interPlane;
	XMVECTOR p1_p2 = TRIANGLE.p1 - TRIANGLE.p2;
	m = XMVector3Cross(pt_p1, p1_p2);

	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, TRIANGLE.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}


	//��p2_p0�ɂ���
	XMVECTOR pt_p2 = TRIANGLE.p2 - interPlane;
	XMVECTOR p2_p0 = TRIANGLE.p2 - TRIANGLE.p0;
	m = XMVector3Cross(pt_p2, p2_p0);

	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, TRIANGLE.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}


	if (INTER)
	{
		*INTER = interPlane;
	}

	return true;
}

bool CollisionManager::CheckRayAndSphere(const Ray& RAY, const Sphere& SPHERE, float* DISTANCE, XMVECTOR* INTER)
{
	XMVECTOR m = RAY.start - SPHERE.center;
	float b = XMVector3Dot(m, RAY.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - SPHERE.radius * SPHERE.radius;

	//Ray�̎n�_��Sphere�̊O���ɂ���(c>0),Ray��Sphere���痣��Ă����������w���Ă���ꍇ(b>0),������Ȃ�
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	//���̔��ʎ���Ray�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f)
	{
		return false;
	}


	//Ray�͋��ƌ������Ă���
	//��������ŏ��̒lT���v�Z
	float t = -b - sqrtf(discr);

	//t�����ł���ꍇ�ARay�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	//�N�����v(�l��͈͓��ɓ����)
	if (t < 0)
	{
		t = 0.0f;
	}

	if (DISTANCE)
	{
		*DISTANCE = t;
	}

	if (INTER)
	{
		*INTER = RAY.start + t * RAY.dir;
	}
	return true;
}

bool CollisionManager::CheckSphereAndSphere(const Sphere& SPHERE_1, const Sphere& SPHERE_2)
{
	XMVECTOR tmp;
	tmp.m128_f32[0] = SPHERE_1.center.m128_f32[0] - SPHERE_2.center.m128_f32[0];
	tmp.m128_f32[1] = SPHERE_1.center.m128_f32[1] - SPHERE_2.center.m128_f32[1];
	tmp.m128_f32[2] = SPHERE_1.center.m128_f32[2] - SPHERE_2.center.m128_f32[2];


	float distance = sqrtf(pow(tmp.m128_f32[0], 2) + pow(tmp.m128_f32[1], 2) + pow(tmp.m128_f32[2], 2));
	float sumRadist = SPHERE_1.radius + SPHERE_2.radius;

	return (distance <= sumRadist);
}

bool CollisionManager::CheckSquareAndSquare(const Square& SQUARE_1, const Square& SQUARE_2)
{
	XMVECTOR adjPos[2] = { SQUARE_1.size / 2.0f,SQUARE_2.size / 2.0f };
	int x = 0, y = 2;
	bool isHit = fabs(SQUARE_1.center.m128_f32[x] - SQUARE_2.center.m128_f32[x]) <= adjPos[0].m128_f32[x] + adjPos[1].m128_f32[x]
		&& fabs(SQUARE_1.center.m128_f32[y] - SQUARE_2.center.m128_f32[y]) <= adjPos[0].m128_f32[y] + adjPos[1].m128_f32[y];
	return isHit;
}

bool CollisionManager::CheckThicklineAndSphere(const Sphere &SPHERE, const ModiRectangle &RECTANGLE)
{
	/*

	�Z ����̗���

	�E ��`���~�̔��a���A�אڂ��钸�_���牄�т�����Ɋg������B

		 ��			 ��
	  �� p0___________p1 ��
		 |			 |
		 |			 |
	  �� p3___________p2 ��
		 ��			 ��

	�E �~�̒��S�_�Ƌ�`�ƂŊO�ς��g�������E������g�p���āA�����蔻����s���B


	*���� �c�@��`�͍����p0�ɂ��āA���v���̏��ԂɃf�[�^�����Ă��������B

	*/



	// Z���̏���Y���ɓ����B

	Sphere sphereBuff = SPHERE;
	ModiRectangle rectangleBuff = RECTANGLE;

	sphereBuff.center.m128_f32[1] = SPHERE.center.m128_f32[2];
	sphereBuff.center.m128_f32[2] = SPHERE.center.m128_f32[1];

	rectangleBuff.p0.m128_f32[1] = RECTANGLE.p0.m128_f32[2];
	rectangleBuff.p0.m128_f32[2] = RECTANGLE.p0.m128_f32[1];

	rectangleBuff.p1.m128_f32[1] = RECTANGLE.p1.m128_f32[2];
	rectangleBuff.p1.m128_f32[2] = RECTANGLE.p1.m128_f32[1];

	rectangleBuff.p2.m128_f32[1] = RECTANGLE.p2.m128_f32[2];
	rectangleBuff.p2.m128_f32[2] = RECTANGLE.p2.m128_f32[1];

	rectangleBuff.p3.m128_f32[1] = RECTANGLE.p3.m128_f32[2];
	rectangleBuff.p3.m128_f32[2] = RECTANGLE.p3.m128_f32[1];


	// ��`�f�[�^��ۑ�����B
	ModiRectangle rectangleBuffBuff = rectangleBuff;

	// �~�̔��a��ۑ�����ϐ�
	XMVECTOR sphereRadius = { sphereBuff.radius, sphereBuff.radius, sphereBuff.radius };

	// ���_���v�Z����ۂɃf�[�^���ꎞ�ۑ�����ϐ�
	XMVECTOR dataBuff = {};
	XMVECTOR dataBuff2 = {};


	/*-- ��`���g������B --*/

	// p0�Ɋւ��Ċg���������s���B
	{

		// p1����p0�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff = KazMath::SubXMVECTOR(rectangleBuffBuff.p0, rectangleBuffBuff.p1);
		dataBuff = XMVector2Normalize(dataBuff);
		// p3����p0�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff2 = KazMath::SubXMVECTOR(rectangleBuffBuff.p0, rectangleBuffBuff.p3);
		dataBuff2 = XMVector2Normalize(dataBuff2);

		//// �����Đ��K������
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p0 += KazMath::MulXMVECTOR(dataBuff, sphereRadius);
		rectangleBuffBuff.p0 += KazMath::MulXMVECTOR(dataBuff2, sphereRadius);

	}

	// p1�Ɋւ��Ċg���������s���B
	{

		// p2����p1�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff = KazMath::SubXMVECTOR(rectangleBuffBuff.p1, rectangleBuffBuff.p2);
		dataBuff = XMVector2Normalize(dataBuff);
		// p0����p1�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff2 = KazMath::SubXMVECTOR(rectangleBuffBuff.p1, rectangleBuffBuff.p0);
		dataBuff2 = XMVector2Normalize(dataBuff2);

		//// �����Đ��K������
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p1 += KazMath::MulXMVECTOR(dataBuff, sphereRadius);
		rectangleBuffBuff.p1 += KazMath::MulXMVECTOR(dataBuff2, sphereRadius);

	}

	// p2�Ɋւ��Ċg���������s���B
	{

		// p3����p2�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff = KazMath::SubXMVECTOR(rectangleBuffBuff.p2, rectangleBuffBuff.p3);
		dataBuff = XMVector2Normalize(dataBuff);
		// p1����p2�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff2 = KazMath::SubXMVECTOR(rectangleBuffBuff.p2, rectangleBuffBuff.p1);
		dataBuff2 = XMVector2Normalize(dataBuff2);

		//// �����Đ��K������
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p2 += KazMath::MulXMVECTOR(dataBuff, sphereRadius);
		rectangleBuffBuff.p2 += KazMath::MulXMVECTOR(dataBuff2, sphereRadius);

	}

	// p3�Ɋւ��Ċg���������s���B
	{

		// p0����p1�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff = KazMath::SubXMVECTOR(rectangleBuffBuff.p3, rectangleBuffBuff.p0);
		dataBuff = XMVector2Normalize(dataBuff);
		// p2����p3�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff2 = KazMath::SubXMVECTOR(rectangleBuffBuff.p3, rectangleBuffBuff.p2);
		dataBuff2 = XMVector2Normalize(dataBuff2);

		//// �����Đ��K������
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p3 += KazMath::MulXMVECTOR(dataBuff, sphereRadius);
		rectangleBuffBuff.p3 += KazMath::MulXMVECTOR(dataBuff2, sphereRadius);

	}

	/*-- �g��������`�Ɖ~�̒��S�_���O�ς̍��E������g���ē����蔻����s���B --*/

	// �����ۑ�����悤�̕ϐ�
	vector<bool> isHit;

	// �O�ϔ��������ۂɃx�N�g����ۑ����邽�߂̕ϐ�
	XMVECTOR vTvVec = {};	// ���_�ƒ��_�̃x�N�g��
	XMVECTOR vTPVec = {};	// ���_�ƒ��S�_�̃x�N�g��

	// ���ʂ�ۑ�����ϐ�
	float crossResultBuff = 0;

	// p0->p1�ł̍��E����
	{

		// ���_�ƒ��_�x�N�g�������߂Đ��K��
		vTvVec = KazMath::SubXMVECTOR(rectangleBuffBuff.p1, rectangleBuffBuff.p0);
		vTvVec = XMVector2Normalize(vTvVec);
		// ���_�ƒ��S�_�̃x�N�g�������߂Đ��K��
		vTPVec = KazMath::SubXMVECTOR(sphereBuff.center, rectangleBuffBuff.p0);
		vTPVec = XMVector2Normalize(vTPVec);

		// �O�ς��v�Z +�̏ꍇ�E����
		float a = XMVector2Cross(vTvVec, vTPVec).m128_f32[0];
		isHit.push_back(XMVector2Cross(vTvVec, vTPVec).m128_f32[0] < 0);

	}

	// p1->p2�ł̍��E����
	{

		// ���_�ƒ��_�x�N�g�������߂Đ��K��
		vTvVec = KazMath::SubXMVECTOR(rectangleBuffBuff.p2, rectangleBuffBuff.p1);
		vTvVec = XMVector2Normalize(vTvVec);
		// ���_�ƒ��S�_�̃x�N�g�������߂Đ��K��
		vTPVec = KazMath::SubXMVECTOR(sphereBuff.center, rectangleBuffBuff.p1);
		vTPVec = XMVector2Normalize(vTPVec);

		// �O�ς��v�Z +�̏ꍇ�E����
		isHit.push_back(XMVector2Cross(vTvVec, vTPVec).m128_f32[0] < 0);

	}

	// p2->p3�ł̍��E����
	{

		// ���_�ƒ��_�x�N�g�������߂Đ��K��
		vTvVec = KazMath::SubXMVECTOR(rectangleBuffBuff.p3, rectangleBuffBuff.p2);
		vTvVec = XMVector2Normalize(vTvVec);
		// ���_�ƒ��S�_�̃x�N�g�������߂Đ��K��
		vTPVec = KazMath::SubXMVECTOR(sphereBuff.center, rectangleBuffBuff.p2);
		vTPVec = XMVector2Normalize(vTPVec);

		// �O�ς��v�Z +�̏ꍇ�E����
		isHit.push_back(XMVector2Cross(vTvVec, vTPVec).m128_f32[0] < 0);

	}

	// p3->p0�ł̍��E����
	{

		// ���_�ƒ��_�x�N�g�������߂Đ��K��
		vTvVec = KazMath::SubXMVECTOR(rectangleBuffBuff.p0, rectangleBuffBuff.p3);
		vTvVec = XMVector2Normalize(vTvVec);
		// ���_�ƒ��S�_�̃x�N�g�������߂Đ��K��
		vTPVec = KazMath::SubXMVECTOR(sphereBuff.center, rectangleBuffBuff.p3);
		vTPVec = XMVector2Normalize(vTPVec);

		// �O�ς��v�Z +�̏ꍇ�E����
		isHit.push_back(XMVector2Cross(vTvVec, vTPVec).m128_f32[0] < 0);

	}

	// ���ʂ��v�Z
	bool isHitResult = true;
	for (int i = 0; i < 4; ++i) {

		isHitResult = isHit[i] && isHitResult;

	}


	return isHitResult;
}
void CollisionManager::ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}
