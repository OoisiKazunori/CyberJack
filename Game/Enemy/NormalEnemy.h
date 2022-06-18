#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"

/// <summary>
/// ƒTƒ“ƒvƒ‹—p‚Ì“G
/// </summary>
class NormalEnemy:public IEnemy
{
public:
	NormalEnemy();

	void Init(const KazMath::Vec3<float> &POS);
	void Finalize();
	void Update();
	void Draw();

private:

};

