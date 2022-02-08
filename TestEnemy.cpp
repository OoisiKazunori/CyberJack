#include "TestEnemy.h"

TestEnemy::TestEnemy()
{
	render.reset(new BoxPolygonRender);

	hitColor = { 255.0f,0.0f,0.0f,255.0f };
	normalColor = { 255.0f,255.0f,0.0f,255.0f };
}

void TestEnemy::Init(const XMVECTOR &POS, const int &HP)
{
	render->data.transform.pos = POS;
	hp = HP;

	deadFlag = false;
	hitFlag = false;
}

void TestEnemy::Finalize()
{
}

void TestEnemy::Update()
{
	if (!deadFlag)
	{




		//Hp��0�ȉ��ɂȂ�A�����[�X���肪����Ύ��S����
		if (hp <= 0 && hitFlag)
		{
			deadFlag = true;
		}
		hitFlag = false;//���t���[��false�ŏ��������鎖�Ŏ��̃����[�X����̏���������


		sphere.center = render->data.transform.pos;
		render->data.color = normalColor;
	}
	else
	{
		render->data.color = hitColor;
	}
}

void TestEnemy::Draw()
{
	render->Draw();
}

void TestEnemy::Hit()
{
	--hp;
}

void TestEnemy::Dead()
{
	hitFlag = true;
}

bool TestEnemy::AliveOrNot()
{
	return 0 < hp || !deadFlag;
}

bool TestEnemy::LockedOrNot()
{
	return hp <= 0;
}
