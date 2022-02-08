//#pragma once
//#include"../DirectXCommon/Base.h"
//#include"VertexBuffer.h"
//#include"../Render/IRender.h"
//
//class BezierCurveLineRender:public IRender
//{
//public:
//	BezierCurveLineRender();
//	~BezierCurveLineRender();
//
//	void DrawCurve(XMFLOAT3 POS, XMFLOAT3 POS2, XMFLOAT3 CONTROL_POINT, XMFLOAT3 CONTROL_POINT2, XMFLOAT4 COLOR, PipeLineNames PIPELINE = PIPELINE_NAME_LINE);
//
//private:
//	HRESULT result;
//
//	vector<Vertex> vertices;
//	VertexBuffer vertBuff;
//
//	XMFLOAT3 speed[3], blueLinePos[2], PointPos;
//	XMMATRIX matWorld, matTrans;
//
//	short CbvHandle;
//	short CbvHandle2;
//
//
//	float div;
//
//	bool initFlag;
//
//	XMFLOAT3 Bezei(XMFLOAT3 POS, XMFLOAT3 POS2, XMFLOAT3 CONTROL_POINT, XMFLOAT3 CONTROL_POINT2, float T);
//};
//
