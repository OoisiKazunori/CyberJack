#include "KazRenderHelper.h"

array<XMFLOAT2, 4> KazRenderHelper::ChangePlaneScale(const XMFLOAT2 &LEFTUP_POS, const XMFLOAT2 &RIGHTDOWN_POS, const XMFLOAT2 &SCALE, const XMFLOAT2 &ANCHOR_POINT, const XMFLOAT2 &TEX_SIZE)
{
	//ì«Ç›çûÇ›
	float width = TEX_SIZE.x;
	float height = TEX_SIZE.y;


	float left = (LEFTUP_POS.x - ANCHOR_POINT.x) * width * SCALE.x;
	float right = (RIGHTDOWN_POS.x - ANCHOR_POINT.x) * width * SCALE.x;
	float up = (LEFTUP_POS.y - ANCHOR_POINT.y) * height * SCALE.y;
	float down = (RIGHTDOWN_POS.y - ANCHOR_POINT.y) * height * SCALE.y;


	array<XMFLOAT2, 4>pos;
	pos[0] = { left, down };
	pos[1] = { left, up };
	pos[2] = { right, down };
	pos[3] = { right, up };
	return pos;
}

array<XMFLOAT2, 4> KazRenderHelper::ChangeModiPlaneScale(const XMFLOAT2 &LEFTUP_POS, const XMFLOAT2 &RIGHTDOWN_POS, const XMFLOAT4 &SCALE, const XMFLOAT2 &ANCHOR_POINT, const XMFLOAT2 &TEX_SIZE)
{
	//ì«Ç›çûÇ›
	float width = TEX_SIZE.x;
	float height = TEX_SIZE.y;


	float left = (LEFTUP_POS.x - ANCHOR_POINT.x) * width * SCALE.x;
	float right = (RIGHTDOWN_POS.x - ANCHOR_POINT.x) * width * SCALE.y;
	float up = (LEFTUP_POS.y - ANCHOR_POINT.y) * height * SCALE.z;
	float down = (RIGHTDOWN_POS.y - ANCHOR_POINT.y) * height * SCALE.w;


	array<XMFLOAT2, 4>pos;
	pos[0] = { left, down };
	pos[1] = { left, up };
	pos[2] = { right, down };
	pos[3] = { right, up };
	return pos;
}

void KazRenderHelper::FlipXUv(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS)
{
	XMFLOAT2 leftUp = *UV_LEFTUP_POS;
	XMFLOAT2 rightUp = *UV_RIGHTUP_POS;
	XMFLOAT2 leftDown = *UV_LEFTDOWN_POS;
	XMFLOAT2 rightDown = *UV_RIGHTDOWN_POS;

	UV_LEFTUP_POS->x = rightUp.x;
	UV_RIGHTUP_POS->x = leftUp.x;
	UV_LEFTDOWN_POS->x = rightDown.x;
	UV_RIGHTDOWN_POS->x = leftDown.x;

}

void KazRenderHelper::FlipYUv(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS)
{
	XMFLOAT2 leftUp = *UV_LEFTUP_POS;
	XMFLOAT2 rightUp = *UV_RIGHTUP_POS;
	XMFLOAT2 leftDown = *UV_LEFTDOWN_POS;
	XMFLOAT2 rightDown = *UV_RIGHTDOWN_POS;

	UV_LEFTUP_POS->y = leftDown.y;
	UV_RIGHTUP_POS->y = rightDown.y;
	UV_LEFTDOWN_POS->y = leftUp.y;
	UV_RIGHTDOWN_POS->y = rightUp.y;
}

void KazRenderHelper::VerticesCut(XMFLOAT2 DIV_SIZE, XMFLOAT2 DIV_LEFTUP_POS, XMFLOAT3 *LEFTUP_POS, XMFLOAT3 *LEFTDOWN_POS, XMFLOAT3 *RIGHTUP_POS, XMFLOAT3 *RIGHTDOWN_POS, const XMFLOAT2 &SCALE, const XMFLOAT2 &ANCHOR_POINT)
{
	XMFLOAT2 texSize = DIV_SIZE;
	XMFLOAT2 divGraphPos = DIV_LEFTUP_POS;


	XMFLOAT2 leftUp, rightDown;
	leftUp = { 0.0f,0.0f };
	rightDown = { 1.0f,1.0f };
	array<XMFLOAT2, 4>tmp = ChangePlaneScale(leftUp, rightDown, SCALE, ANCHOR_POINT, DIV_SIZE);






	*LEFTUP_POS = { tmp[2].x,tmp[2].y,0.0f };
	*LEFTDOWN_POS = { tmp[3].x,tmp[3].y,0.0f };
	*RIGHTUP_POS = { tmp[0].x,tmp[0].y,0.0f };
	*RIGHTDOWN_POS = { tmp[1].x,tmp[1].y,0.0f };
}

void KazRenderHelper::UVCut(XMFLOAT2 UV_DIV_LEFTUP_POS, XMFLOAT2 DIV_SIZE, XMFLOAT2 TEX_SIZE, XMFLOAT2 *LEFTUP_POS, XMFLOAT2 *LEFTDOWN_POS, XMFLOAT2 *RIGHTUP_POS, XMFLOAT2 *RIGHTDOWN_POS)
{
	float texX = UV_DIV_LEFTUP_POS.x;
	float texY = UV_DIV_LEFTUP_POS.y;
	float texHeight = DIV_SIZE.y;
	float texWidth = DIV_SIZE.x;

	float tex_left = texX / TEX_SIZE.x;
	float tex_right = (texX + texWidth) / TEX_SIZE.x;
	float tex_top = texY / TEX_SIZE.y;
	float tex_bottom = (texY + texHeight) / TEX_SIZE.y;

	*LEFTUP_POS = { tex_right,tex_top };
	*LEFTDOWN_POS = { tex_right,tex_bottom };
	*RIGHTUP_POS = { tex_left,tex_top };
	*RIGHTDOWN_POS = { tex_left,tex_bottom };
}

void KazRenderHelper::InitUvPos(XMFLOAT2 *UV_LEFTUP_POS, XMFLOAT2 *UV_LEFTDOWN_POS, XMFLOAT2 *UV_RIGHTUP_POS, XMFLOAT2 *UV_RIGHTDOWN_POS)
{
	*UV_LEFTUP_POS = { 0.0f,0.0f };
	*UV_LEFTDOWN_POS = { 0.0f,1.0f };
	*UV_RIGHTUP_POS = { 1.0f,0.0f };
	*UV_RIGHTDOWN_POS = { 1.0f,1.0f };
}

void KazRenderHelper::InitVerticesPos(XMFLOAT3 *LEFTUP_POS, XMFLOAT3 *LEFTDOWN_POS, XMFLOAT3 *RIGHTUP_POS, XMFLOAT3 *RIGHTDOWN_POS, const XMFLOAT2 &ANCHOR_POINT)
{
	*LEFTUP_POS = { (0.0f - ANCHOR_POINT.x), (0.0f - ANCHOR_POINT.y),0.0f };
	*LEFTDOWN_POS = { (0.0f - ANCHOR_POINT.x), (1.0f - ANCHOR_POINT.y),0.0f };
	*RIGHTUP_POS = { (1.0f - ANCHOR_POINT.x), (0.0f - ANCHOR_POINT.y) ,0.0f };
	*RIGHTDOWN_POS = { (1.0f - ANCHOR_POINT.x), (1.0f - ANCHOR_POINT.y) ,0.0f };
}

array<unsigned short, 6> KazRenderHelper::InitIndciesForPlanePolygon()
{
	array<unsigned short, 6>result;
	result[0] = 0;
	result[1] = 1;
	result[2] = 2;
	result[3] = 2;
	result[4] = 1;
	result[5] = 3;
	return result;
}

int KazRenderHelper::SetBufferOnCmdList(const GraphicsRootSignatureParameter &PARAM, const GraphicsRangeType &RANGE, const GraphicsRootParamType &TYPE)
{
	int type = -1;
	int keepParam = -1;
	//ÉãÅ[ÉgÉpÉâÉÄÇÃî‘çÜÇï€ë∂
	for (int i = 0; i < PARAM.paramMax; i++)
	{
		if (PARAM.range[i] == RANGE && PARAM.paramData[i].type == TYPE)
		{
			type = PARAM.type[i];
			keepParam = PARAM.paramData[i].param;
		}
	}

	return keepParam;
}

XMFLOAT4 KazRenderHelper::SendColorDataToGPU(XMFLOAT4 COLOR_DATA)
{
	return XMFLOAT4(COLOR_DATA.x / 255.0f, COLOR_DATA.y / 255.0f, COLOR_DATA.z / 255.0f, COLOR_DATA.w / 255.0f);
}
