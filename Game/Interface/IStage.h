#pragma once

/// <summary>
/// �X�e�[�W�̒��ۃN���X
/// �X�e�[�W���̕`���Z�߂Ă���܂�
/// </summary>
class IStage
{
public:
	~IStage() {};
	virtual void Update() = 0;
	virtual void Draw() = 0;

private:
};

