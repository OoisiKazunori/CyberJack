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

	void Init(const XMVECTOR &POS);
	void Finalize();
	void Update();
	void Draw();

private:

};

