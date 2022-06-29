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

	//���
	{
		float lMaxDistance = initFlamePos[RIGHT_UP].Distance(initFlamePos[LEFT_UP]);
		float lDistance = lMaxDistance / static_cast<float>(LINE_CPU_UPDOWN_NUM);
		std::array<KazMath::Vec3<float>, LINE_CPU_UPDOWN_NUM> lDiv;
		for (int i = 0; i < LINE_CPU_UPDOWN_NUM; ++i)
		{
			lDiv[i] = initFlamePos[LEFT_UP];
			lDiv[i].x += lDistance / 2.0f;
			lDiv[i].x += lDistance * static_cast<float>(i);
			upDownMemoryine[LINE_UPVEC][i].Init(LINE_UPVEC, lDiv[i]);

			upDownMemoryine[LINE_UPVEC][i].rate = (lDiv[i].x - initFlamePos[LEFT_UP].x) / lMaxDistance;
		}
	}

	//����
	{
		float lMaxDistance = initFlamePos[RIGHT_DOWN].Distance(initFlamePos[LEFT_DOWN]);
		float lDistance = lMaxDistance / static_cast<float>(LINE_CPU_UPDOWN_NUM);
		std::array<KazMath::Vec3<float>, LINE_CPU_UPDOWN_NUM> lDiv;
		for (int i = 0; i < LINE_CPU_UPDOWN_NUM; ++i)
		{
			lDiv[i] = initFlamePos[LEFT_DOWN];
			lDiv[i].x += lDistance / 2.0f;
			lDiv[i].x += lDistance * static_cast<float>(i);
			upDownMemoryine[LINE_DOWNVEC][i].Init(LINE_DOWNVEC, lDiv[i]);

			upDownMemoryine[LINE_DOWNVEC][i].rate = initFlamePos[LEFT_DOWN].Distance(lDiv[i]) / lMaxDistance;
		}
	}

	//����
	{
		float lMaxDistance = initFlamePos[LEFT_UP].Distance(initFlamePos[LEFT_DOWN]);
		float lDistance = lMaxDistance / static_cast<float>(LINE_CPU_NUM);
		std::array<KazMath::Vec3<float>, LINE_CPU_NUM> lDiv;
		for (int i = 0; i < LINE_CPU_NUM; ++i)
		{
			lDiv[i] = initFlamePos[LEFT_DOWN];
			lDiv[i].y += lDistance / 2.0f;
			lDiv[i].y += lDistance * static_cast<float>(i);
			sideMemoryine[LINE_LEFTVEC - 2][i].Init(LINE_LEFTVEC, lDiv[i]);

			sideMemoryine[LINE_LEFTVEC - 2][i].rate = initFlamePos[LEFT_DOWN].Distance(lDiv[i]) / lMaxDistance;
		}
	}

	//�E��
	{
		float lMaxDistance = initFlamePos[RIGHT_UP].Distance(initFlamePos[RIGHT_DOWN]);
		float lDistance = lMaxDistance / static_cast<float>(LINE_CPU_NUM);
		std::array<KazMath::Vec3<float>, LINE_CPU_NUM> lDiv;
		for (int i = 0; i < LINE_CPU_NUM; ++i)
		{
			lDiv[i] = initFlamePos[RIGHT_DOWN];
			lDiv[i].y += lDistance / 2.0f;
			lDiv[i].y += lDistance * static_cast<float>(i);
			sideMemoryine[LINE_RIGHTVEC - 2][i].Init(LINE_RIGHTVEC, lDiv[i]);

			sideMemoryine[LINE_RIGHTVEC - 2][i].rate = initFlamePos[RIGHT_DOWN].Distance(lDiv[i]) / lMaxDistance;
		}
	}

	for (int i = 0; i < flame.size(); ++i)
	{
		flame[i].data.pipelineName = PIPELINE_NAME_LINE_FLASHEFFECT;
		constBufferHandle[i] = flame[i].CreateConstBuffer(sizeof(XMFLOAT4), typeid(XMFLOAT4).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
	}


	for (int edge = 0; edge < LINE_CPU_UPDOWN_NUM; ++edge)
	{
		upDownMemoryine[LINE_UPVEC][edge].rate = 1.0f - upDownMemoryine[LINE_UPVEC][edge].rate;
	}
	for (int edge = 0; edge < LINE_CPU_NUM; ++edge)
	{
		sideMemoryine[LINE_LEFTVEC - 2][edge].rate = 1.0f - sideMemoryine[LINE_LEFTVEC - 2][edge].rate;
	}



	for (int i = 0; i < flashTimer.size(); ++i)
	{
		flashTimer[i] = 120;
	}
	flashTimer[0] = 0;
	flameIndex = 0;
	flashMaxTimer = 60;
	makeFlameMaxTimer = 10;
}

void PortalFlame::Update()
{
	//�|�[�^���g�̐����̉ߒ���`�悷��-------------------------
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

	if (makeFlameMaxTimer <= flameTimer)
	{
		++flameIndex;
		flameTimer = 0;
	}
	if (flame.size() <= flameIndex)
	{
		flameIndex = 3;
		flameTimer = makeFlameMaxTimer;
	}
	else
	{
		++flameTimer;
	}


	if (3 <= flameIndex && makeFlameMaxTimer <= flameTimer)
	{
		if (flashMaxTimer <= flashTimer[flameFlashIndex])
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

	//�t���b�V���p�̃f�[�^
	for (int i = 0; i < flame.size(); ++i)
	{
		lineEffectData[i].x = 1.0f;
		lineEffectData[i].y = 0.0f;
		lineEffectData[i].z = 0.0f;
		lineEffectData[i].w = static_cast<float>(flashTimer[i]) / static_cast<float>(flashMaxTimer);
		flame[i].TransData(&lineEffectData[i], constBufferHandle[i], typeid(XMFLOAT4).name());
	}


	//timer��������ǂꂭ�炢�L�΂������߂�
	{
		KazMath::Vec3<float> lDistance = lTmpVecPos[flameIndex][END] - lTmpVecPos[flameIndex][START];
		flame[flameIndex].data.startPos = lTmpVecPos[flameIndex][START];
		flame[flameIndex].data.endPos = lTmpVecPos[flameIndex][START];
		KazMath::Vec3<float> lAddVel = (lDistance * (static_cast<float>(flameTimer) / static_cast<float>(makeFlameMaxTimer)));
		flame[flameIndex].data.endPos = lTmpVecPos[flameIndex][START] + lAddVel;
		KazMath::CheckIsnan(&flame[flameIndex].data.endPos);
	}
	//�|�[�^���g�̐����̉ߒ���`�悷��-------------------------
	float lNowRate = 0.0f;
	{
		KazMath::Vec3<float>lCaluMaxRate;
		float lFlashDisntace = lTmpVecPos[flameFlashIndex][END].Distance(lTmpVecPos[flameFlashIndex][START]);
		float lMaxRate = lCaluMaxRate.Distance(lTmpVecPos[flameFlashIndex][END] - lTmpVecPos[flameFlashIndex][START]);
		float lFlashAddVel = (lFlashDisntace * (static_cast<float>(flashTimer[flameFlashIndex]) / static_cast<float>(flashMaxTimer)));
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

		//���E�̐��̐�
		if (vec == LINE_LEFTVEC || vec == LINE_RIGHTVEC)
		{
			for (int edge = 0; edge < sideMemoryine[vec - 2].size(); ++edge)
			{
				if (sideMemoryine[vec - 2][edge].rate <= lRate)
				{
					sideMemoryine[vec - 2][edge].FlashLight();
				}
			}
		}
		//�㉺�̐��̐�
		else
		{
			for (int edge = 0; edge < upDownMemoryine[vec].size(); ++edge)
			{
				if (upDownMemoryine[vec][edge].rate <= lRate)
				{
					upDownMemoryine[vec][edge].FlashLight();
				}
			}
		}
		lNowRate = lCaliNowRate;
	}



	//��������
	for (int i = 0; i < sideMemoryine.size(); ++i)
	{
		for (int edge = 0; edge < sideMemoryine[i].size(); ++edge)
		{
			sideMemoryine[i][edge].Update();
		}
	}
	for (int i = 0; i < upDownMemoryine.size(); ++i)
	{
		for (int edge = 0; edge < upDownMemoryine[i].size(); ++edge)
		{
			upDownMemoryine[i][edge].Update();
		}
	}

	if (flameFlashIndex != prevFlameFlashIndex)
	{
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


		//���E�̐��̐�
		if (vec == LINE_LEFTVEC || vec == LINE_RIGHTVEC)
		{
			//���݂̃t���[���̌��̈ʒu�̊�����0.0f�ȉ��Ȃ珉��������
			for (int edge = 0; edge < sideMemoryine[vec - 2].size(); ++edge)
			{
				sideMemoryine[vec - 2][edge].InitFlash();
			}
		}
		else
		{
			//���݂̃t���[���̌��̈ʒu�̊�����0.0f�ȉ��Ȃ珉��������
			for (int edge = 0; edge < upDownMemoryine[vec].size(); ++edge)
			{
				upDownMemoryine[vec][edge].InitFlash();
			}
		}
	}
	prevFlameFlashIndex = flameFlashIndex;
}

void PortalFlame::Draw()
{
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, L"Draw PortalLine");
	for (int i = 0; i < flame.size(); ++i)
	{
		flame[i].Draw();
	}

	for (int i = 0; i < sideMemoryine.size(); ++i)
	{
		for (int edge = 0; edge < sideMemoryine[i].size(); ++edge)
		{
			sideMemoryine[i][edge].Draw();
		}
	}

	for (int i = 0; i < upDownMemoryine.size(); ++i)
	{
		for (int edge = 0; edge < upDownMemoryine[i].size(); ++edge)
		{
			upDownMemoryine[i][edge].Draw();
		}
	}
	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());
}
