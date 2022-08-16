#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

class BattleshipEnemy:public IEnemy
{
public:
	BattleshipEnemy();

	void Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG);
	void Finalize();
	void Update();
	void Draw();

private:
	KazMath::Vec3<float>basePos;
	int appearTimer;
	float vel;
	int misileShotIndex;
	std::array<BoxPolygonRender, 8>misileR;
	FbxModelRenderPtr model;

	bool isShotFlag;
};

