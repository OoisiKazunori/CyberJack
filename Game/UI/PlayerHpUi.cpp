#include "PlayerHpUi.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

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

	basePos = { 150.0f,650.0f };
	for (int i = 0; i < maxHp; ++i)
	{
		XMVECTOR space = { i * 70.0f,0.0f,0.0f };
		XMVECTOR pos = basePos + space;
		hpTex[i].data.transform.pos = pos;
		hpFlame[i].data.transform.pos = pos;
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
