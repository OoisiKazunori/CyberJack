#include "PortalFlame.h"

PortalFlame::PortalFlame()
{
}

void PortalFlame::Init(const KazMath::Vec3<float> &POS, const KazMath::Vec2<float> &SIZE)
{
	flamePos[LEFT_UP] =
	{
		POS.x - SIZE.x / 2.0f,
		POS.y - SIZE.y / 2.0f,
		POS.z
	};
	flamePos[LEFT_DOWN] =
	{
		POS.x - SIZE.x / 2.0f,
		POS.y + SIZE.y / 2.0f,
		POS.z
	};
	flamePos[RIGHT_UP] =
	{
		POS.x + SIZE.x / 2.0f,
		POS.y - SIZE.y / 2.0f,
		POS.z
	};
	flamePos[RIGHT_DOWN] =
	{
		POS.x + SIZE.x / 2.0f,
		POS.y + SIZE.y / 2.0f,
		POS.z
	};

	/*flame[0].data.startPos = flamePos[RIGHT_UP];
	flame[0].data.endPos = flamePos[LEFT_UP];
	flame[1].data.startPos = flamePos[LEFT_UP];
	flame[1].data.endPos = flamePos[LEFT_DOWN];
	flame[2].data.startPos = flamePos[LEFT_DOWN];
	flame[2].data.endPos = flamePos[RIGHT_DOWN];
	flame[3].data.startPos = flamePos[RIGHT_DOWN];
	flame[3].data.endPos = flamePos[RIGHT_UP];*/

	flameIndex = 0;
	maxTimer = 60;
}

void PortalFlame::Update()
{
	std::array<std::array<KazMath::Vec3<float>, 2>, 4>tmpVecPos;
	const int START = 0;
	const int END = 1;
	tmpVecPos[0][START] = flamePos[RIGHT_UP];
	tmpVecPos[0][END] = flamePos[LEFT_UP];
	tmpVecPos[1][START] = flamePos[LEFT_UP];
	tmpVecPos[1][END] = flamePos[LEFT_DOWN];
	tmpVecPos[2][START] = flamePos[LEFT_DOWN];
	tmpVecPos[2][END] = flamePos[RIGHT_DOWN];
	tmpVecPos[3][START] = flamePos[RIGHT_DOWN];
	tmpVecPos[3][END] = flamePos[RIGHT_UP];


	if (maxTimer < flameTimer)
	{
		++flameIndex;
		flameTimer = 0;
	}
	if (flame.size() <= flameIndex)
	{
		flameIndex = 3;
	}

	++flameTimer;
	KazMath::Vec3<float> distance = tmpVecPos[flameIndex][END] - tmpVecPos[flameIndex][START];
	flame[flameIndex].data.startPos = tmpVecPos[flameIndex][START];
	flame[flameIndex].data.endPos = tmpVecPos[flameIndex][START];
	flame[flameIndex].data.endPos = tmpVecPos[flameIndex][START] + distance - (distance / static_cast<float>(flameTimer));
	KazMath::CheckIsnan(&flame[flameIndex].data.endPos);
}

void PortalFlame::Draw()
{
	for (int i = 0; i < flame.size(); ++i)
	{
		flame[i].Draw();
	}
}
