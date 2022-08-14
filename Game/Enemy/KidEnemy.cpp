#include "KidEnemy.h"

KidEnemy::KidEnemy()
{
}

void KidEnemy::Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//���W�̏�����
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(4);							//�c�胍�b�N�I�������̏�����

	iEnemy_ModelRender->data.colorData = { 255,0,0,255 };

	demoFlag = DEMO_FLAG;
}

void KidEnemy::Finalize()
{
}

void KidEnemy::Update()
{
	
}

void KidEnemy::Draw()
{
	//iEnemy_ModelRender->Draw();
}
