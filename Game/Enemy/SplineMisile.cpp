#include "SplineMisile.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

SplineMisile::SplineMisile()
{
	maxTime = 60 * 5;
	splineBox.data.color = { 255.0f,0.0f,0.0f,255.0f };

	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "misile.obj");
}

void SplineMisile::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;
	iEnemy_ModelRender->data.transform.scale = { 1.3f,1.3f,1.3f };
	iEnemy_EnemyStatusData->timer = maxTime;

	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;
	iEnemy_EnemyStatusData->hitBox.center = &iEnemy_ModelRender->data.transform.pos;
	iOperationData.Init(1);

	startIndex = 1;
	timeRate = 0.0f;


	points.clear();
	points.shrink_to_fit();

	XMVECTOR startPos = POS;
	XMVECTOR endPos = { 0.0f,0.0f,0.0f };

	XMVECTOR distance = startPos - endPos;
	float addDistance = distance.m128_f32[2] / 4.0f;

	distance.m128_f32[2] /= 2.0f;	//���Ԃ�Z�����̔���
	distance.m128_f32[2] -= addDistance;


	//�X�^�[�g�n�_
	points.push_back(startPos);
	points.push_back(startPos);

	//����
	points.push_back(distance);

	//�S�[���n�_
	points.push_back(endPos);
	points.push_back(endPos);

	pointTime = maxTime /( points.size() - 3);
}

void SplineMisile::Finalize()
{
}

void SplineMisile::Update()
{
	if (iOperationData.enableToHitFlag)
	{
		--iEnemy_EnemyStatusData->timer;
	}

	//�ړ�����----------------------------------------------
	timeRate = static_cast<float>(nowTime) / static_cast<float>(pointTime);
	if (1.0f <= timeRate)
	{
		if (startIndex < points.size() - 3)
		{
			startIndex++;
			timeRate = 0.0f;
			nowTime = 0;
		}
		else
		{
			timeRate = 1.0f;
		}
	}
	++nowTime;
	//�ړ�����----------------------------------------------


	//���W�ݒ�----------------------------------------------
	position = KazMath::SplinePosition(points, startIndex, timeRate, true);

	for (int i = 0; i < pointsRender.size(); ++i)
	{
		pointsRender[i].data.transform.pos = points[i];
	}

	iEnemy_ModelRender->data.transform.pos = position;
	//���W�ݒ�----------------------------------------------

	//�O�x�N�g���̐ݒ�----------------------------------------------
	float nextTimeRate = static_cast<float>(nowTime + 1) / static_cast<float>(pointTime);
	XMVECTOR nextpos = KazMath::SplinePosition(points, startIndex, nextTimeRate, true);

	XMVECTOR upVector = nextpos - position;
	upVector = XMVector3Normalize(upVector);
	iEnemy_ModelRender->data.upVector = upVector;
	//�O�x�N�g���̐ݒ�----------------------------------------------


	//���S����
	if (!iOperationData.enableToHitFlag)
	{
		iOperationData.initFlag = false;
	}
}

void SplineMisile::Draw()
{
	for (int i = 0; i < pointsRender.size(); ++i)
	{
		//pointsRender[i].Draw();
	}
	iEnemy_ModelRender->Draw();
}
