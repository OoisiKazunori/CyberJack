#include "Player.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

Player::Player()
{
	render = std::make_unique<BoxPolygonRender>();
	render->data.pipelineName = PIPELINE_NAME_COLOR_MULTITEX;
	hp = -1;
	pos = {};

	damageSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "PlayerDamage.wav");

}

void Player::Init(const KazMath::Vec3<float> &POS)
{
	pos = POS;
	render->data.transform.pos = pos;
	render->data.transform.scale = { 0.5f,1.3f,0.5f };
	render->data.transform.rotation = { 35.0f,0.0f,1.0f };
	hp = 3;

	hpUi.Init(hp);
	prevHp = hp;
	redFlag = false;
	redTimer = 0;
}

void Player::Finalize()
{
}

void Player::Input()
{
}

void Player::Update()
{
	render->data.transform.pos = pos;
	/*ImGui::Begin("Player");
	ImGui::InputFloat("ScaleX", &render->data.transform.scale.m128_f32[0]);
	ImGui::InputFloat("ScaleY", &render->data.transform.scale.m128_f32[1]);
	ImGui::InputFloat("ScaleZ", &render->data.transform.scale.m128_f32[2]);
	ImGui::InputFloat("RotaX", &render->data.transform.rotation.m128_f32[0]);
	ImGui::InputFloat("RotaY", &render->data.transform.rotation.m128_f32[1]);
	ImGui::InputFloat("RotaZ", &render->data.transform.rotation.m128_f32[2]);
	ImGui::End();*/

	hpUi.Update();


	//----------HPが減ったらプレイヤーを赤くする----------
	if (hp != prevHp)
	{
		SoundManager::Instance()->PlaySoundMem(damageSoundHandle, 1);
		redFlag = true;
	}
	prevHp = hp;


	if (redFlag)
	{
		++redTimer;

		if (60 <= redTimer)
		{
			redFlag = false;
		}
	}
	else
	{
		redTimer = 0;
	}

	if (redFlag)
	{
		render->data.color = { 255.0f,0.0f,0.0f,255.0f };
	}
	else
	{
		render->data.color = { 255.0f,255.0f,255.0f,255.0f };
	}
	//----------HPが減ったらプレイヤーを赤くする----------

}

void Player::Draw()
{
	render->Draw();
	hpUi.Draw();
}

void Player::Hit()
{
	--hp;
	hpUi.Sub();
}

bool Player::isAlive()
{
	return 0 < hp;
}
