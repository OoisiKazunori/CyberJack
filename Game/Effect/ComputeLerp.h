#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"

/// <summary>
/// メモリ上の二つの値を使って線形補完を行う
/// </summary>
class ComputeLerp
{
public:
	ComputeLerp(const ComputeBufferHelper::BufferData &BASE_DATA, const ComputeBufferHelper::BufferData &LERP_DATA);
	void Compute();

private:
	ComputeBufferHelper computeHelper;
};

