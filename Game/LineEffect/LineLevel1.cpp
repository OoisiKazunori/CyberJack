#include "LineLevel1.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

LineLevel1::LineLevel1()
{
	initFlag = false;

	for (int i = 0; i < countVec.size(); ++i)
	{
		countVec[i] = 0;
	}
	allFinishFlag = false;
}

void LineLevel1::Attack(const XMVECTOR &PLAYER_POS, const XMVECTOR &ENEMY_POS, const EnemyMoveData &FLAG_DATA)
{
	if (!initFlag)
	{
		//1,2....プレイア－と敵との座標を算出する
		XMVECTOR adj = { 30.0f,30.0f,30.0f };
		XMVECTOR distance = (ENEMY_POS - PLAYER_POS) + adj;

		//3...敵との距離を考慮していくつ線を作るか決める
		const int randomMinNum = 3;
		int randomMaxNum = distance.m128_f32[2] / 10;
		//最大値が最小値を下回ないようにする
		if (randomMaxNum < randomMinNum)
		{
			randomMaxNum = randomMinNum;
		}
		int lineNum = KazMath::IntRand(randomMaxNum, randomMinNum);


		//4...線の数+1で制御点の数を決める.......<-仕様変更ポイント
		int limitNum = lineNum + 1;
		limitPos.resize(limitNum);


		//0...右,1...左,2...上,3...下,4...前
		const int moveVecMaxNum = 5;
		int moveVector = -1;
		//5...制御点の座標を算出する
		for (int i = 0; i < limitPos.size(); i++)
		{
			//最初の制御点はプレイヤー座標
			if (i == 0)
			{
				limitPos[i] = PLAYER_POS;
				continue;
			}
			//最後の制御点はエネミー座標
			if (i == limitPos.size() - 1)
			{
				limitPos[i] = ENEMY_POS;
				continue;
			}

			/*
			「中間の制御点の座標を決める際の条件」
			1.前の線とは同じ伸ばし方をしない
			2.前の制御点から見て直線状に制御点を配置する
			3.最後から一個前の制御点は必ず最後の制御点の位置の直線状に配置する
			*/
			//1.前の線とは同じ伸ばし方をしない
			moveVector = CalucurateDirection(moveVector, moveVecMaxNum);


			limitPos[i] = limitPos[i - 1];
			//3.最後から一個前の制御点は必ず最後の制御点の位置の直線状に配置する
			//最後から二個前の制御点Y軸だけ合わせる
			if (i == limitPos.size() - 3)
			{
				//高さがあっているかどうか確認,合っていないなら高さを合わせる
				if (limitPos[i].m128_f32[1] != ENEMY_POS.m128_f32[1])
				{
					limitPos[i].m128_f32[1] = ENEMY_POS.m128_f32[1];
				}
				continue;
			}
			//最後から一個前の制御点はX軸だけを合わせる
			if (i == limitPos.size() - 2)
			{
				//敵のX軸を合わせる
				limitPos[i].m128_f32[0] = ENEMY_POS.m128_f32[0];
				continue;
			}

			//2.前の制御点から見て直線状に制御点を配置する
			if (i < limitPos.size() - 2)
			{
				limitPos[i] += CalucurateMoveVector(moveVector, KazMath::FloatRand(50.0f, 60.0f));
			}
		}

		//制御点の描画クラス用意
		limitPolygon.resize(limitNum);
		for (int i = 0; i < limitPolygon.size(); i++)
		{
			limitPolygon[i].reset(new BoxPolygonRender);
		}

		line.resize(lineNum);
		for (int i = 0; i < line.size(); i++)
		{
			line[i].reset(new LineEffect);
			line[i]->RockOn(limitPos[i], limitPos[i + 1]);
		}

		//ここまでで制御点の配置は終わる
		initFlag = true;
	}

	for (int i = 0; i < limitPolygon.size(); i++)
	{
		limitPolygon[i]->data.transform.pos = limitPos[i];
		limitPolygon[i]->data.color = { 255.0f,255.0f,255.0f,255.0f };
	}
}

void LineLevel1::Attack2(const XMVECTOR &PLAYER_POS, const XMVECTOR &ENEMY_POS, const EnemyMoveData &FLAG_DATA)
{
	if (!initFlag)
	{
#pragma region ゴール座標算出

		//ゴール座標算出を開始----------------------------------------------------------------
		//1.どの面に刺すか決める
		eSurface surface //SURFACE_RIGHT;
			= static_cast<eSurface>(KazMath::IntRand(6, 0));

		//2.自分の座標を見て、刺す面は見えている位置かどうか確認する
		bool xMinusFlag = signbit(ENEMY_POS.m128_f32[0]);
		bool yMinusFlag = signbit(ENEMY_POS.m128_f32[1]);
		bool zMinusFlag = signbit(ENEMY_POS.m128_f32[2]);

		//どれがマイナスかでどの面が見えているかどうか分かる
		array<eSurface, 3> canLook;
		if (xMinusFlag)
		{
			canLook[0] = SURFACE_RIGHT;
		}
		else
		{
			canLook[0] = SURFACE_LEFT;
		}
		if (yMinusFlag)
		{
			canLook[1] = SURFACE_TOP;
		}
		else
		{
			canLook[1] = SURFACE_BUTTOM;
		}
		if (zMinusFlag)
		{
			canLook[2] = SURFACE_BACK;
		}
		else
		{
			canLook[2] = SURFACE_FRONT;
		}

		//見える面に刺すかどうか判断する
		bool canLookFlag = false;
		for (int i = 0; i < canLook.size(); i++)
		{
			if (canLook[i] == surface)
			{
				canLookFlag = true;
				break;
			}
		}

		XMVECTOR goalPos = ENEMY_POS;


		//使用しない
		if (false)
		{
			//ゴール座標から順に制御点にプッシュするための座標を記録する
			vector<XMVECTOR> endLimitPos;
			//true...プレイヤーから見えている面の場合,false...プレイヤーから見えていない面の場合
			if (canLookFlag)
			{
				//垂直に線を伸ばす...乱数で伸ばす
				XMVECTOR dir = firstDir(surface);
				goalPos += dir;
				endLimitPos.push_back(goalPos);
			}
			else
			{
				//垂直に線を伸ばす...乱数で伸ばす
				XMVECTOR dir = firstDir(surface);
				goalPos += dir;
				endLimitPos.push_back(goalPos);


				//刺す面から伸ばした方向を記録
				int oldMoveVector;
				switch (surface)
				{
				case SURFACE_NONE:
					break;
				case SURFACE_LEFT:
					oldMoveVector = LINE_MOVE_LEFT;
					break;
				case SURFACE_RIGHT:
					oldMoveVector = LINE_MOVE_RIGHT;
					break;
				case SURFACE_FRONT:
					oldMoveVector = LINE_MOVE_BACK;
					break;
				case SURFACE_BACK:
					oldMoveVector = LINE_MOVE_STRAIGHT;
					break;
				case SURFACE_TOP:
					oldMoveVector = LINE_MOVE_UP;
					break;
				case SURFACE_BUTTOM:
					oldMoveVector = LINE_MOVE_DOWN;
					break;
				default:
					break;
				}



				//条件に沿って線を伸ばしていく
				int moveVector = oldMoveVector;
				array<int, 5>limitCount;
				for (int i = 0; i < limitCount.size(); ++i)
				{
					limitCount[i] = 0;
				}

				int oldNum = -1;
				while (1)
				{
					//どの方向に線を伸ばすか指定
					moveVector = CalucurateDirection(moveVector, 5);
					//伸ばした方向をカウントする
					++limitCount[moveVector];

					//true...プレイヤーの方向に線を伸ばす,false...乱数で線を伸ばす
					if (2 <= limitCount[moveVector])
					{
						//XMVECTOR dir;
						////ゴール座標と敵との距離
						//XMVECTOR distance = ENEMY_POS - goalPos;
						////敵とプレイヤーの距離
						//XMVECTOR distance2 = ENEMY_POS - PLAYER_POS;

						///*
						//敵の座標より手前に座標を持ってくる
						//正し、プレイヤーより線を越えてはいけないのでプレイヤーと敵との距離分割り、その値を加える
						//*/
						//distance += (distance2 / 3);


						////どの方向に線を伸ばすか
						//int random = KazMath::IntRand(3, 0);
						//while (1)
						//{
						//	if (random != oldNum && distance.m128_f32[random] != 0)
						//	{
						//		break;
						//	}
						//	random = KazMath::IntRand(3, 0);
						//}
						//oldNum = random;

						//switch (random)
						//{
						//case LINE_MOVE_X:
						//	dir = { distance.m128_f32[0],0.0f,0.0f };
						//	break;
						//case LINE_MOVE_Y:
						//	dir = { 0.0f,distance.m128_f32[1],0.0f };
						//	break;
						//case LINE_MOVE_Z:
						//	dir = { 0.0f,0.0f,distance.m128_f32[2] };
						//	break;
						//default:
						//	break;
						//}

						////プレイヤーの方向に向かせる
						//if (xMinusFlag)
						//{
						//	dir.m128_f32[0] *= -1;
						//}
						//if (yMinusFlag)
						//{
						//	dir.m128_f32[1] *= -1;
						//}
						//if (zMinusFlag)
						//{
						//	dir.m128_f32[2] *= -1;
						//}
						////プレイヤーの方向に向かせる

						////線を伸ばす、その後記録
						//goalPos += dir;

						goalPos += CalucurateMoveVector(moveVector, 10);
					}
					else
					{
						//線を伸ばす、その後記録
						goalPos += CalucurateMoveVector(moveVector, 10);
					}

					//三つの条件いずれかが当てはまればループから抜け出す
					bool nearXFlag = abs(goalPos.m128_f32[0]) < abs(ENEMY_POS.m128_f32[0]);
					bool nearYFlag = abs(goalPos.m128_f32[1]) < abs(ENEMY_POS.m128_f32[1]);
					bool nearZFlag = abs(goalPos.m128_f32[2]) < abs(ENEMY_POS.m128_f32[2]);
					if (nearXFlag || nearYFlag || nearZFlag)
					{
						break;
					}
					else
					{
						endLimitPos.push_back(goalPos);
					}
				}
			}
		}
#pragma endregion

		goalPos = ENEMY_POS;
		//通常処理開始----------------------------------------------------------------
		//1.プレイヤーと敵の距離を算出する
		XMVECTOR distance = goalPos - PLAYER_POS;
		array<bool, 3> minusFlags;

		for (int i = 0; i < minusFlags.size(); ++i)
		{
			if (distance.m128_f32[i] < 0)
			{
				minusFlags[i] = true;
				distance.m128_f32[i] = (float)fabs(distance.m128_f32[i]);
			}
			else
			{
				minusFlags[i] = false;
			}
		}

		//(最初の制御点の位置はプレイヤー座標)
		limitPos.push_back(PLAYER_POS);


		//ゴールまでの線を伸ばす処理をループさせる
		int limitRandom = KazMath::IntRand(2, 0);
		eLineMove oldVec = LINE_MOVE_NONE;
		while (1)
		{
			//2.2軸以上、残りの距離が0になったら次の制御点を敵の座標にしてループを抜ける
			int count = 0;
			int dontUseVec = LINE_MOVE_NONE;
			for (int i = 0; i < 3; ++i)
			{
				if (distance.m128_f32[i] <= 0)
				{
					++count;
					dontUseVec = i;
				}
			}
			if (2 <= count)
			{
				limitPos.push_back(goalPos);
				initFlag = true;
				break;
			}

			//3.「XYZ軸どちらに伸ばすか」を乱数で算出
			eLineMove eVec = CaluRandom(dontUseVec, oldVec);
			oldVec = eVec;

			++countVec[eVec];


			if (countVec[eVec] <= limitRandom)
			{
				//3.「その軸の残り距離を割る数」を乱数で算出
				XMVECTOR dir = { 0.0f,0.0f,0.0f };
				dir.m128_f32[eVec] = distance.m128_f32[eVec] / KazMath::IntRand(3, 1);

				//4.伸ばす距離で残り距離を引く
				distance.m128_f32[eVec] -= dir.m128_f32[eVec];

				if (minusFlags[eVec])
				{
					dir.m128_f32[eVec] *= -1;
				}

				//4.一つ前の制御点の座標から足した距離を次の制御点の座標とする
				XMVECTOR tmp = limitPos[limitPos.size() - 1];
				limitPos.push_back(tmp + dir);
			}
			else
			{
				//5.同じ軸に一定回数以上割ったら、残り距離を0にする
				XMVECTOR tmp = limitPos[limitPos.size() - 1];
				XMVECTOR dir = { 0.0f, 0.0f, 0.0f };
				dir.m128_f32[eVec] = distance.m128_f32[eVec];
				distance.m128_f32[eVec] -= dir.m128_f32[eVec];
				if (minusFlags[eVec])
				{
					dir.m128_f32[eVec] *= -1;
				}
				limitPos.push_back(tmp + dir);
			}
		}


		//ゴール座標から敵までの座標を追加していく
		//for (int i = endLimitPos.size() - 1; 0 <= i; --i)
		//{
		//	limitPos.push_back(endLimitPos[i]);
		//}
		limitPos.push_back(ENEMY_POS);



		//制御点の描画クラス用意----------------------------------------------------------------
		limitPolygon.resize(limitPos.size());
		for (int i = 0; i < limitPolygon.size(); i++)
		{
			limitPolygon[i].reset(new BoxPolygonRender);
		}

		line.resize(limitPos.size() - 1);
		for (int i = 0; i < line.size(); i++)
		{
			line[i].reset(new LineEffect);
			line[i]->RockOn(limitPos[i], limitPos[i + 1]);
		}

		allFinishFlag = false;
	}

	for (int i = 0; i < limitPolygon.size(); i++)
	{
		limitPolygon[i]->data.transform.pos = limitPos[i];
		limitPolygon[i]->data.color = { 255.0f,255.0f,255.0f,255.0f };
	}


	line[0]->StartEffect();

}

void LineLevel1::ReleaseShot()
{
	if (line.size() != 0)
	{
		line[0]->ReleaseEffect();
	}
}

void LineLevel1::Release()
{
	for (int i = 0; i < line.size(); i++)
	{
		line[i].reset();
		limitPolygon[i].reset();
	}
	line.shrink_to_fit();
	line.clear();

	limitPolygon.shrink_to_fit();
	limitPolygon.clear();

	limitPos.shrink_to_fit();
	limitPos.clear();

	initFlag = false;


	for (int i = 0; i < countVec.size(); ++i)
	{
		countVec[i] = 0;
	}
}

void LineLevel1::Update()
{

	line[0]->Update();
	for (int i = 1; i < line.size(); ++i)
	{
		if (line[i - 1]->finishRockOnFlag)
		{
			line[i]->StartEffect();
		}
		if (line[i - 1]->finishReleaseFlag)
		{
			line[i]->ReleaseEffect();
		}


		line[i]->Update();
	}

	int count = 0;
	for (int i = 1; i < line.size(); ++i)
	{
		if (line[i]->finishReleaseFlag)
		{
			++count;
		}
	}


	//終了処理
	if (line.size() - 1 <= count)
	{
		for (int i = 0; i < line.size(); ++i)
		{
			line[i]->Finalize();
		}

		if (line[0]->FinishFlag())
		{
			initFlag = false;
			allFinishFlag = true;
		}
	}
}

void LineLevel1::Draw()
{
	for (int i = 0; i < limitPolygon.size(); i++)
	{
		//limitPolygon[i]->Draw();
	}

	for (int i = 0; i < line.size(); i++)
	{
		line[i]->Draw();
	}
}

XMVECTOR LineLevel1::CalucurateMoveVector(const int &RANDM_NUM, const float &LENGTH)
{
	XMVECTOR vel{};
	EnumLineMove moveType = static_cast<EnumLineMove>(RANDM_NUM);

	switch (moveType)
	{
	case LINE_MOVE_LEFT:
		vel = { -LENGTH,0.0f,0.0f };
		break;
	case LINE_MOVE_RIGHT:
		vel = { LENGTH,0.0f,0.0f };
		break;
	case LINE_MOVE_UP:
		vel = { 0.0f,LENGTH,0.0f };
		break;
	case LINE_MOVE_DOWN:
		vel = { 0.0f,-LENGTH,0.0f };
		break;
	case LINE_MOVE_STRAIGHT:
		vel = { 0.0f,0.0f,-LENGTH };
		break;
	case LINE_MOVE_BACK:
		vel = { 0.0f,0.0f,LENGTH };
		break;
	default:
		break;
	}

	return vel;
}

int LineLevel1::CalucurateDirection(const int &MOVE_VEC, const int &MOVE_VEC_MAX_NUM)
{
	int random = KazMath::IntRand(MOVE_VEC_MAX_NUM, 0);
	while (1)
	{
		//左右交互に制御点が置かれようとしたらフラグを立てる
		bool lineGoBackFlag =
			(random == LINE_MOVE_LEFT && MOVE_VEC == LINE_MOVE_RIGHT) ||
			(random == LINE_MOVE_RIGHT && MOVE_VEC == LINE_MOVE_LEFT);

		//上下交互に制御点が置かれようとしたらフラグを立てる
		bool lineGoUpFlag =
			(random == LINE_MOVE_UP && MOVE_VEC == LINE_MOVE_DOWN) ||
			(random == LINE_MOVE_DOWN && MOVE_VEC == LINE_MOVE_UP);

		//前後交互に制御点が置かれようとしたらフラグを立てる
		bool lineGoFrontFlag =
			(random == LINE_MOVE_STRAIGHT && MOVE_VEC == LINE_MOVE_BACK) ||
			(random == LINE_MOVE_BACK && MOVE_VEC == LINE_MOVE_STRAIGHT);

		//前の値と一緒だった場合
		bool lineSameFlag = (random == MOVE_VEC);

		//true...乱数を入れ直し,false...被りなしを確認、ループを抜ける
		if (lineGoBackFlag || lineGoUpFlag || lineSameFlag || lineGoFrontFlag)
		{
			random = KazMath::IntRand(MOVE_VEC_MAX_NUM, 0);
		}
		else
		{
			break;
		}
	}
	return random;
}

int LineLevel1::CalucurateDirection2(const int &MOVE_VEC, const int &MOVE_VEC_MAX_NUM)
{
	int random = KazMath::IntRand(MOVE_VEC_MAX_NUM, 0);
	while (true)
	{
		//左右交互に制御点が置かれようとしたらフラグを立てる
		bool lineGoBackFlag =
			random == LINE_MOVE_LEFT && MOVE_VEC == LINE_MOVE_RIGHT ||
			random == LINE_MOVE_RIGHT && MOVE_VEC == LINE_MOVE_LEFT;

		//上下交互に制御点が置かれようとしたらフラグを立てる
		bool lineGoUpFlag =
			random == LINE_MOVE_UP && MOVE_VEC == LINE_MOVE_DOWN ||
			random == LINE_MOVE_DOWN && MOVE_VEC == LINE_MOVE_UP;

		//前の値と一緒だった場合
		bool lineSameFlag = (random == MOVE_VEC);

		//true...乱数を入れ直し,false...被りなしを確認、ループを抜ける
		if (lineGoBackFlag || lineGoUpFlag || lineSameFlag)
		{
			random = KazMath::IntRand(MOVE_VEC_MAX_NUM, 0);
		}
		else
		{
			break;
		}
	}
	return random;
}


eLineMove LineLevel1::CaluRandom(int DONT_USE, int OLD_NUM)
{
	eLineMove eVec;
	while (1)
	{
		int random = KazMath::IntRand(LINE_MOVE_Z + 1, LINE_MOVE_X);
		if (DONT_USE != random && OLD_NUM != random)
		{
			eVec = static_cast<eLineMove>(random);
			break;
		}
	}

	return eVec;
}

XMVECTOR LineLevel1::firstDir(eSurface SURFACE)
{
	XMVECTOR dir = { 0.0f,0.0f,0.0f };
	float vec = KazMath::FloatRand(30.0f, 10.0f);
	switch (SURFACE)
	{
	case SURFACE_NONE:
		break;
	case SURFACE_LEFT:
		dir += XMVectorSet(-vec, 0.0f, 0.0f, 0.0f);
		break;
	case SURFACE_RIGHT:
		dir += XMVectorSet(vec, 0.0f, 0.0f, 0.0f);
		break;
	case SURFACE_FRONT:
		dir += XMVectorSet(0.0f, 0.0f, -vec, 0.0f);
		break;
	case SURFACE_BACK:
		dir += XMVectorSet(0.0f, 0.0f, vec, 0.0f);
		break;
	case SURFACE_TOP:
		dir += XMVectorSet(0.0f, vec, 0.0f, 0.0f);
		break;
	case SURFACE_BUTTOM:
		dir += XMVectorSet(0.0f, -vec, 0.0f, 0.0f);
		break;
	default:
		break;
	}

	return dir;
}
