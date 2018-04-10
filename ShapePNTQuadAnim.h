#pragma once
#include "d3dutil.h"
#include "ShapePNTQuad.h"

class ShapePNTQuadAnim :public ShapePNTQuad
{
public:
	ShapePNTQuadAnim(const tstring& TexFileName);
	virtual ~ShapePNTQuadAnim(void);

	virtual void Tick(float dTime);
protected:
	static const int NUMCELLS = 8;
	int m_CellNr;
	float m_FrameTime;
	ID3D10ShaderResourceView *m_pTextureAnim[NUMCELLS];


};
