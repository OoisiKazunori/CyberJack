#include "PortalEffect.h"

PortalEffect::PortalEffect()
{
}

void PortalEffect::Init(const KazMath::Vec3<float> &POS)
{
	KazMath::Vec3<float> adj = { 0.0f,0.0f,45.0f };
	portalEffect.Init(POS + adj);
	portalFlame.Init(POS + adj, KazMath::Vec2<float>(41.5f, 23.5f));
}

void PortalEffect::Update()
{
	if (portalFlame.Flame())
	{
		portalEffect.Start();
	}

	/*if (20 < portalEffect.timer && !initWarpSoundFlag)
	{
		SoundManager::Instance()->PlaySoundMem(warpSoundHandle, 1);
		initWarpSoundFlag = true;
	}*/

	portalEffect.noiseSprite->data.transform.pos.z = portalFlame.basePos.z + 0.01f;
	portalEffect.noiseSprite->data.handleData = portalTexHandle;

	portalEffect.Update();
	portalFlame.Update();
}

void PortalEffect::Draw()
{
	portalEffect.Draw();
	portalFlame.Draw();
}

void PortalEffect::Start()
{
	portalFlame.Start();
}

bool PortalEffect::AllHidden()
{
	return portalEffect.AllHidden();
}

bool PortalEffect::DrawPrevStageFlag()
{
	return portalEffect.DrawPrevStageFlag();
}

bool PortalEffect::Reset()
{
	return portalEffect.noiseSprite->data.transform.pos.z <= -20.0f;
}

void PortalEffect::CheckCameraPos(float POS_Z)
{
	portalEffect.CheckCameraPos(POS_Z);
}
