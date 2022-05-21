#include "Player.h"
#include"../KazLibrary/Imgui/MyImgui.h"
Player::Player()
{
	render = std::make_unique<BoxPolygonRender>();
	render->data.pipelineName = PIPELINE_NAME_COLOR_MULTITEX;
	hp = -1;
	pos = {};
}

void Player::Init(const XMVECTOR &POS)
{
	pos = POS;
	render->data.transform.pos = pos;
	render->data.transform.scale = { 0.5f,1.3f,0.5f };
	render->data.transform.rotation = { 35.0f,0.0f,1.0f };
	hp = 2;
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
	ImGui::Begin("Player");
	ImGui::InputFloat("ScaleX", &render->data.transform.scale.m128_f32[0]);
	ImGui::InputFloat("ScaleY", &render->data.transform.scale.m128_f32[1]);
	ImGui::InputFloat("ScaleZ", &render->data.transform.scale.m128_f32[2]);
	ImGui::InputFloat("RotaX", &render->data.transform.rotation.m128_f32[0]);
	ImGui::InputFloat("RotaY", &render->data.transform.rotation.m128_f32[1]);
	ImGui::InputFloat("RotaZ", &render->data.transform.rotation.m128_f32[2]);
	ImGui::End();
}

void Player::Draw()
{
	render->Draw();
}

void Player::Hit()
{
	--hp;
}

bool Player::isAlive()
{
	return 0 < hp;
}
