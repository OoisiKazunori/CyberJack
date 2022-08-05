#pragma once
#include"../Interface/IEnemy.h"

class NormalMisileEnemy:public IEnemy
{
public:
	NormalMisileEnemy();

	void Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG = false);
	void Finalize();
	void Update();
	void Draw();

private:
	int shotTimer;//ƒ~ƒTƒCƒ‹”­ŽË‚Ü‚Å‚ÌŽžŠÔ
	bool initShotFlag;

	int shotSoundHandle;
};
