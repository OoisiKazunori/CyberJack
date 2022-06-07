#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include<array>

class PlayerHpUi
{
public:
	PlayerHpUi();

	void Init(int HP_MAX);
	void Update();
	void Draw();

	void Sub();
	int hp;
private:
	int maxHp;
	int prevHp;
	bool disappearFlag;
	int disappearTimer;
	int subRedHpNum;

	XMVECTOR basePos;
	std::array<Sprite2DRender, 5> hpTex;
	std::array<Sprite2DRender, 5> redHpTex;
	std::array<XMVECTOR, 5> lerpScale;
	std::array<int, 5> timer;
	std::array<bool, 5> lerpFlag;
	std::array<Sprite2DRender, 5> hpFlame;
	Sprite2DRender hpBackGround;

	int damageTimer;
	bool subFlag;

	int redHpUiNum;
};
