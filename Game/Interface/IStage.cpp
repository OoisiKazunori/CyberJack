#include "IStage.h"

IStage::IStage()
{

}

IStage::~IStage()
{
}

void IStage::InitBackGround(const KazMath::Color &START_COLOR, const KazMath::Color &END_COLOR, float SPACE, float HEIGHT)
{
	{
		std::array<SpriteVertex, 4>vert;
		DirectX::XMVECTOR adjPos = { 50.0f,0.0f,0.0f };
		vert[0].pos = { -7680.0f,407.0f,0.0f };
		vert[0].uv = { 0.0f,0.0f };
		vert[1].pos = { 7680.0f,407.0f,0.0f };
		vert[1].uv = { 1.0f,0.0f };
		vert[2].pos = { -7680.0f,-407.0f,0.0f };
		vert[2].uv = { 0.0f,1.0f };
		vert[3].pos = { 7680.0f,-407.0f,0.0f };
		vert[3].uv = { 1.0f,1.0f };

		GradationData gradationData;
		gradationData.firstColor = START_COLOR.ConvertColorRateToXMFLOAT4();
		gradationData.endColor = END_COLOR.ConvertColorRateToXMFLOAT4();
		for (int i = 0; i < backGround.size(); ++i)
		{
			backGround[i] = std::make_unique<PolygonRender>(vert);
			RESOURCE_HANDLE handle = backGround[i]->CreateConstBuffer(sizeof(GradationData), typeid(GradationData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
			backGround[i]->TransData(&gradationData, handle, typeid(gradationData).name());
			backGround[i]->data.pipelineName = PIPELINE_NAME_SPRITE_GRADATION;
		}

		backGround[0]->data.transform.pos.z = SPACE;
		backGround[1]->data.transform.pos.x = SPACE;
		backGround[1]->data.transform.rotation.y = 100.0f;
		backGround[2]->data.transform.pos.x = -SPACE;
		backGround[2]->data.transform.rotation.y = 100.0f;
		backGround[3]->data.transform.pos.z = -SPACE;
	}

	{
		std::array<SpriteVertex, 4>vert;
		DirectX::XMVECTOR adjPos = { 50.0f,0.0f,0.0f };
		vert[0].pos = DirectX::XMFLOAT3(-800.0f, 0.0f, 700.0f);
		vert[0].uv = { 0.0f,0.0f };
		vert[1].pos = DirectX::XMFLOAT3(800.0f, 0.0f, 700.0f);
		vert[1].uv = { 1.0f,0.0f };
		vert[2].pos = DirectX::XMFLOAT3(-800.0f, 0.0f, -700.0f);
		vert[2].uv = { 0.0f,1.0f };
		vert[3].pos = DirectX::XMFLOAT3(800.0f, 0.0f, -700.0f);
		vert[3].uv = { 1.0f,1.0f };
		topPolygon = std::make_unique<PolygonRender>(vert);

		RESOURCE_HANDLE handle = topPolygon->CreateConstBuffer(sizeof(GradationData), typeid(GradationData).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		topPolygon->data.pipelineName = PIPELINE_NAME_SPRITE_GRADATION;

		GradationData gradData;
		gradData.endColor = END_COLOR.ConvertColorRateToXMFLOAT4();
		gradData.firstColor = END_COLOR.ConvertColorRateToXMFLOAT4();
		topPolygon->TransData(&gradData, handle, typeid(gradData).name());

		topPolygon->data.transform.pos.y = HEIGHT;
	}
}

void IStage::DrawBackGround()
{
	for (int i = 0; i < 4; ++i)
	{
		backGround[i]->Draw();
	}
	topPolygon->Draw();
}
