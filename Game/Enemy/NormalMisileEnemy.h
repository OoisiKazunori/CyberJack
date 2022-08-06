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
	int shotTimer;//�~�T�C�����˂܂ł̎���
	bool initShotFlag;

	int shotSoundHandle;
};
