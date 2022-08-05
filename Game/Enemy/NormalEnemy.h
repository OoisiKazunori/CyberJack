#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

/// <summary>
/// �T���v���p�̓G
/// </summary>
class NormalEnemy:public IEnemy
{
public:
	NormalEnemy();

	void Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG);
	void Finalize();
	void Update();
	void Draw();

private:

};

