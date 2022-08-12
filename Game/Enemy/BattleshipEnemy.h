#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

class BattleshipEnemy:public IEnemy
{
public:
	BattleshipEnemy();

	void Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG);
	void Finalize();
	void Update();
	void Draw();

private:
};

