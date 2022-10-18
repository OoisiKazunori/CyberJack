#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/UI/PlayerHpUi.h"
#include"../Game/Effect/PlayerDamageEmitter.h"
#include"../Game/UI/DamageTextWindow.h"

class Player
{
public:
	Player();
	void Init(const KazMath::Vec3<float> &POS, bool DRAW_UI_FLAG = true, bool APPEAR_FLAG = false);
	void Finalize();
	void Input();
	void Update();
	void Draw();

	void Hit();

	bool isAlive();
	KazMath::Vec3<float> pos;

	BoxPolygonRenderPtr render;
private:
	short hp,prevHp;
	int coolTimer;
	static const int COOL_MAX_TIME;
	bool coolTimeFlag;
	bool redFlag;

	bool drawHpFlag;

	PlayerHpUi hpUi;

	int damageSoundHandle;
	PlayerDamageEmitter damageEffect;
	DamageTextWindow damageWindow;
};

