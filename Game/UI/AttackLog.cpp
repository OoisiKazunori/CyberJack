#include "AttackLog.h"

AttackLog::AttackLog()
{
}

void AttackLog::Init()
{
	logArrayIndex = 0;
	logDataIndex = 0;
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		logStringArray[i].logIndex = -1;
	}
}

void AttackLog::Update()
{
	//���Ԍo�߂Ń��O������
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		if (logStringArray[i].log.TimeOver() && 0 <= logStringArray[i].logIndex)
		{
			logStringArray[i].logIndex = -1;
			for (int logIndex = 0; logIndex < logStringArray.size(); ++logIndex)
			{
				--logStringArray[logIndex].logIndex;
			}
			--logDataIndex;
		}
	}

	//���O�̔p��
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		if (logStringArray[i].logIndex <= -1)
		{
			logStringArray[i].log.Finalize();
		}
	}




	for (int i = 0; i < logStringArray.size(); ++i)
	{
		if (0 <= logStringArray[i].logIndex)
		{
			logStringArray[i].log.Update(logStringArray[i].logIndex);
		}
	}
}

void AttackLog::Draw()
{
	for (int i = 0; i < logStringArray.size(); ++i)
	{
		logStringArray[i].log.Draw();
	}
}

void AttackLog::WriteLog(const std::string &STRING, float FONT_SIZE)
{
	logStringArray[logArrayIndex].log.Init(KazMath::Vec2<float>(10.0f, 300.0f), STRING, FONT_SIZE);
	logStringArray[logArrayIndex].logIndex = logDataIndex;

	//���O�̏��Ԃ��ő�l�܂œ��B�����炻�̍ő�l�̂܂܂ɂ���
	++logDataIndex;
	if (logStringArray.size() <= logDataIndex)
	{
		logDataIndex = static_cast<int>(logStringArray.size() - 1);
		//�ő�l�܂ōs������S�Ẵ��O�̃C���f�b�N�X��������A�����̍��W�����炷
		for (int i = 0; i < logStringArray.size(); ++i)
		{
			--logStringArray[i].logIndex;
		}
	}

	//���O�z��̓Y�������ő�l�܂œ��B������ŏ��̃��O���g����
	++logArrayIndex;
	if (logStringArray.size() <= logArrayIndex)
	{
		logArrayIndex = 0;
	}
}
