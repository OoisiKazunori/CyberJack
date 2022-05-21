#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

class Player
{
public:
	Player();
	void Init(const XMVECTOR &POS);
	void Finalize();
	void Input();
	void Update();
	void Draw();

	void Hit();

	bool isAlive();
	XMVECTOR pos;
private:
	short hp;
	BoxPolygonRenderPtr render;
};

