#include "Player.h"

Player::Player()
{
	render.reset(new BoxPolygonRender);
}

void Player::Init(const XMVECTOR &POS)
{
	pos = POS;
	render->data.transform.pos = pos;
}

void Player::Finalize()
{
}

void Player::Input()
{
}

void Player::Update()
{
}

void Player::Draw()
{
	render->Draw();
}
