#pragma once
#include"../Interface/IEnemy.h"
#include"../Game/Effect/RocketEffect.h"

class NormalMisileEnemy:public IEnemy
{
public:
	NormalMisileEnemy();

	void Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG = false);
	void Finalize();
	void Update();
	void Draw();

private:
	int shotTimer;//ミサイル発射までの時間
	bool initShotFlag;
	FbxModelRender model;
	int shotSoundHandle;
	bool startFlag;
};
