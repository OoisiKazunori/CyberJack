#include "SplineMisile.h"
#include"../KazLibrary/Math/KazMath.h"

SplineMisile::SplineMisile()
{
	maxTime = 60 * 5;
	splineBox.data.color = { 255.0f,0.0f,0.0f,255.0f };
}

void SplineMisile::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;
	iEnemy_EnemyStatusData->timer = maxTime;
	iOperationData.Init(1);

	startIndex = 0;
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

	pointTime = maxTime /( points.size() - 2);
}

void SplineMisile::Finalize()
{
}

void SplineMisile::Update()
{
	--iEnemy_EnemyStatusData->timer;

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

	splineBox.data.transform.pos = position;
	//���W�ݒ�----------------------------------------------



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
		pointsRender[i].Draw();
	}
	splineBox.Draw();
}
