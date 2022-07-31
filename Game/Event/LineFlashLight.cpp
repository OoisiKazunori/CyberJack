#include "LineFlashLight.h"
#include"../KazLibrary/Helper/KazHelper.h"

LineFlashLight::LineFlashLight()
{
}

void LineFlashLight::Init(const std::vector<KazMath::Vec3<float>> &POS_ARRAY, int TIMER)
{
	posArray = POS_ARRAY;

	//�S���̎Z�o
	float distance = 0.0f;
	for (int i = 0; i < posArray.size(); ++i)
	{
		if (i + 1 < posArray.size())
		{
			distance += posArray[i + 1].Distance(posArray[i]);
		}
		else
		{
			break;
		}
	}
	//1Flame�̃X�s�[�h
	speed = distance / static_cast<float>(TIMER);

	lineIndex = 0;
	boxR.data.transform.pos = posArray[lineIndex];

	finishFlag = false;
}

void LineFlashLight::Update()
{
	//�{�b�N�X���ǂ̕����Ɍ����킹�邩���߂�
	CheckWhereToGo();

	//�{�b�N�X�𓮂���
	if (nowPos != nullptr)
	{
		*nowPos += speed;
	}

	//���𒴂����玟�̐��ɍs��
	if (nowPos != nullptr && nextPos != nullptr && *nextPos - *nowPos <= 0.0f)
	{
		++lineIndex;

		int lLoopCount = 0;
		float lDistance = 0.0f;
		//��x�ɐ����z�����񐔂�100��ȉ�&&�����z���Ȃ����������[�v���I����
		while (lLoopCount <= 100 && 0.0f <= lDistance)
		{
			//�����z��������
			lDistance = *nextPos - *nowPos;
			if (lDistance <= -0.1f)
			{
				boxR.data.transform.pos = posArray[lineIndex];
				//�Ō�̐���_�Ȃ�I������
				if (!KazHelper::IsitInAnArray(lineIndex + 1, posArray.size()))
				{
					break;
				}
				CheckWhereToGo();
				*nowPos += lDistance;

				++lLoopCount;
			}
		}
	}
}

void LineFlashLight::Draw()
{
	boxR.Draw();
}

bool LineFlashLight::IsFinish()
{
	return finishFlag;
}

void LineFlashLight::CheckWhereToGo()
{
	KazMath::Vec3<float> lDistance = posArray[lineIndex + 1] - posArray[lineIndex];

	if (0.1f <= abs(lDistance.x))
	{
		//X���̋�����- && ���݂̃X�s�[�h��-�ł͂Ȃ��ꍇ��-�����ɐi�܂���
		if (signbit(lDistance.x) && !signbit(speed))
		{
			speed *= -1.0f;
		}
		//X���̋�����-�ł͂Ȃ� && ���݂̃X�s�[�h��-�̏ꍇ+�����ɐi�܂���
		else if (!signbit(lDistance.x) && signbit(speed))
		{
			speed *= -1.0f;
		}
		nowPos = &boxR.data.transform.pos.x;
		nextPos = &posArray[lineIndex + 1].x;
	}
	else if (0.1f <= abs(lDistance.y))
	{
		//Y���̋�����- && ���݂̃X�s�[�h��-�ł͂Ȃ��ꍇ��-�����ɐi�܂���
		if (signbit(lDistance.y) && !signbit(speed))
		{
			speed *= -1.0f;
		}
		//Y���̋�����-�ł͂Ȃ� && ���݂̃X�s�[�h��-�̏ꍇ+�����ɐi�܂���
		else if (!signbit(lDistance.y) && signbit(speed))
		{
			speed *= -1.0f;
		}
		nowPos = &boxR.data.transform.pos.y;
		nextPos = &posArray[lineIndex + 1].y;
	}
	else if (0.1f <= abs(lDistance.z))
	{
		//Z���̋�����- && ���݂̃X�s�[�h��-�ł͂Ȃ��ꍇ��-�����ɐi�܂���
		if (signbit(lDistance.z) && !signbit(speed))
		{
			speed *= -1.0f;
		}
		//Z���̋�����-�ł͂Ȃ� && ���݂̃X�s�[�h��-�̏ꍇ+�����ɐi�܂���
		else if (!signbit(lDistance.z) && signbit(speed))
		{
			speed *= -1.0f;
		}
		nowPos = &boxR.data.transform.pos.z;
		nextPos = &posArray[lineIndex + 1].z;
	}
}
