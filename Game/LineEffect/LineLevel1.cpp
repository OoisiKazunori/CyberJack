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

void LineLevel1::CalucurateDistance(const XMVECTOR &PLAYER_POS, const XMVECTOR &ENEMY_POS)
{
	XMVECTOR distance(ENEMY_POS - PLAYER_POS);

	distanceValue = distance / rockOnDistance;


	for (int i = 0; i < 3; ++i)
	{
		if (isnan<float>(distanceValue.m128_f32[i]) || isinf<float>(distanceValue.m128_f32[i]))
		{
			distanceValue.m128_f32[i] = 0.0f;
		}
	}

}

void LineLevel1::Attack(const XMVECTOR &PLAYER_POS, const XMVECTOR &ENEMY_POS, const EnemyMoveData &FLAG_DATA)
{
	if (!initFlag)
	{
		//1,2....�v���C�A�|�ƓG�Ƃ̍��W���Z�o����
		XMVECTOR adj = { 30.0f,30.0f,30.0f };
		XMVECTOR distance = (ENEMY_POS - PLAYER_POS) + adj;

		//3...�G�Ƃ̋������l�����Ă���������邩���߂�
		const int randomMinNum = 3;
		int randomMaxNum = distance.m128_f32[2] / 10;
		//�ő�l���ŏ��l������Ȃ��悤�ɂ���
		if (randomMaxNum < randomMinNum)
		{
			randomMaxNum = randomMinNum;
		}
		int lineNum = KazMath::IntRand(randomMaxNum, randomMinNum);


		//4...���̐�+1�Ő���_�̐������߂�.......<-�d�l�ύX�|�C���g
		int limitNum = lineNum + 1;
		limitPos.resize(limitNum);


		//0...�E,1...��,2...��,3...��,4...�O
		const int moveVecMaxNum = 5;
		int moveVector = -1;
		//5...����_�̍��W���Z�o����
		for (int i = 0; i < limitPos.size(); i++)
		{
			//�ŏ��̐���_�̓v���C���[���W
			if (i == 0)
			{
				limitPos[i] = PLAYER_POS;
				continue;
			}
			//�Ō�̐���_�̓G�l�~�[���W
			if (i == limitPos.size() - 1)
			{
				limitPos[i] = ENEMY_POS;
				continue;
			}

			/*
			�u���Ԃ̐���_�̍��W�����߂�ۂ̏����v
			1.�O�̐��Ƃ͓����L�΂��������Ȃ�
			2.�O�̐���_���猩�Ē�����ɐ���_��z�u����
			3.�Ōォ���O�̐���_�͕K���Ō�̐���_�̈ʒu�̒�����ɔz�u����
			*/
			//1.�O�̐��Ƃ͓����L�΂��������Ȃ�
			moveVector = CalucurateDirection(moveVector, moveVecMaxNum);


			limitPos[i] = limitPos[i - 1];
			//3.�Ōォ���O�̐���_�͕K���Ō�̐���_�̈ʒu�̒�����ɔz�u����
			//�Ōォ���O�̐���_Y���������킹��
			if (i == limitPos.size() - 3)
			{
				//�����������Ă��邩�ǂ����m�F,�����Ă��Ȃ��Ȃ獂�������킹��
				if (limitPos[i].m128_f32[1] != ENEMY_POS.m128_f32[1])
				{
					limitPos[i].m128_f32[1] = ENEMY_POS.m128_f32[1];
				}
				continue;
			}
			//�Ōォ���O�̐���_��X�����������킹��
			if (i == limitPos.size() - 2)
			{
				//�G��X�������킹��
				limitPos[i].m128_f32[0] = ENEMY_POS.m128_f32[0];
				continue;
			}

			//2.�O�̐���_���猩�Ē�����ɐ���_��z�u����
			if (i < limitPos.size() - 2)
			{
				limitPos[i] += CalucurateMoveVector(moveVector, KazMath::FloatRand(50.0f, 60.0f));
			}
		}

		//����_�̕`��N���X�p��
		limitPolygon.resize(limitNum);
		for (int i = 0; i < limitPolygon.size(); i++)
		{
			limitPolygon[i].reset(new BoxPolygonRender);
		}

		line.resize(lineNum);
		for (int i = 0; i < line.size(); i++)
		{
			line[i].reset(new LineEffect);
			//line[i]->RockOn(limitPos[i], limitPos[i + 1]);
		}

		//�����܂łŐ���_�̔z�u�͏I���
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
#pragma region �S�[�����W�Z�o

		//�S�[�����W�Z�o���J�n----------------------------------------------------------------
		//1.�ǂ̖ʂɎh�������߂�
		eSurface surface //SURFACE_RIGHT;
			= static_cast<eSurface>(KazMath::IntRand(6, 0));

		//2.�����̍��W�����āA�h���ʂ͌����Ă���ʒu���ǂ����m�F����
		bool xMinusFlag = signbit(ENEMY_POS.m128_f32[0]);
		bool yMinusFlag = signbit(ENEMY_POS.m128_f32[1]);
		bool zMinusFlag = signbit(ENEMY_POS.m128_f32[2]);
		//���������ɂ��邩�ǂ���
		bool yEqualFlag = ENEMY_POS.m128_f32[1] <= 1.0f && -1.0f <= ENEMY_POS.m128_f32[1];
		//�^���ʂ��ǂ���
		bool frontFlag = ENEMY_POS.m128_f32[0] == PLAYER_POS.m128_f32[0];

		//�ǂꂪ�}�C�i�X���łǂ̖ʂ������Ă��邩�ǂ���������
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

		//�^���ʂɓG������ꍇ�͐��ʂ��������Ȃ�
		if (frontFlag)
		{
			canLook[0] = SURFACE_NONE;
			canLook[1] = SURFACE_NONE;
			canLook[2] = SURFACE_FRONT;
		}
		//�����������Ȃ�㉺�ƌ��ʂ͌����Ȃ�
		if (yEqualFlag)
		{
			canLook[1] = SURFACE_NONE;
			canLook[2] = SURFACE_FRONT;
		}



		//������ʂɎh�����ǂ������f����
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

		//�S�[�����W���珇�ɐ���_�Ƀv�b�V�����邽�߂̍��W���L�^����
		vector<XMVECTOR> endLimitPos;
		if (true)
		{
			//true...�v���C���[���猩���Ă���ʂ̏ꍇ,false...�v���C���[���猩���Ă��Ȃ��ʂ̏ꍇ
			if (canLookFlag)
			{
				//�����ɐ���L�΂�...�����ŐL�΂�
				XMVECTOR dir = firstDir(surface);
				goalPos += dir;
				endLimitPos.push_back(goalPos);
			}
			else
			{
				//�����ɐ���L�΂�...�����ŐL�΂�
				XMVECTOR dir = firstDir(surface);
				goalPos += dir;
				endLimitPos.push_back(goalPos);


				//�h���ʂ���L�΂����������L�^
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


#pragma region ��蓹���̏���
				//�����ɉ����Đ���L�΂��Ă���
				int moveVector = oldMoveVector;
				//�L�т������̉񐔂̏�����
				array<int, 5>limitCount;
				for (int i = 0; i < limitCount.size(); ++i)
				{
					limitCount[i] = 0;
				}
				int oldNum = -1;

				//��蓹���̏����J�n
				while (1)
				{
					//�ǂ̕����ɐ���L�΂����w��
					moveVector = CalucurateDirection(moveVector, 5);

					bool yUpLimitFlag = (LINE_MOVE_UP == moveVector && 1 <= limitCount[moveVector]);
					bool yDownLimitFlag = (LINE_MOVE_DOWN == moveVector && 1 <= limitCount[moveVector]);
					//�ォ����2��ȏ�L�΂����Ƃ����������x�������o��
					if (yUpLimitFlag || yDownLimitFlag)
					{
						continue;
					}

					//�L�΂����������J�E���g����
					++limitCount[moveVector];


					//�������ꂽ�񐔈ȏ�ɐ����L�т悤�Ƃ�����true�ɂȂ�
					//true...�v���C���[�̕����ɐ���L�΂�,false...�����Ő���L�΂�
					if (2 <= limitCount[moveVector])
					{
						//XMVECTOR dir;
						//�]�蕪�̋���������
						XMVECTOR addDistance = { 10.0f,10.0f,10.0f };


						//�S�[�����W�ƓG�Ƃ̋���
						XMVECTOR distance = ENEMY_POS - goalPos;
						//�G�ƃv���C���[�̋���
						XMVECTOR distance2 = ENEMY_POS - PLAYER_POS;


						//+����-�����𔻒f�������-----------------------
						//�G�ƃS�[�����W
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

						//�v���C���[�ƓG���W
						array<bool, 3> minusFlags2;
						for (int i = 0; i < minusFlags2.size(); ++i)
						{
							if (distance2.m128_f32[i] < 0)
							{
								minusFlags2[i] = true;
								distance2.m128_f32[i] = (float)fabs(distance2.m128_f32[i]);
							}
							else
							{
								minusFlags2[i] = false;
							}
						}
						//+����-�����𔻒f�������-----------------------

						//�L�΂�����
						XMVECTOR vec = {};
						/*
						�G�̍��W����O�ɍ��W�������Ă���
						�����A�v���C���[�������z���Ă͂����Ȃ��̂Ŕ�r����
						*/
						//true...�G�Ƃ̋����̕����v���C���[�Ƃ̋����̕����Z��
						for (int axis = 0; axis < 3; ++axis)
						{
							if (distance2.m128_f32[axis] <= distance.m128_f32[axis] + addDistance.m128_f32[axis])
							{
								//�v���C���[�ƓG�Ƃ̋��������G���O�̍��W��z�u����
								//�ǂꂭ�炢�����Ă��邩�m�F
								XMVECTOR tmp;
								tmp.m128_f32[axis] = distance2.m128_f32[axis] - distance.m128_f32[axis];
								tmp.m128_f32[axis] = (float)fabs(tmp.m128_f32[axis]);

								//�G�ƃS�[�����W�Ƃ̋����ɒ���������������
								//tmp�̎���0�o�Ȃ�����A����������������
								if (0.1f <= tmp.m128_f32[axis])
								{
									vec.m128_f32[axis] = distance.m128_f32[axis] - tmp.m128_f32[axis];
								}
							}
							else
							{
								//�z���Ȃ���΂��̂܂܎g��
								vec = distance + addDistance;
							}
						}

						//�ǂ̕����ɐ���L�΂���-----------------------
						float dirVec = 0.0f;
						switch (moveVector)
						{
						case LINE_MOVE_LEFT:
							dirVec = vec.m128_f32[0];
							break;
						case LINE_MOVE_RIGHT:
							dirVec = vec.m128_f32[0];
							break;
						case LINE_MOVE_UP:
							dirVec = vec.m128_f32[1];
							break;
						case LINE_MOVE_DOWN:
							dirVec = vec.m128_f32[1];
							break;
						case LINE_MOVE_STRAIGHT:
							dirVec = vec.m128_f32[2];
							break;
						case LINE_MOVE_BACK:
							dirVec = vec.m128_f32[2];
							break;
						default:
							break;
						}

						//�v���C���[�̕����Ɍ�������
						if (xMinusFlag)
						{
							dirVec *= -1;
						}
						if (yMinusFlag)
						{
							dirVec *= -1;
						}
						if (zMinusFlag)
						{
							dirVec *= -1;
						}
						//�v���C���[�̕����Ɍ�������

						goalPos += CalucurateMoveVector(moveVector, dirVec);
						//�ǂ̕����ɐ���L�΂���-----------------------


					
					}
					else
					{
						//����L�΂��A���̌�L�^
						goalPos += CalucurateMoveVector(moveVector, 10);
					}

					//�O�̏��������ꂩ�����Ă͂܂�΃��[�v���甲���o��
					bool nearXFlag = abs(goalPos.m128_f32[0]) < abs(ENEMY_POS.m128_f32[0]);
					bool nearYFlag = abs(goalPos.m128_f32[1]) < abs(ENEMY_POS.m128_f32[1]);
					bool nearZFlag = abs(goalPos.m128_f32[2]) < abs(ENEMY_POS.m128_f32[2]);
					if (nearXFlag || nearZFlag || nearYFlag)
					{
						break;
					}
					else
					{
						endLimitPos.push_back(goalPos);
					}
				}
#pragma endregion
			}
		}
#pragma endregion

		//goalPos = ENEMY_POS;
		//�ʏ폈���J�n----------------------------------------------------------------
		//1.�v���C���[�ƓG�̋������Z�o����
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

		//(�ŏ��̐���_�̈ʒu�̓v���C���[���W)
		limitPos.push_back(PLAYER_POS);


		//�S�[���܂ł̐���L�΂����������[�v������
		int limitRandom = KazMath::IntRand(2, 0);
		eLineMove oldVec = LINE_MOVE_NONE;
		while (1)
		{
			//2.2���ȏ�A�c��̋�����0�ɂȂ����玟�̐���_��G�̍��W�ɂ��ă��[�v�𔲂���
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

			//3.�uXYZ���ǂ���ɐL�΂����v�𗐐��ŎZ�o
			eLineMove eVec = CaluRandom(dontUseVec, oldVec);
			oldVec = eVec;

			++countVec[eVec];


			if (countVec[eVec] <= limitRandom)
			{
				//3.�u���̎��̎c�苗�������鐔�v�𗐐��ŎZ�o
				XMVECTOR dir = { 0.0f,0.0f,0.0f };
				dir.m128_f32[eVec] = distance.m128_f32[eVec] / KazMath::IntRand(3, 1);

				//4.�L�΂������Ŏc�苗��������
				distance.m128_f32[eVec] -= dir.m128_f32[eVec];

				if (minusFlags[eVec])
				{
					dir.m128_f32[eVec] *= -1;
				}

				//4.��O�̐���_�̍��W���瑫�������������̐���_�̍��W�Ƃ���
				XMVECTOR tmp = limitPos[limitPos.size() - 1];
				limitPos.push_back(tmp + dir);
			}
			else
			{
				//5.�������Ɉ��񐔈ȏ㊄������A�c�苗����0�ɂ���
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


		//�S�[�����W����G�܂ł̍��W��ǉ����Ă���
		for (int i = endLimitPos.size() - 1; 0 <= i; --i)
		{
			limitPos.push_back(endLimitPos[i]);
		}
		limitPos.push_back(ENEMY_POS);



		//����_�̕`��N���X�p��----------------------------------------------------------------
		limitPolygon.resize(limitPos.size());
		for (int i = 0; i < limitPolygon.size(); i++)
		{
			limitPolygon[i].reset(new BoxPolygonRender);
		}

		line.resize(limitPos.size() - 1);
		for (int i = 0; i < line.size(); i++)
		{
			line[i].reset(new LineEffect);


			XMVECTOR startPlayerdistance = limitPos[i] - PLAYER_POS;
			XMVECTOR endPlayerdistance = limitPos[i + 1] - PLAYER_POS;

			line[i]->RockOn(limitPos[i], limitPos[i + 1], startPlayerdistance, endPlayerdistance);
		}



		for (int i = 0; i < limitPolygon.size(); i++)
		{
			limitPolygon[i]->data.transform.pos = limitPos[i];
			limitPolygon[i]->data.transform.scale = { 0.5f,2.0f,0.5f };
			limitPolygon[i]->data.color = { 255.0f,0.0f,0.0f,255.0f };
		}

		int count = endLimitPos.size();
		for (int i = limitPolygon.size() - 1; 0 <= i; --i)
		{
			if (count <= 0)
			{
				break;
			}
			limitPolygon[i]->data.transform.scale = { 2.0f,0.5f,0.5f };
			limitPolygon[i]->data.color = { 0.0f,255.0f,0.0f,255.0f };
			--count;
		}



		rockOnDistance = ENEMY_POS - PLAYER_POS;

		allFinishFlag = false;
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
	if (initFlag)
	{
		//���b�N�I�����̋���-----------------------
		//�G�ƃv���C���[�̋����@/ ���b�N�I�����̋����@�Ŋ��������߂�
		for (int i = 0; i < line.size(); ++i)
		{
			line[i]->playerPos = playerPos;
			line[i]->MoveLine(distanceValue);
		}
		//���b�N�I�����̋���-----------------------


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


		//�I������
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
}

void LineLevel1::Draw()
{
	if (initFlag)
	{
		for (int i = 0; i < limitPolygon.size(); i++)
		{
			limitPolygon[i]->Draw();
		}

		for (int i = 0; i < line.size(); i++)
		{
			line[i]->Draw();
		}
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
		//���E���݂ɐ���_���u����悤�Ƃ�����t���O�𗧂Ă�
		bool lineGoBackFlag =
			(random == LINE_MOVE_LEFT && MOVE_VEC == LINE_MOVE_RIGHT) ||
			(random == LINE_MOVE_RIGHT && MOVE_VEC == LINE_MOVE_LEFT);

		//�㉺���݂ɐ���_���u����悤�Ƃ�����t���O�𗧂Ă�
		bool lineGoUpFlag =
			(random == LINE_MOVE_UP && MOVE_VEC == LINE_MOVE_DOWN) ||
			(random == LINE_MOVE_DOWN && MOVE_VEC == LINE_MOVE_UP);

		//�O����݂ɐ���_���u����悤�Ƃ�����t���O�𗧂Ă�
		bool lineGoFrontFlag =
			(random == LINE_MOVE_STRAIGHT && MOVE_VEC == LINE_MOVE_BACK) ||
			(random == LINE_MOVE_BACK && MOVE_VEC == LINE_MOVE_STRAIGHT);

		//�O�̒l�ƈꏏ�������ꍇ
		bool lineSameFlag = (random == MOVE_VEC);

		//true...��������꒼��,false...���Ȃ����m�F�A���[�v�𔲂���
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
		//���E���݂ɐ���_���u����悤�Ƃ�����t���O�𗧂Ă�
		bool lineGoBackFlag =
			random == LINE_MOVE_LEFT && MOVE_VEC == LINE_MOVE_RIGHT ||
			random == LINE_MOVE_RIGHT && MOVE_VEC == LINE_MOVE_LEFT;

		//�㉺���݂ɐ���_���u����悤�Ƃ�����t���O�𗧂Ă�
		bool lineGoUpFlag =
			random == LINE_MOVE_UP && MOVE_VEC == LINE_MOVE_DOWN ||
			random == LINE_MOVE_DOWN && MOVE_VEC == LINE_MOVE_UP;

		//�O�̒l�ƈꏏ�������ꍇ
		bool lineSameFlag = (random == MOVE_VEC);

		//true...��������꒼��,false...���Ȃ����m�F�A���[�v�𔲂���
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
