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

	int hp;
private:
	int maxHp;

	XMVECTOR basePos;
	std::array<Sprite2DRender, 5> hpTex;
	std::array<Sprite2DRender, 5> hpFlame;
};
