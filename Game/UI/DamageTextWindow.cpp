#include "DamageTextWindow.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

const int DamageTextWindow::DISAPPER_MAX_TIME = 120;
const int DamageTextWindow::DRAW_MAX_TIME = DISAPPER_MAX_TIME + 60;

DamageTextWindow::DamageTextWindow()
{
	flame = std::make_unique<WindowFlame>();
	damageTextR = std::make_unique<Sprite2DRender>();

	damageTextR->data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::UIPath + "damage.png");
	damageTextR->data.pipelineName = PIPELINE_NAME_SPRITE_CUTALPHA;
}

void DamageTextWindow::Init(const KazMath::Vec3<float> &POS)
{
	pos = POS;
	flame->Init(KazMath::Transform3D(POS, KazMath::Vec3<float>(1.0f, 1.0f, 1.0f), KazMath::Vec3<float>(30.0f, 0.0f, 0.0f)), KazMath::Vec2<float>(1.4f, 1.0f), false, WINDOW_2D);
	flame->Start();
	appearTimer = 0;
}

void DamageTextWindow::Finalize()
{
}

void DamageTextWindow::Update()
{
	damageTextR->data.transform.pos = { pos.x,pos.y };
	flame->basePos = pos;
	flame->Update();

	++appearTimer;
	if (DISAPPER_MAX_TIME <= appearTimer)
	{
		flame->End();
	}
}

void DamageTextWindow::Draw()
{
	bool lDrawFlag = appearTimer < DRAW_MAX_TIME;
	if (10 <= appearTimer && appearTimer <= DISAPPER_MAX_TIME)
	{
		damageTextR->Draw();
	}
	if (lDrawFlag)
	{
		flame->Draw();
	}
}
