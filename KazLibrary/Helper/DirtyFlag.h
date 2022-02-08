#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/RenderData.h"

template<class T>
class DirtyFlag
{
public:
	DirtyFlag(T *TARGET, bool SEND_DATA_FIRST_TIME = true) : target(TARGET), oldTarget(*TARGET)
	{
		sendDataFlag = true;
		sendDataFlag = SEND_DATA_FIRST_TIME;
	};

	bool Dirty()
	{
		bool tmp = sendDataFlag;
		sendDataFlag = false;
		return (oldTarget != *target) || tmp;
	}
	void Record()
	{
		oldTarget = *target;
	};

private:
	T *target;
	T oldTarget;

	bool sendDataFlag;
};




class DirtySet
{
public:

	~DirtySet()
	{
	}

	DirtySet(XMVECTOR &DATA)
	{
		shared_ptr<DirtyFlag<float>> tmp;
		shared_ptr<DirtyFlag<float>> tmp1;
		shared_ptr<DirtyFlag<float>> tmp2;
		tmp.reset(new DirtyFlag<float>(&DATA.m128_f32[0]));
		tmp1.reset(new DirtyFlag<float>(&DATA.m128_f32[1]));
		tmp2.reset(new DirtyFlag<float>(&DATA.m128_f32[2]));
		floatDirtyFlag.push_back(tmp);
		floatDirtyFlag.push_back(tmp1);
		floatDirtyFlag.push_back(tmp2);
	};
	DirtySet(XMFLOAT4 &DATA)
	{
		shared_ptr<DirtyFlag<float>> tmp;
		shared_ptr<DirtyFlag<float>> tmp1;
		shared_ptr<DirtyFlag<float>> tmp2;
		shared_ptr<DirtyFlag<float>> tmp3;
		tmp.reset(new DirtyFlag<float>(&DATA.x));
		tmp1.reset(new DirtyFlag<float>(&DATA.y));
		tmp2.reset(new DirtyFlag<float>(&DATA.z));
		tmp3.reset(new DirtyFlag<float>(&DATA.w));
		floatDirtyFlag.push_back(tmp);
		floatDirtyFlag.push_back(tmp1);
		floatDirtyFlag.push_back(tmp2);
		floatDirtyFlag.push_back(tmp3);
	};
	DirtySet(XMFLOAT3 &DATA)
	{
		shared_ptr<DirtyFlag<float>> tmp;
		shared_ptr<DirtyFlag<float>> tmp1;
		shared_ptr<DirtyFlag<float>> tmp2;
		tmp.reset(new DirtyFlag<float>(&DATA.x));
		tmp1.reset(new DirtyFlag<float>(&DATA.y));
		tmp2.reset(new DirtyFlag<float>(&DATA.z));
		floatDirtyFlag.push_back(tmp);
		floatDirtyFlag.push_back(tmp1);
		floatDirtyFlag.push_back(tmp2);
	};
	DirtySet(XMFLOAT2 &DATA)
	{
		shared_ptr<DirtyFlag<float>> tmp;
		shared_ptr<DirtyFlag<float>> tmp1;
		tmp.reset(new DirtyFlag<float>(&DATA.x));
		tmp1.reset(new DirtyFlag<float>(&DATA.y));
		floatDirtyFlag.push_back(tmp);
		floatDirtyFlag.push_back(tmp1);
	};
	DirtySet(float &DATA)
	{
		shared_ptr<DirtyFlag<float>> tmp;
		tmp.reset(new DirtyFlag<float>(&DATA));
		floatDirtyFlag.push_back(tmp);
	};
	DirtySet(FlipData &DATA)
	{
		shared_ptr<DirtyFlag<bool>> tmp;
		shared_ptr<DirtyFlag<bool>> tmp1;
		tmp.reset(new DirtyFlag<bool>(&DATA.x));
		tmp1.reset(new DirtyFlag<bool>(&DATA.y));
		boolDirtyFlag.push_back(tmp);
		boolDirtyFlag.push_back(tmp1);
	};
	DirtySet(XMMATRIX &DATA)
	{
		floatDirtyFlag.reserve(16);
		floatDirtyFlag.resize(16);

		int i = 0;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				floatDirtyFlag[i].reset(new DirtyFlag<float>(&DATA.r[y].m128_f32[x]));
				i++;
			}
		}
	};


	bool FloatDirty()
	{
		//どの値が少しでも1Flame前の値と比べて変わっていたらtrue
		bool isValueDiffrendFlag = false;
		for (int i = 0; i < floatDirtyFlag.size(); i++)
		{
			if (floatDirtyFlag[i]->Dirty())
			{
				isValueDiffrendFlag = true;
			}
		}
		return isValueDiffrendFlag;
	};
	bool BoolDirty()
	{
		//どの値が少しでも1Flame前の値と比べて変わっていたらtrue
		bool isValueDiffrendFlag = false;
		for (int i = 0; i < boolDirtyFlag.size(); i++)
		{
			if (boolDirtyFlag[i]->Dirty())
			{
				isValueDiffrendFlag = true;
			}
		}
		return isValueDiffrendFlag;
	};


	void Record()
	{
		for (int i = 0; i < floatDirtyFlag.size(); i++)
		{
			floatDirtyFlag[i]->Record();
		}
		for (int i = 0; i < boolDirtyFlag.size(); i++)
		{
			boolDirtyFlag[i]->Record();
		}
	}


private:
	vector<shared_ptr<DirtyFlag<float>>>floatDirtyFlag;
	vector<shared_ptr<DirtyFlag<bool>>>boolDirtyFlag;
};