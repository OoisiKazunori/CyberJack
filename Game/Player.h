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

	bool IsAlive();
	KazMath::Vec3<float> pos;
	KazMath::Vec2<float> cameraRate;
private:
	short hp,prevHp;
	int coolTimer;
	static const int COOL_MAX_TIME;
	bool coolTimeFlag;
	bool redFlag;
	BoxPolygonRenderPtr render;
	FbxModelRender lRender,rRender;
	bool drawHpFlag;

	PlayerHpUi hpUi;

	int damageSoundHandle;
	PlayerDamageEmitter damageEffect;
	DamageTextWindow damageWindow;


	bool leftFlag;
	bool rightFlag;
	FbxTime totalTime;

	int larpTime;
	KazMath::Vec3<float>minScale;
	float sinTimer;
};

