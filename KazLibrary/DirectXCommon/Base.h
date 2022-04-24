#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include<DirectXMath.h>
using namespace DirectX;

#include<wrl.h>
using namespace Microsoft::WRL;
#include"d3dx12.h"
using namespace std;
#include<array>
#include<memory>

static const int WIN_X = 1280;
static const int WIN_Y = 720;

//static const XMFLOAT3 BG_COLOR = { 255.0f,255.0f,255.0f };
static const XMFLOAT3 BG_COLOR_WHITE = { 255.0f,255.0f,255.0f };
static const XMFLOAT3 BG_COLOR = { 158.0f,37.0f,179.0f };
//static const XMFLOAT3 BG_COLOR = { 22,38,37 };
static const XMFLOAT3 PLAYER_POS = { 0.0f,10.0f,30.0f };
//static const XMFLOAT3 BG_COLOR = { 255,255,255 };

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

struct SpriteVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

struct ConstBufferData
{
	XMFLOAT4 color;
	XMMATRIX mat;
	XMMATRIX viewproj;
	XMMATRIX world;
	XMMATRIX view;
};

struct LineVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tickness;
};


struct posData
{
	XMFLOAT3 endLinepos;
	bool checkFlag;
};

static const int MAX_BONES = 32;
struct ConstBufferDataSkin
{
	XMMATRIX bones[MAX_BONES];
};

/// <summary>
/// dogData
/// xyz...Color
/// w...Density
/// </summary>
struct FogData
{
	XMFLOAT4 fogdata;
};

static int TEST_COUNT;



struct RockOnPosData
{
	XMFLOAT3 pos;
	bool useFlag;
};

struct LineData
{
	float tickness;
};

struct GoalLightData
{
	XMFLOAT4 light;
};


struct Material
{
	string name;//マテリアル名
	XMFLOAT3 ambient;//アンビエント影響度
	XMFLOAT3 diffuse;//ディフューズ影響度
	XMFLOAT3 specular;//スペキュラー影響度
	float alpha;//α
	string textureFilename;//テクスチャファイル名

	Material()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0;
	}
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient;	//アンビエント係数
	float pad1;			//バディング
	XMFLOAT3 diffuse;	//ディフューズ係数
	float pad2;			//パディング
	XMFLOAT3 specular;	//スペキュラー係数
	float alpha;		//アルファ
};

struct ScreenPosData
{
	XMVECTOR LeftUpPos;
	XMVECTOR LeftDownPos;
	XMVECTOR RightUpPos;
	XMVECTOR RightDownPos;
};

struct constBufferVhsData
{
	float time;
};