#include "HitEffectPattern1Emitter.h"

HitEffectPattern1Emitter::HitEffectPattern1Emitter()
{
	baseScale[0] = { 0.1f,0.1f };
	baseScale[1] = { 0.1f,0.1f };
	baseScale[2] = { 0.1f,0.1f };
	baseScale[3] = { 0.1f,0.1f };

	easeScale[0] = { 2.0f,5.3f };
	easeScale[1] = { 1.5f,2.0f };
	easeScale[2] = { 1.5f,1.5f };
	easeScale[3] = { 1.0f,1.0f };

	timer[0] = { 60,30 };
	timer[1] = { 60,60 };
	timer[2] = { 60,60 };
	timer[3] = { 60,60 };
}

void HitEffectPattern1Emitter::Init(const KazMath::Vec2<float> &POS)
{
	for (int i = 0; i < PARTICLE_MAX; ++i)
	{
		if (i == 0)
		{
			particle[i].Init(POS + KazMath::Vec2<float>(0.0f, 53.0f), baseScale[i], easeScale[i], timer[i]);
		}
		else
		{
			particle[i].Init(POS, baseScale[i], easeScale[i], timer[i]);
		}
	}

}

void HitEffectPattern1Emitter::Update()
{
	for (int i = 0; i < particle.size(); ++i)
	{
		particle[i].Update();
	}
}

void HitEffectPattern1Emitter::Draw()
{
	for (int i = 0; i < particle.size(); ++i)
	{
		particle[i].Draw();
	}
}
