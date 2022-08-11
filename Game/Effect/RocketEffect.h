#pragma once
#include"../KazLibrary/Render/KazRender.h"

class RocketEffect
{
public:
	RocketEffect();

	void Init(KazMath::Vec3<float> *BASE_POS_PTR);
	void Update();
	void Draw();

private:
	KazMath::Vec3<float> *pos;

	KazMath::Vec3<float>fireTexPos;
	KazMath::Vec3<float>bloomTexPos;
	Sprite3DRender fireEffectRender;
	CircleRender lightEffectRender;

	int flashTimer;
	bool initFlag;
};

