#pragma once
struct VertexPosCol
{
	VertexPosCol():pos(0.0f, 0.0f, 0.0f),color(1.0f,1.0f,1.0f,1.0f){}
	VertexPosCol(float x, float y, float z,	float cx, float cy, float cz, float cw):pos(x,y,z), color(cx,cy,cz,cw){}

	D3DXVECTOR3 pos;
	D3DXCOLOR color;
};

struct VertexPos
{
	VertexPos():pos(0.0f, 0.0f, 0.0f){}
	VertexPos(float x, float y, float z):pos(x,y,z){}

	D3DXVECTOR3 pos;
};

struct VertexPosColNormal
{
	VertexPosColNormal():pos(0.0f, 0.0f, 0.0f),color(1.0f,1.0f,1.0f,1.0f),normal(0.0f, 0.0f, 0.0f){}
	VertexPosColNormal(float x, float y, float z,	float cx, float cy, float cz, float cw, float nx, float ny, float nz):pos(x,y,z), color(cx,cy,cz,cw),normal(nx,ny,nz){}

	D3DXVECTOR3 pos;
	D3DXCOLOR color;
	D3DXVECTOR3 normal;
};
//===============================================================
struct VertexPT
{
	VertexPT():pos(0.0f, 0.0f, 0.0f),tex0(0.0f, 0.0f){}
	VertexPT(float x, float y, float z, float u, float v):pos(x,y,z),  tex0(u,v){}
	VertexPT(const D3DXVECTOR3& v, const D3DXVECTOR2& uv):pos(v), tex0(uv){}

	D3DXVECTOR3 pos;
	D3DXVECTOR2 tex0;
};

//===============================================================
struct VertexPNT
{
	VertexPNT()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f){}
	VertexPNT(float x, float y, float z, 
		float nx, float ny, float nz,
		float u, float v):pos(x,y,z), normal(nx,ny,nz), tex0(u,v){}
	VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv)
		:pos(v),normal(n), tex0(uv){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;
};
