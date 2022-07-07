#include "LockOnBoxEffect.h"

const int LockOnBoxEffect::BOX_DISAPPEAR_MAX_TIMER = 60;
const float LockOnBoxEffect::BOX_ROTA_SPEED = 1.0f;

LockOnBoxEffect::LockOnBoxEffect()
{
	boxDisappearTimer = 0;
	aliveFlag = false;
}

void LockOnBoxEffect::Start(KazMath::Vec2<float> *POS)
{
	if (!aliveFlag)
	{
		lockOnPos = POS;
		boxAngle = 0.0f;


		boxSize[LEFT_UP] = { -100.0f,-100.0f };
		boxSize[LEFT_DOWN] = { -100.0f,100.0f };
		boxSize[RIGHT_UP] = { 100.0f,-100.0f };
		boxSize[RIGHT_DOWN] = { 100.0f,100.0f };

		aliveFlag = true;
	}
}

void LockOnBoxEffect::Update()
{
	if (boxDisappearTimer < BOX_DISAPPEAR_MAX_TIMER && aliveFlag)
	{
		++boxDisappearTimer;
	}
	else
	{
		boxDisappearTimer = BOX_DISAPPEAR_MAX_TIMER;
		aliveFlag = false;
	}

	KazMath::Vec2<float>lPos = {};
	//ç∂è„
	lPos = *lockOnPos + boxSize[LEFT_UP];
	CaluBox(lPos, *lockOnPos, &box.data.leftUpPos, boxDisappearTimer, BOX_DISAPPEAR_MAX_TIMER);
	//âEâ∫
	lPos = *lockOnPos + boxSize[RIGHT_DOWN];
	CaluBox(lPos, *lockOnPos, &box.data.rightDownPos, boxDisappearTimer, BOX_DISAPPEAR_MAX_TIMER);
	//ç∂â∫
	lPos = *lockOnPos + boxSize[LEFT_DOWN];
	CaluBox(lPos, *lockOnPos, &box.data.leftDownPos, boxDisappearTimer, BOX_DISAPPEAR_MAX_TIMER);
	//âEè„
	lPos = *lockOnPos + boxSize[RIGHT_UP];
	CaluBox(lPos, *lockOnPos, &box.data.rightUpPos, boxDisappearTimer, BOX_DISAPPEAR_MAX_TIMER);
}

void LockOnBoxEffect::Draw()
{
	box.Draw();
}

bool LockOnBoxEffect::IsAlive()
{
	return aliveFlag;
}
