#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"
#include"../Game/Effect/RocketEffect.h"

class TutorialBlock :public IEnemy
{
public:
	TutorialBlock();

	void Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG);
	void Finalize();
	void Update();
	void Draw();

private:
	int timer;
	KazMath::Vec3<float>larpScale;
	BoxPolygonRender box;
};