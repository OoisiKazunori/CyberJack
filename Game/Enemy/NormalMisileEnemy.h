#pragma once
#include"../Interface/IEnemy.h"

class NormalMisileEnemy:public IEnemy
{
public:
	NormalMisileEnemy();

	void Init(const XMVECTOR &POS);
	void Finalize();
	void Update();
	void Draw();

private:
	int shotTimer;//�~�T�C�����˂܂ł̎���
	bool initShotFlag;
};
