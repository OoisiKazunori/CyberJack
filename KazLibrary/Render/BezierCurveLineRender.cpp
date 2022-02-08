//#include "BezierCurveLineRender.h"
//
//#include"../DirectXCommon/DirectX12CmdList.h"
//#include"../DirectXCommon/DirectX12Device.h"
//
//#include"ConstBufferMgr.h"
//#include"../Loader/TextureResourceMgr.h"
//
//BezierCurveLineRender::BezierCurveLineRender()
//{
//	div = 100.0f;
//
//	vertices.resize(div);
//
//	UINT VertLEN = vertices.size();
//	UINT VertByte = VertLEN * sizeof(Vertex);
//
//
//	//OK‚¾‚Á‚½‚ç‚Ç‚ñ‚Ç‚ñˆ—‚ði‚ß‚é
//	result = vertBuff.CreateBuffer(VertByte);
//	if (result == S_OK)
//	{
//		result = vertBuff.SendDataToGPU(vertices.data(), VertLEN);
//		if (result == S_OK)
//		{
//
//			result = vertBuff.CreateBufferView(vertices.data(), VertByte);
//			if (result == S_OK)
//			{
//				//Š®—¹
//			}
//		}
//	}
//
//	GetInstance();
//
//
//	initFlag = false;
//	CbvHandle = baseConstBufferMgr->CreateBuffer();
//}
//
//BezierCurveLineRender::~BezierCurveLineRender()
//{
//	vertBuff.~VertexBuffer();	
//	baseConstBufferMgr->Release(CbvHandle);
//}
//
//void BezierCurveLineRender::DrawCurve(XMFLOAT3 POS, XMFLOAT3 POS2, XMFLOAT3 CONTROL_POINT, XMFLOAT3 CONTROL_POINT2, XMFLOAT4 COLOR, PipeLineNames PIPELINE)
//{	
//	basePipelineMgr->SetPipeLineAndRootSignature(PIPELINE_NAME_LINE);
//
//
//	//§Œä“_1~2‚Ö
//	XMVECTOR a = XMVectorSet(POS.x, POS.y, POS.z, 1);
//	XMVECTOR b = XMVectorSet(CONTROL_POINT.x, CONTROL_POINT.y, CONTROL_POINT.z, 1);
//	XMVECTOR a_b_Distance = b - a;
//
//	speed[0].x = a_b_Distance.m128_f32[0] / div;
//	speed[0].y = a_b_Distance.m128_f32[1] / div;
//	speed[0].z = a_b_Distance.m128_f32[2] / div;
//
//
//	//§Œä“_2~3‚Ö
//	XMVECTOR c = XMVectorSet(CONTROL_POINT.x, CONTROL_POINT.y, CONTROL_POINT.z, 1);
//	XMVECTOR d = XMVectorSet(POS2.x, POS2.y, POS2.z, 1);
//	XMVECTOR c_d_Distance = d - c;
//
//	speed[1].x = c_d_Distance.m128_f32[0] / div;
//	speed[1].y = c_d_Distance.m128_f32[1] / div;
//	speed[1].z = c_d_Distance.m128_f32[2] / div;
//
//
//
//	blueLinePos[0] = POS;
//	blueLinePos[1] = CONTROL_POINT;
//	PointPos = POS;
//
//
//
//	float t = 0;
//	for (float i = 0.0f; i != 100.0f; i += 1.0f)
//	{
//		t = i / 100.0f;
//		vertices[i].pos = Bezei(POS, POS2, CONTROL_POINT, CONTROL_POINT2, t);
//	}
//
//	
//
//
//
//	vertBuff.SendDataToGPU(vertices.data(), vertices.size());
//
//
//
//
//	matWorld = XMMatrixIdentity();
//	baseConstBufferMgr->CaluMatrix(CbvHandle, matWorld, cameraMgr->GetViewMatrix(), PROJECTION_MODE_PERSPECTIVE);
//	baseConstBufferMgr->MapingColor({ COLOR.x / 255.0f,COLOR.y / 255.0f,COLOR.z / 255.0f,1.0 }, CbvHandle);
//	baseConstBufferMgr->SetCBV(CbvHandle, baseRootSigMgr->GetRootParam(basePipelineMgr->GetRootSignatureName(PIPELINE_NAME_LINE)), GRAPHICS_PRAMTYPE_DRAW);
//
//
//	baseCmdList->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
//	baseCmdList->cmdList->IASetVertexBuffers(0, 1, &vertBuff.vbView);
//	baseCmdList->cmdList->DrawInstanced(vertices.size(), 1, 0, 0);
//}
//
//XMFLOAT3 BezierCurveLineRender::Bezei(XMFLOAT3 POS, XMFLOAT3 POS2, XMFLOAT3 CONTROL_POINT, XMFLOAT3 CONTROL_POINT2, float T)
//{
//	XMFLOAT3 p;
//	p.x = pow(1 - T, 3) * POS.x + 3 * pow(1 - T, 2) * T * CONTROL_POINT.x + 3 * (1 - T) * T * T * CONTROL_POINT2.x + T * T * T * POS2.x;
//	p.y = pow(1 - T, 3) * POS.y + 3 * pow(1 - T, 2) * T * CONTROL_POINT.y + 3 * (1 - T) * T * T * CONTROL_POINT2.y + T * T * T * POS2.y;
//	p.z = pow(1 - T, 3) * POS.z + 3 * pow(1 - T, 2) * T * CONTROL_POINT.z + 3 * (1 - T) * T * T * CONTROL_POINT2.z + T * T * T * POS2.z;
//	return p;
//}