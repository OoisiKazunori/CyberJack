#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"

/// <summary>
/// ��������̓�̒l���g���Đ��`�⊮���s��
/// </summary>
class ComputeLerp
{
public:
	ComputeLerp(const ComputeBufferHelper::BufferData &BASE_DATA, const ComputeBufferHelper::BufferData &LERP_DATA);
	void Compute();

private:
	ComputeBufferHelper computeHelper;
};

