#pragma once
#include"../DirectXCommon/Base.h"

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

	DirtySet(float &DATA)
	{
		shared_ptr<DirtyFlag<float>> tmp;
		tmp.reset(new DirtyFlag<float>(&DATA));
		floatDirtyFlag.push_back(tmp);
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
		//Ç«ÇÃílÇ™è≠ÇµÇ≈Ç‡1FlameëOÇÃílÇ∆î‰Ç◊ÇƒïœÇÌÇ¡ÇƒÇ¢ÇΩÇÁtrue
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


	void Record()
	{
		for (int i = 0; i < floatDirtyFlag.size(); i++)
		{
			floatDirtyFlag[i]->Record();
		}
	}


private:
	std::vector<std::shared_ptr<DirtyFlag<float>>>floatDirtyFlag;
};
