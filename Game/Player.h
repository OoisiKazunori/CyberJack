#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/UI/PlayerHpUi.h"

class Player
{
public:
	Player();
	void Init(const KazMath::Vec3<float> &POS, bool DRAW_UI_FLAG = true);
	void Finalize();
	void Input();
	void Update();
	void Draw();

	void Hit();

	bool isAlive();
	KazMath::Vec3<float> pos;
private:
	short hp,prevHp;
	bool redFlag;
	int redTimer;
	BoxPolygonRenderPtr render;
	bool drawHpFlag;

	PlayerHpUi hpUi;


	int damageSoundHandle;

};

