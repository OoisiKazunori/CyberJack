#include "PortalFlame.h"

PortalFlame::PortalFlame()
{
}

void PortalFlame::Init(const KazMath::Vec3<float> &POS, const KazMath::Vec2<float> &SIZE)
{
	initFlamePos[LEFT_UP] =
	{
		POS.x - SIZE.x / 2.0f,
		POS.y + SIZE.y / 2.0f,
		POS.z
	};
	initFlamePos[LEFT_DOWN] =
	{
		POS.x - SIZE.x / 2.0f,
		POS.y - SIZE.y / 2.0f,
		POS.z
	};
	initFlamePos[RIGHT_UP] =
	{
		POS.x + SIZE.x / 2.0f,
		POS.y + SIZE.y / 2.0f,
		POS.z
	};
	initFlamePos[RIGHT_DOWN] =
	{
		POS.x + SIZE.x / 2.0f,
		POS.y - SIZE.y / 2.0f,
		POS.z
	};

	//上辺
	{
		float distance = (initFlamePos[RIGHT_UP].x - initFlamePos[LEFT_UP].x) / static_cast<float>(memoryLine.size());
		std::array<KazMath::Vec3<float>, 5> div;
		for (int i = 0; i < memoryLine.size(); ++i)
		{
			div[i] = initFlamePos[LEFT_UP];
			div[i].x += distance / 2.0f;
			div[i].x += distance * static_cast<float>(i);
			memoryLine[i][LINE_UPVEC].Init(LINE_UPVEC, div[i]);
		}
	}

	//下辺
	{
		float distance = (initFlamePos[RIGHT_DOWN].x - initFlamePos[LEFT_DOWN].x) / static_cast<float>(memoryLine.size());
		std::array<KazMath::Vec3<float>, 5> div;
		for (int i = 0; i < memoryLine.size(); ++i)
		{
			div[i] = initFlamePos[LEFT_DOWN];
			div[i].x += distance / 2.0f;
			div[i].x += distance * static_cast<float>(i);
			memoryLine[i][LINE_DOWNVEC].Init(LINE_DOWNVEC, div[i]);
		}
	}

	//左辺
	{
		float distance = (initFlamePos[LEFT_UP].y - initFlamePos[LEFT_DOWN].y) / static_cast<float>(memoryLine.size());
		std::array<KazMath::Vec3<float>, 5> div;
		for (int i = 0; i < memoryLine.size(); ++i)
		{
			div[i] = initFlamePos[LEFT_DOWN];
			div[i].y += distance / 2.0f;
			div[i].y += distance * static_cast<float>(i);
			memoryLine[i][LINE_LEFTVEC].Init(LINE_LEFTVEC, div[i]);
		}
	}

	//右辺
	{
		float distance = (initFlamePos[RIGHT_UP].y - initFlamePos[RIGHT_DOWN].y) / static_cast<float>(memoryLine.size());
		std::array<KazMath::Vec3<float>, 5> div;
		for (int i = 0; i < memoryLine.size(); ++i)
		{
			div[i] = initFlamePos[RIGHT_DOWN];
			div[i].y += distance / 2.0f;
			div[i].y += distance * static_cast<float>(i);
			memoryLine[i][LINE_RIGHTVEC].Init(LINE_RIGHTVEC, div[i]);
		}
	}

	for (int i = 0; i < flame.size(); ++i)
	{
		flame[i].data.pipelineName = PIPELINE_NAME_LINE_FLASHEFFECT;
		flameCbHandle[i] = flame[i].CreateConstBuffer(sizeof(XMFLOAT4), typeid(XMFLOAT4).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	}


	flameIndex = 0;
	maxTimer = 60;
}

void PortalFlame::Update()
{
	//ポータル枠の生成の過程を描画する-------------------------
	std::array<std::array<KazMath::Vec3<float>, 2>, 4>tmpVecPos;
	const int START = 0;
	const int END = 1;
	tmpVecPos[0][START] = initFlamePos[RIGHT_UP];
	tmpVecPos[0][END] = initFlamePos[LEFT_UP];
	tmpVecPos[1][START] = initFlamePos[LEFT_UP];
	tmpVecPos[1][END] = initFlamePos[LEFT_DOWN];
	tmpVecPos[2][START] = initFlamePos[LEFT_DOWN];
	tmpVecPos[2][END] = initFlamePos[RIGHT_DOWN];
	tmpVecPos[3][START] = initFlamePos[RIGHT_DOWN];
	tmpVecPos[3][END] = initFlamePos[RIGHT_UP];

	if (maxTimer <= flameTimer)
	{
		++flameIndex;
		flameTimer = 0;
	}
	if (flame.size() <= flameIndex)
	{
		flameIndex = 3;
		flameTimer = 60;
	}
	else
	{
		++flameTimer;
	}

	//フラッシュ用のデータ
	for (int i = 0; i < flame.size(); ++i)
	{
		lineEffectData[i].x = 1.0f;
		lineEffectData[i].y = 0.0f;
		lineEffectData[i].z = 0.0f;
		lineEffectData[i].w = static_cast<float>(20) / static_cast<float>(60);
		flame[i].TransData(&lineEffectData[i], flameCbHandle[i], typeid(XMFLOAT4).name());
	}


	//timerから線をどれくらい伸ばすか決める
	KazMath::Vec3<float> distance = tmpVecPos[flameIndex][END] - tmpVecPos[flameIndex][START];
	flame[flameIndex].data.startPos = tmpVecPos[flameIndex][START];
	flame[flameIndex].data.endPos = tmpVecPos[flameIndex][START];
	KazMath::Vec3<float> addVel = (distance * (static_cast<float>(flameTimer) / static_cast<float>(60)));
	flame[flameIndex].data.endPos = tmpVecPos[flameIndex][START] + addVel;
	KazMath::CheckIsnan(&flame[flameIndex].data.endPos);
	//ポータル枠の生成の過程を描画する-------------------------


	//メモリ線
	for (int i = 0; i < memoryLine.size(); ++i)
	{
		for (int edge = 0; edge < memoryLine[i].size(); ++edge)
		{
			memoryLine[i][edge].Update();
		}
	}

}

void PortalFlame::Draw()
{
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, L"Draw PortalLine");
	for (int i = 0; i < flame.size(); ++i)
	{
		flame[i].Draw();
	}

	for (int i = 0; i < memoryLine.size(); ++i)
	{
		for (int edge = 0; edge < memoryLine[i].size(); ++edge)
		{
			memoryLine[i][edge].Draw();
		}
	}
	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());
}
