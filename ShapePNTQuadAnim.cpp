#include "StdAfx.h"
#include "ShapePNTQuadAnim.h"
#include "TextureManager.h"

ShapePNTQuadAnim::ShapePNTQuadAnim(const tstring& TexFileName):ShapePNTQuad(_T("")),
		m_CellNr(0),m_FrameTime(0)
{
	for(int i=0;i<NUMCELLS;++i)
	{
		tstringstream strstr;
		//if(i<9)strstr << _T("./Textures/FireAnim/Fire00") << i+1 << _T(".bmp");
		//else if(i<99)strstr << _T("./Textures/FireAnim/Fire0") << i+1 << _T(".bmp");
		//else strstr << _T("./Textures/FireAnim/Fire") << i+1 << _T(".bmp");
		strstr << _T("./Textures/Tarzan/Tarzan") << i+1 << _T(".bmp");
		m_pTextureAnim[i] = TEXTUREMANAGER.GetTexture(strstr.str().c_str());
	}
}

ShapePNTQuadAnim::~ShapePNTQuadAnim(void)
{
}
void ShapePNTQuadAnim::Tick(float dTime)
{
	m_pTextureRV = m_pTextureAnim[m_CellNr];

	//30 frames per second
	m_FrameTime+=dTime;
	if(m_FrameTime>1/30.0f)
	{
		m_FrameTime=0;
		++m_CellNr%=NUMCELLS;
	}
}

