#include "LineFlashLight.h"
#include"../KazLibrary/Helper/KazHelper.h"

LineFlashLight::LineFlashLight()
{
}

void LineFlashLight::Init(const std::vector<KazMath::Vec3<float>> &POS_ARRAY, int TIMER)
{
	posArray = POS_ARRAY;

	//全長の算出
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
	//1Flameのスピード
	speed = distance / static_cast<float>(TIMER);

	lineIndex = 0;
	boxR.data.transform.pos = posArray[lineIndex];

	finishFlag = false;
}

void LineFlashLight::Update()
{
	//ボックスをどの方向に向かわせるか決める
	CheckWhereToGo();

	//ボックスを動かす
	if (nowPos != nullptr)
	{
		*nowPos += speed;
	}

	//線を超えたら次の線に行く
	if (nowPos != nullptr && nextPos != nullptr && *nextPos - *nowPos <= 0.0f)
	{
		++lineIndex;

		int lLoopCount = 0;
		float lDistance = 0.0f;
		//一度に線を越えた回数が100回以下&&線を越えなかった時ループを終える
		while (lLoopCount <= 100 && 0.0f <= lDistance)
		{
			//線を越えた判定
			lDistance = *nextPos - *nowPos;
			if (lDistance <= -0.1f)
			{
				boxR.data.transform.pos = posArray[lineIndex];
				//最後の制御点なら終了する
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
		//X軸の距離が- && 現在のスピードが-ではない場合に-方向に進ませる
		if (signbit(lDistance.x) && !signbit(speed))
		{
			speed *= -1.0f;
		}
		//X軸の距離が-ではない && 現在のスピードが-の場合+方向に進ませる
		else if (!signbit(lDistance.x) && signbit(speed))
		{
			speed *= -1.0f;
		}
		nowPos = &boxR.data.transform.pos.x;
		nextPos = &posArray[lineIndex + 1].x;
	}
	else if (0.1f <= abs(lDistance.y))
	{
		//Y軸の距離が- && 現在のスピードが-ではない場合に-方向に進ませる
		if (signbit(lDistance.y) && !signbit(speed))
		{
			speed *= -1.0f;
		}
		//Y軸の距離が-ではない && 現在のスピードが-の場合+方向に進ませる
		else if (!signbit(lDistance.y) && signbit(speed))
		{
			speed *= -1.0f;
		}
		nowPos = &boxR.data.transform.pos.y;
		nextPos = &posArray[lineIndex + 1].y;
	}
	else if (0.1f <= abs(lDistance.z))
	{
		//Z軸の距離が- && 現在のスピードが-ではない場合に-方向に進ませる
		if (signbit(lDistance.z) && !signbit(speed))
		{
			speed *= -1.0f;
		}
		//Z軸の距離が-ではない && 現在のスピードが-の場合+方向に進ませる
		else if (!signbit(lDistance.z) && signbit(speed))
		{
			speed *= -1.0f;
		}
		nowPos = &boxR.data.transform.pos.z;
		nextPos = &posArray[lineIndex + 1].z;
	}
}
