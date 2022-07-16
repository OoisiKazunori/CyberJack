#pragma once
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>

class GpuParticleStage :public IStage
{
public:
	GpuParticleStage();
	void Update()override;
	void Draw()override;

private:

};