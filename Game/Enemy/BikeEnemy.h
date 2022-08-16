#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

class BikeEnemy:public IEnemy
{
public:
	BikeEnemy();

	void Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG);
	void Finalize();
	void Update();
	void Draw();

private:
	KazMath::Vec3<float>basePos;
	KazMath::Vec3<float>kockBackPos;

	int appearTimer;
	LineRender line;

	static const float DISAPPEAR_Z_POS;

	BoxPolygonRender misileR, misileR2;
};

