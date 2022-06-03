#include "PlayerHpUi.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Math/KazMath.h"

PlayerHpUi::PlayerHpUi()
{
	int hpHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::HpPath + "Hp.png");
	int hpFlameHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::HpPath + "HpFlame.png");

	for (int i = 0; i < hpTex.size(); ++i)
	{
		hpTex[i].data.handle = hpHandle;
		hpFlame[i].data.handle = hpFlameHandle;
	}
}

void PlayerHpUi::Init(int HP_MAX)
{
	maxHp = HP_MAX;
	hp = maxHp;

	timer[0] = 10;
	timer[1] = 5;
	timer[2] = 0;
	timer[3] = 15;
	timer[4] = 20;
}

void PlayerHpUi::Update()
{
	if (hp <= 0)
	{
		hp = 0;
	}
	if (maxHp <= hp)
	{
		hp = maxHp;
	}

	//for (int i = 4; 0 <= i; --i)
	for (int i = 0; i < 5; ++i)
	{
		++timer[i];
		if (20 <= timer[i])
		{
			lerpFlag[i] = !lerpFlag[i];
			timer[i] = 0;
		}

		if (lerpFlag[i])
		{
			lerpScale[i] = { 1.0f,1.5f };
		}
		else
		{
			lerpScale[i] = { 1.0f,1.0f };
		}
	}



	basePos = { 150.0f,650.0f };
	for (int i = 0; i < maxHp; ++i)
	{
		XMVECTOR space = { i * 70.0f,0.0f,0.0f };
		XMVECTOR pos = basePos + space;
		hpTex[i].data.transform.pos = pos;
		hpFlame[i].data.transform.pos = pos;

		KazMath::Larp(lerpScale[i].m128_f32[1], &hpTex[i].data.transform.scale.m128_f32[1], 0.2f);
	}


}

void PlayerHpUi::Draw()
{
	//Hp
	for (int i = 0; i < hp; ++i)
	{
		hpTex[i].Draw();
	}
	//Hp‚ÌƒtƒŒ[ƒ€
	for (int i = 0; i < maxHp; ++i)
	{
		hpFlame[i].Draw();
	}
}
