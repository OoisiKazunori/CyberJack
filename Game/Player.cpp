#include "Player.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

const int Player::COOL_MAX_TIME = 120;

Player::Player()
{
	render = std::make_unique<BoxPolygonRender>();
	render->data.pipelineName = PIPELINE_NAME_COLOR_MULTITEX;
	hp = -1;
	pos = {};

	damageSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "PlayerDamage.wav");

	RESOURCE_HANDLE lHandle = render->CreateConstBuffer(sizeof(DirectX::XMFLOAT4), typeid(DirectX::XMFLOAT4).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);

	DirectX::XMFLOAT4 lColor = { 0.0f,0.0f,0.0f,0.0f };
	render->TransData(&lColor, lHandle, typeid(DirectX::XMFLOAT4).name());

}

void Player::Init(const KazMath::Vec3<float> &POS, bool DRAW_UI_FLAG)
{
	pos = POS;
	render->data.transform.pos = pos;
	render->data.transform.scale = { 0.5f,1.3f,0.5f };
	render->data.transform.rotation = { 35.0f,0.0f,1.0f };
	hp = 3;

	hpUi.Init(hp);
	prevHp = hp;
	redFlag = false;
	coolTimer = 0;
	coolTimeFlag = false;

	drawHpFlag = DRAW_UI_FLAG;
}

void Player::Finalize()
{
	damageWindow.Finalize();
}

void Player::Input()
{
}

void Player::Update()
{
	render->data.transform.pos = pos;
	hpUi.Update();


	//----------HPが減ったらプレイヤーを赤くする----------
	if (hp != prevHp)
	{
		SoundManager::Instance()->PlaySoundMem(damageSoundHandle, 1);
		damageEffect.Init(pos);
		damageWindow.Init(pos + KazMath::Vec3<float>(WIN_X / 2.0f, WIN_Y / 2.0f + 200.0f, 0.0f));
		redFlag = true;
	}
	prevHp = hp;

	if (redFlag)
	{
		++coolTimer;

		if (COOL_MAX_TIME <= coolTimer)
		{
			redFlag = false;
			coolTimeFlag = false;
		}
	}
	else
	{
		coolTimer = 0;
	}

	if (redFlag)
	{
		render->data.color = { 255,0,0,255 };
	}
	else
	{
		render->data.color = { 255,255,255,255 };
	}
	//----------HPが減ったらプレイヤーを赤くする----------

	damageEffect.Update();
	damageWindow.Update();
}

void Player::Draw()
{
	render->Draw();
	damageEffect.Draw();
	damageWindow.Draw();

	if (drawHpFlag)
	{
		hpUi.Draw();
	}
}

void Player::Hit()
{
	if (!coolTimeFlag)
	{
		--hp;
		hpUi.Sub();
	}
	coolTimeFlag = true;
}

bool Player::isAlive()
{
	return 0 < hp;
}
