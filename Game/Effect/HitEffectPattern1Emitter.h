#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../KazLibrary/Easing/easing.h"
#include"../Game/Effect/EaseScaleParticle.h"
#include<array>

/// <summary>
/// https://youtu.be/nvB6ga7w-2o
/// 動画の一番最初に敵を攻撃したら出るエフェクトの挙動
/// </summary>
class HitEffectPattern1Emitter
{
public:
	HitEffectPattern1Emitter();

	void Init(const KazMath::Vec2<float> &POS);
	void Update();
	void Draw();

	static const int PARTICLE_MAX = 4;
	std::array<KazMath::Vec2<float>, PARTICLE_MAX>baseScale;
	std::array<KazMath::Vec2<float>, PARTICLE_MAX>easeScale;
	std::array<KazMath::Vec2<int>, PARTICLE_MAX>timer;
private:

	std::array<EaseScaleParticle, PARTICLE_MAX>particle;
};

