#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

/// <summary>
/// �T���v���p�̎q�G
/// </summary>
class KidEnemy :public IEnemy
{
public:
	KidEnemy();

	void Init(const KazMath::Vec3<float> &POS);
	void Finalize();
	void Update();
	void Draw();

private:

};

