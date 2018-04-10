//=============================================================================
// D3DUtil.h by Frank Luna (C) 2005 All Rights Reserved.
// modified by Bart Uyttenhove
// Contains various utility code for DirectX applications, such as, clean up
// and debugging code.
//=============================================================================

#pragma once 

// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO

#endif
#endif

#pragma comment(lib, "ObjReader.lib")
#ifdef _UNICODE
#pragma comment(lib, "libexpatw.lib")
#else
#pragma comment(lib, "libexpat.lib")
#endif

#include <d3d10.h>
#include <d3dx10.h>
#include <D3DX10Math.h>
#pragma comment(lib, "d3d10.lib")

//voor timegettime
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

//voor DXTrace
#pragma comment(lib, "dxerr.lib")
#include "dxerr.h"

//===============================================================
// Colors and Materials

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);



typedef struct DirLight
{
	D3DXCOLOR Ambient;
	D3DXCOLOR Diffuse;
	D3DXCOLOR Specular;
	D3DXVECTOR3 DirW;
}DirLight;


//===============================================================
// Math Constants

const float INFINITY = FLT_MAX;
const float EPSILON  = 0.001f;

//===============================================================
// Bounding Volumes

struct AABB 
{
	// Initialize to an infinitely small bounding box.
	AABB()
		: minPt(INFINITY, INFINITY, INFINITY),
		  maxPt(-INFINITY, -INFINITY, -INFINITY){}

    D3DXVECTOR3 center()
	{
		return 0.5f*(minPt+maxPt);
	}
	D3DXVECTOR3 size()
	{
		return D3DXVECTOR3(maxPt.x-minPt.x,maxPt.y-minPt.y,maxPt.z-minPt.z);
	}
	D3DXVECTOR3 minPt;
	D3DXVECTOR3 maxPt;
};

struct BoundingSphere 
{
	BoundingSphere()
		: pos(0.0f, 0.0f, 0.0f), radius(0.0f){}

	D3DXVECTOR3 pos;
	float radius;
};

//===============================================================
// Clean up

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p!=0) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p!=0) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    

//===============================================================
// Debug

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if(FAILED(hr))                                 \
		{                                              \
			DXTrace(__FILE__, __LINE__, hr, _T(#x), TRUE); \
		}                                              \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) x;
	#endif
#endif 

