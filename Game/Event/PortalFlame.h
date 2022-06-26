#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/KazRender.h"
#include"LineCpuEffect.h"
#include<array>

class PortalFlame
{
public:
	PortalFlame();

	void Init(const KazMath::Vec3<float> &POS, const KazMath::Vec2<float> &SIZE);
	void Update();
	void Draw();

private:
	KazMath::Vec3<float>basePos;

	//フレーム-------------------------
	std::array<LineRender, 4>flame;
	std::array<RESOURCE_HANDLE, 4>flameCbHandle;
	std::array<XMFLOAT4, 4>lineEffectData;
	std::array<KazMath::Vec3<float>, 4>initFlamePos;
	int flameIndex;
	int flameTimer;
	int maxTimer;
	enum
	{
		LEFT_UP,
		LEFT_DOWN,
		RIGHT_UP,
		RIGHT_DOWN,
	};
	//フレーム-------------------------


	//メモリ線-------------------------
	std::array<std::array<LineCpuEffect, 5>, 4>memoryLine;
};

