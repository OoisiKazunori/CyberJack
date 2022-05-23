#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

class SplineMisile :public IEnemy
{
public:
	SplineMisile();

	void Init(const XMVECTOR &POS);
	void Finalize();
	void Update();
	void Draw();

private:
	int maxTime;		//���v����
	int pointTime;		//����_�Ԃ��ړ����鎞��
	int nowTime;		//���݂̎���

	//�X�v���C���Ȑ�----------------------------------------------
	std::vector<XMVECTOR> points;
	int startIndex;		//�ǂ̐���_���m���ړ����Ă��邩
	float timeRate;		//����_�Ԃ̎���
	XMVECTOR position;	//���ݒn
	BoxPolygonRender splineBox;
	std::array<BoxPolygonRender, 3> pointsRender;
	//�X�v���C���Ȑ�----------------------------------------------

};
