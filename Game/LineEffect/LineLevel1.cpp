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

void LineLevel1::CalucurateDistance(const KazMath::Vec3<float> &PLAYER_POS, const KazMath::Vec3<float> &ENEMY_POS)
{
	KazMath::Vec3<float> distance(ENEMY_POS - PLAYER_POS);

	distanceValue = distance / rockOnDistance;


	if (isnan<float>(distanceValue.x) || isinf<float>(distanceValue.x))
	{
		distanceValue.x = 0.0f;
	}
	if (isnan<float>(distanceValue.y) || isinf<float>(distanceValue.y))
	{
		distanceValue.y = 0.0f;
	}
	if (isnan<float>(distanceValue.z) || isinf<float>(distanceValue.z))
	{
		distanceValue.z = 0.0f;
	}
}

void LineLevel1::Attack(const KazMath::Vec3<float> &PLAYER_POS, const KazMath::Vec3<float> &ENEMY_POS, const EnemyMoveData &FLAG_DATA)
{
	if (!initFlag)
	{
		//1,2....�v���C�A�|�ƓG�Ƃ̍��W���Z�o����
		KazMath::Vec3<float> adj = { 30.0f,30.0f,30.0f };
		KazMath::Vec3<float> distance = (ENEMY_POS - PLAYER_POS) + adj;

		//3...�G�Ƃ̋������l�����Ă���������邩���߂�
		const int randomMinNum = 3;
		int randomMaxNum = distance.z / 10;
		//�ő�l���ŏ��l������Ȃ��悤�ɂ���
		if (randomMaxNum < randomMinNum)
		{
			randomMaxNum = randomMinNum;
		}
		int lineNum = KazMath::Rand<int>(randomMaxNum, randomMinNum);


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
				if (limitPos[i].y != ENEMY_POS.y)
				{
					limitPos[i].y = ENEMY_POS.y;
				}
				continue;
			}
			//�Ōォ���O�̐���_��X�����������킹��
			if (i == limitPos.size() - 2)
			{
				//�G��X�������킹��
				limitPos[i].x = ENEMY_POS.x;
				continue;
			}

			//2.�O�̐���_���猩�Ē�����ɐ���_��z�u����
			if (i < limitPos.size() - 2)
			{
				limitPos[i] += CalucurateMoveVector(moveVector, KazMath::Rand<float>(50.0f, 60.0f));
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

void LineLevel1::Attack2(const KazMath::Vec3<float> &PLAYER_POS, const KazMath::Vec3<float> &ENEMY_POS, const EnemyMoveData &FLAG_DATA)
{
	if (!initFlag)
	{
		limitPos.clear();
		line.clear();
		limitPolygon.clear();

#pragma region �S�[�����W�Z�o

		//�S�[�����W�Z�o���J�n----------------------------------------------------------------
		//1.�ǂ̖ʂɎh�������߂�
		eSurface surface //SURFACE_RIGHT;
			= static_cast<eSurface>(KazMath::Rand<int>(6, 0));

		//2.�����̍��W�����āA�h���ʂ͌����Ă���ʒu���ǂ����m�F����
		bool xMinusFlag = signbit(ENEMY_POS.x);
		bool yMinusFlag = signbit(ENEMY_POS.y);
		bool zMinusFlag = signbit(ENEMY_POS.z);
		//���������ɂ��邩�ǂ���
		bool yEqualFlag = ENEMY_POS.y <= 1.0f && -1.0f <= ENEMY_POS.y;
		//�^���ʂ��ǂ���
		bool frontFlag = ENEMY_POS.x == PLAYER_POS.x;

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

		KazMath::Vec3<float> goalPos = ENEMY_POS;

		//�S�[�����W���珇�ɐ���_�Ƀv�b�V�����邽�߂̍��W���L�^����
		std::vector<KazMath::Vec3<float>> endLimitPos;
		if (true)
		{
			//true...�v���C���[���猩���Ă���ʂ̏ꍇ,false...�v���C���[���猩���Ă��Ȃ��ʂ̏ꍇ
			if (canLookFlag)
			{
				//�����ɐ���L�΂�...�����ŐL�΂�
				KazMath::Vec3<float> dir = firstDir(surface);
				goalPos += dir;
				endLimitPos.push_back(goalPos);
			}
			else
			{
				//�����ɐ���L�΂�...�����ŐL�΂�
				KazMath::Vec3<float> dir = firstDir(surface);
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
						//KazMath::Vec3<float> dir;
						//�]�蕪�̋���������
						KazMath::Vec3<float> addDistance = { 10.0f,10.0f,10.0f };


						//�S�[�����W�ƓG�Ƃ̋���
						KazMath::Vec3<float> distance = ENEMY_POS - goalPos;
						//�G�ƃv���C���[�̋���
						KazMath::Vec3<float> distance2 = ENEMY_POS - PLAYER_POS;


						//+����-�����𔻒f�������-----------------------
						//�G�ƃS�[�����W
						array<bool, 3> minusFlags;
						minusFlags[0] = IsMinus(distance.x);
						if (minusFlags[0])
						{
							distance.x = fabs(distance.x);
						}
						minusFlags[1] = IsMinus(distance.y);
						if (minusFlags[1])
						{
							distance.y = fabs(distance.y);
						}
						minusFlags[2] = IsMinus(distance.z);
						if (minusFlags[2])
						{
							distance.z = fabs(distance.z);
						}


						//�v���C���[�ƓG���W
						array<bool, 3> minusFlags2;
						minusFlags2[0] = IsMinus(distance2.x);
						if (minusFlags2[0])
						{
							distance2.x = fabs(distance2.x);
						}
						minusFlags2[1] = IsMinus(distance2.y);
						if (minusFlags2[1])
						{
							distance2.y = fabs(distance2.y);
						}
						minusFlags2[2] = IsMinus(distance.z);
						if (minusFlags2[2])
						{
							distance2.z = fabs(distance2.z);
						}
						//+����-�����𔻒f�������-----------------------

						//�L�΂�����
						KazMath::Vec3<float> vec = {};
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
								KazMath::Vec3<float> tmp;
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
							dirVec = vec.x;
							break;
						case LINE_MOVE_RIGHT:
							dirVec = vec.x;
							break;
						case LINE_MOVE_UP:
							dirVec = vec.y;
							break;
						case LINE_MOVE_DOWN:
							dirVec = vec.y;
							break;
						case LINE_MOVE_STRAIGHT:
							dirVec = vec.z;
							break;
						case LINE_MOVE_BACK:
							dirVec = vec.z;
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
					bool nearXFlag = abs(goalPos.x) < abs(ENEMY_POS.x);
					bool nearYFlag = abs(goalPos.y) < abs(ENEMY_POS.y);
					bool nearZFlag = abs(goalPos.z) < abs(ENEMY_POS.z);
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
		KazMath::Vec3<float> distance = goalPos - PLAYER_POS;
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
		int limitRandom = KazMath::Rand<int>(2, 0);
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
				KazMath::Vec3<float> dir = { 0.0f,0.0f,0.0f };
				dir.m128_f32[eVec] = distance.m128_f32[eVec] / KazMath::Rand<int>(3, 1);

				//4.�L�΂������Ŏc�苗��������
				distance.m128_f32[eVec] -= dir.m128_f32[eVec];

				if (minusFlags[eVec])
				{
					dir.m128_f32[eVec] *= -1;
				}

				//4.��O�̐���_�̍��W���瑫�������������̐���_�̍��W�Ƃ���
				KazMath::Vec3<float> tmp = limitPos[limitPos.size() - 1];
				limitPos.push_back(tmp + dir);
			}
			else
			{
				//5.�������Ɉ��񐔈ȏ㊄������A�c�苗����0�ɂ���
				KazMath::Vec3<float> tmp = limitPos[limitPos.size() - 1];
				KazMath::Vec3<float> dir = { 0.0f, 0.0f, 0.0f };
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


			KazMath::Vec3<float> startPlayerdistance = limitPos[i] - PLAYER_POS;
			KazMath::Vec3<float> endPlayerdistance = limitPos[i + 1] - PLAYER_POS;

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



	lineReachObjFlag = false;
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


		//�����[�X���̉��o���ڕW�ɂ��ǂ蒅������t���O�𗧂Ă�
		if (line[line.size() - 1]->finishReleaseFlag)
		{
			lineReachObjFlag = true;
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
			//limitPolygon[i]->Draw();
		}

		for (int i = 0; i < line.size(); i++)
		{
			line[i]->Draw();
		}
	}
}

KazMath::Vec3<float> LineLevel1::CalucurateMoveVector(const int &RANDM_NUM, const float &LENGTH)
{
	KazMath::Vec3<float> vel{};
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
	int random = KazMath::Rand<int>(MOVE_VEC_MAX_NUM, 0);
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
			random = KazMath::Rand<int>(MOVE_VEC_MAX_NUM, 0);
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
	int random = KazMath::Rand<int>(MOVE_VEC_MAX_NUM, 0);
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
			random = KazMath::Rand<int>(MOVE_VEC_MAX_NUM, 0);
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
		int random = KazMath::Rand<int>(LINE_MOVE_Z + 1, LINE_MOVE_X);
		if (DONT_USE != random && OLD_NUM != random)
		{
			eVec = static_cast<eLineMove>(random);
			break;
		}
	}

	return eVec;
}

KazMath::Vec3<float> LineLevel1::firstDir(eSurface SURFACE)
{
	KazMath::Vec3<float> dir = { 0.0f,0.0f,0.0f };
	float vec = KazMath::Rand<float>(30.0f, 10.0f);
	switch (SURFACE)
	{
	case SURFACE_NONE:
		break;
	case SURFACE_LEFT:
		dir.x += -vec;
		break;
	case SURFACE_RIGHT:
		dir.x += vec;
		break;
	case SURFACE_FRONT:
		dir.z += -vec;
		break;
	case SURFACE_BACK:
		dir.z += vec;
		break;
	case SURFACE_TOP:
		dir.y += vec;
		break;
	case SURFACE_BUTTOM:
		dir.y += -vec;
		break;
	default:
		break;
	}

	return dir;
}
