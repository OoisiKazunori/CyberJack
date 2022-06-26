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
		float maxDistance = initFlamePos[RIGHT_UP].Distance(initFlamePos[LEFT_UP]);
		float distance = maxDistance / static_cast<float>(memoryLine.size());
		std::array<KazMath::Vec3<float>, 5> div;
		for (int i = 0; i < memoryLine.size(); ++i)
		{
			div[i] = initFlamePos[LEFT_UP];
			div[i].x += distance / 2.0f;
			div[i].x += distance * static_cast<float>(i);
			memoryLine[LINE_UPVEC][i].Init(LINE_UPVEC, div[i]);

			memoryLine[LINE_UPVEC][i].rate = initFlamePos[LEFT_UP].Distance(div[i]) / maxDistance;
		}
	}

	//下辺
	{
		float maxDistance = initFlamePos[RIGHT_DOWN].Distance(initFlamePos[LEFT_DOWN]);
		float distance = maxDistance / static_cast<float>(memoryLine.size());
		std::array<KazMath::Vec3<float>, 5> div;
		for (int i = 0; i < memoryLine.size(); ++i)
		{
			div[i] = initFlamePos[LEFT_DOWN];
			div[i].x += distance / 2.0f;
			div[i].x += distance * static_cast<float>(i);
			memoryLine[LINE_DOWNVEC][i].Init(LINE_DOWNVEC, div[i]);

			memoryLine[LINE_DOWNVEC][i].rate = initFlamePos[LEFT_DOWN].Distance(div[i]) / maxDistance;
		}
	}

	//左辺
	{
		float maxDistance = initFlamePos[LEFT_UP].Distance(initFlamePos[LEFT_DOWN]);
		float distance = maxDistance / static_cast<float>(memoryLine.size());
		std::array<KazMath::Vec3<float>, 5> div;
		for (int i = 0; i < memoryLine.size(); ++i)
		{
			div[i] = initFlamePos[LEFT_DOWN];
			div[i].y += distance / 2.0f;
			div[i].y += distance * static_cast<float>(i);
			memoryLine[LINE_LEFTVEC][i].Init(LINE_LEFTVEC, div[i]);

			memoryLine[LINE_LEFTVEC][i].rate = initFlamePos[LEFT_DOWN].Distance(div[i]) / maxDistance;
		}
	}

	//右辺
	{
		float maxDistance = initFlamePos[RIGHT_UP].Distance(initFlamePos[RIGHT_DOWN]);
		float distance = maxDistance / static_cast<float>(memoryLine.size());
		std::array<KazMath::Vec3<float>, 5> div;
		for (int i = 0; i < memoryLine.size(); ++i)
		{
			div[i] = initFlamePos[RIGHT_DOWN];
			div[i].y += distance / 2.0f;
			div[i].y += distance * static_cast<float>(i);
			memoryLine[LINE_RIGHTVEC][i].Init(LINE_RIGHTVEC, div[i]);

			memoryLine[LINE_RIGHTVEC][i].rate = initFlamePos[RIGHT_DOWN].Distance(div[i]) / maxDistance;
		}
	}

	for (int i = 0; i < flame.size(); ++i)
	{
		flame[i].data.pipelineName = PIPELINE_NAME_LINE_FLASHEFFECT;
		constBufferHandle[i] = flame[i].CreateConstBuffer(sizeof(XMFLOAT4), typeid(XMFLOAT4).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	}


	for (int edge = 0; edge < memoryLine.size(); ++edge)
	{
		memoryLine[LINE_UPVEC][edge].rate = 1.0f - memoryLine[LINE_UPVEC][edge].rate;
	}
	for (int edge = 0; edge < memoryLine.size(); ++edge)
	{
		memoryLine[LINE_LEFTVEC][edge].rate = 1.0f - memoryLine[LINE_LEFTVEC][edge].rate;
	}



	for (int i = 0; i < flashTimer.size(); ++i)
	{
		flashTimer[i] = 120;
	}
	flashTimer[0] = 0;
	flameIndex = 0;
	maxTimer = 60;
}

void PortalFlame::Update()
{
	//ポータル枠の生成の過程を描画する-------------------------
	std::array<std::array<KazMath::Vec3<float>, 2>, 4>lTmpVecPos;
	const int START = 0;
	const int END = 1;
	lTmpVecPos[0][START] = initFlamePos[RIGHT_UP];
	lTmpVecPos[0][END] = initFlamePos[LEFT_UP];
	lTmpVecPos[1][START] = initFlamePos[LEFT_UP];
	lTmpVecPos[1][END] = initFlamePos[LEFT_DOWN];
	lTmpVecPos[2][START] = initFlamePos[LEFT_DOWN];
	lTmpVecPos[2][END] = initFlamePos[RIGHT_DOWN];
	lTmpVecPos[3][START] = initFlamePos[RIGHT_DOWN];
	lTmpVecPos[3][END] = initFlamePos[RIGHT_UP];

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


	if (3 <= flameIndex && 60 <= flameTimer)
	{
		if (maxTimer <= flashTimer[flameFlashIndex])
		{
			flashTimer[flameFlashIndex] = 120;
			++flameFlashIndex;
			if (flameFlashIndex < flashTimer.size())
			{
				flashTimer[flameFlashIndex] = 0;
			}
		}
		if (flashTimer.size() <= flameFlashIndex)
		{
			flameFlashIndex = 0;
			flashTimer[flameFlashIndex] = 0;
		}
		else
		{
			++flashTimer[flameFlashIndex];
		}
	}

	//フラッシュ用のデータ
	for (int i = 0; i < flame.size(); ++i)
	{
		lineEffectData[i].x = 1.0f;
		lineEffectData[i].y = 0.0f;
		lineEffectData[i].z = 0.0f;
		lineEffectData[i].w = static_cast<float>(flashTimer[i]) / static_cast<float>(maxTimer);
		flame[i].TransData(&lineEffectData[i], constBufferHandle[i], typeid(XMFLOAT4).name());
	}


	//timerから線をどれくらい伸ばすか決める
	{
		KazMath::Vec3<float> lDistance = lTmpVecPos[flameIndex][END] - lTmpVecPos[flameIndex][START];
		flame[flameIndex].data.startPos = lTmpVecPos[flameIndex][START];
		flame[flameIndex].data.endPos = lTmpVecPos[flameIndex][START];
		KazMath::Vec3<float> lAddVel = (lDistance * (static_cast<float>(flameTimer) / static_cast<float>(60)));
		flame[flameIndex].data.endPos = lTmpVecPos[flameIndex][START] + lAddVel;
		KazMath::CheckIsnan(&flame[flameIndex].data.endPos);
	}
	//ポータル枠の生成の過程を描画する-------------------------
	float lNowRate = 0.0f;
	{
		KazMath::Vec3<float>lCaluMaxRate;
		float lFlashDisntace = lTmpVecPos[flameFlashIndex][END].Distance(lTmpVecPos[flameFlashIndex][START]);
		float lMaxRate = lCaluMaxRate.Distance(lTmpVecPos[flameFlashIndex][END] - lTmpVecPos[flameFlashIndex][START]);
		float lFlashAddVel = (lFlashDisntace * (static_cast<float>(flashTimer[flameFlashIndex]) / static_cast<float>(60)));
		float lCaliNowRate = lMaxRate - lFlashDisntace + lFlashAddVel;
		float lRate = lCaliNowRate / lMaxRate;

		LineEffectVec vec = LINE_UPVEC;
		switch (flameFlashIndex)
		{
		case 0:
			vec = LINE_UPVEC;
			break;
		case 1:
			vec = LINE_LEFTVEC;
			break;
		case 2:
			vec = LINE_DOWNVEC;
			break;
		case 3:
			vec = LINE_RIGHTVEC;
			break;
		default:
			break;
		}

		for (int edge = 0; edge < memoryLine[vec].size(); ++edge)
		{
			if (memoryLine[vec][edge].rate <= lRate)
			{
				memoryLine[vec][edge].FlashLight();
			}
		}
		lNowRate = lCaliNowRate;
	}



	//メモリ線
	for (int i = 0; i < memoryLine.size(); ++i)
	{
		for (int edge = 0; edge < memoryLine[i].size(); ++edge)
		{
			memoryLine[i][edge].Update();

			LineEffectVec vec = LINE_UPVEC;
			switch (flameFlashIndex)
			{
			case 0:
				vec = LINE_UPVEC;
				break;
			case 1:
				vec = LINE_LEFTVEC;
				break;
			case 2:
				vec = LINE_DOWNVEC;
				break;
			case 3:
				vec = LINE_RIGHTVEC;
				break;
			default:
				break;
			}

			//現在のフレームの光の位置の割合が0.0f以下なら初期化する
			if (lNowRate <= 1.0f && i == vec)
			{
				memoryLine[vec][edge].InitFlash();
			}
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
