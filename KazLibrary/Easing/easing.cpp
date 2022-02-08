#include "../Easing/easing.h"


//関数のポインタ
static float (*FuncPtr[])(float t) = { InLinear, InSmooth, InQuad, InCubic, InQuart, InQuint, InSine, InCirc, InExp, InBack, InSoftBack, InElastic, InBounce };

//EasingType...In,Out,InOut、EaseInType...InLinear,InSmooth...、t...変化率
float EasingMaker(int ChooseType, int ChooseEase, float t) {

	if (ChooseType == In)
	{
		return FuncPtr[ChooseEase](t);
	}
	else if (ChooseType == Out)
	{
		return 1 - FuncPtr[ChooseEase](1 - t);
	}
	else if (ChooseType == InOut)
	{
		if (t < 0.5)
		{
			return FuncPtr[ChooseEase](2 * t) / 2;
		}
		else
		{
			return 1 - FuncPtr[ChooseEase](2 - 2 * t) / 2;
		}
	}

}


//EaseInの種類

float InLinear(float t)
{
	return t;
}
float InSmooth(float t)
{
	return t * t * (3 - t) / 2;
}
float InQuad(float t)
{
	return t * t;
}
float InCubic(float t)
{
	return t * t * t;
}
float InQuart(float t)
{
	return t * t * t * t;
}
float InQuint(float t)
{
	return t * t * t * t * t;
}
float InSine(float t)
{
	return 1 - cos(t * 3.14 / 2);
}
float InCirc(float t)
{
	return 1 - sqrt(Max(0, 1 - t * t));
}
float InExp(float t)
{
	return pow(2, -(1 - t) * 10);
}
float InBack(float t)
{
	return t * t * (2.70158 * t - 1.70158);
}
float InSoftBack(float t)
{
	return t * t * (2 * t - 1);
}
float InElastic(float t)
{
	return 56 * t * t * t * t * t - 105 * t * t * t * t + 60 * t * t * t - 10 * t * t;
}
float InBounce(float t)
{
	float pow2, bounce = 4;

	while (t < ((pow2 = pow(2, --bounce)) - 1) / 11) {

	}

	return 1 / pow(4, 3 - bounce) - 7.5625 * pow((pow2 * 3 - 2) / 22 - t, 2);
}


float Max(float a, float b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

void Rate(float *T, float RATE, float MAX_RATE)
{
	if (*T < MAX_RATE)
	{
		*T += RATE;
	}
	else if (MAX_RATE <= *T)
	{
		*T = MAX_RATE;
	}
}
