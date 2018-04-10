#include "StdAfx.h"
#include "InputMesh.h"

InputMesh::InputMesh()
{
}

InputMesh::InputMesh(vector<float3>& vertices, vector<int>& triangles)
{
	m_VertexList.assign(vertices.begin(),vertices.end());
	m_TriangleLists.insert(m_TriangleLists.end(),triangles.begin(),triangles.end());
}

InputMesh::~InputMesh(void)
{
}

int InputMesh::GetNumFaces() const
{
	return (int)m_TriangleLists.size()/3;
}

void* InputMesh::GetTrianglesPointer(){
	return &(m_TriangleLists[0]);
}