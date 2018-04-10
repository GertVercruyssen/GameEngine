#include "StdAfx.h"
#include "util.h"
#include "ObjPhysicsReader.h"
#include "StringUtil.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ObjPhysicsReader::ObjPhysicsReader(tstring filename):
m_FileName(filename),
m_VertexOffset(0),
m_pCurrentMesh(0)
{
	m_Input.open(filename.c_str());
}

ObjPhysicsReader::~ObjPhysicsReader(void)
{
}

float3 ObjPhysicsReader::GetCenterPoint(){
	float3 result;
	//m_MaxVertex.Add(result,m_MinVertex);
	//result.Scale(result,.5f);
	return result;
}

float ObjPhysicsReader::GetHeight(){
	return m_MaxVertex.y - m_MinVertex.y;
}

bool ObjPhysicsReader::Read(){
	if ( !m_Input)
		return false;

	tstring line;
	if (m_Input.is_open())
	{
		while (! m_Input.eof() )
		{
			getline (m_Input,line);
			// only vertices , faces and groups are important
			if ( line.find(_T("v"),0) == 0){
				ReadVertex(line);
			}else if ( line.find(_T("f"),0) == 0){
				ReadFace(line);
			}else if ( line.find(_T("g"),0) == 0){
				ReadGroup(line);
			}
		}
		m_Input.close();
	}
	return true;
}

void ObjPhysicsReader::ReadVertex(tstring& line)
{
	StringUtil::Split(line,_T(" "),parseHelp);
	
	if ( parseHelp.size() >= 4 ){
		float3 loc;
		loc.x = static_cast<float>(tof(parseHelp[1].c_str()));
		loc.y = static_cast<float>(tof(parseHelp[2].c_str()));
		loc.z = static_cast<float>(tof(parseHelp[3].c_str()));
		
		if (m_vertices.size() == 0 ){
			m_MaxVertex=loc;
			m_MinVertex=loc;
		}else{
			if ( loc.x > m_MaxVertex.x )
				m_MaxVertex.x = loc.x;
			if ( loc.y > m_MaxVertex.y )
				m_MaxVertex.y = loc.y;
			if ( loc.z > m_MaxVertex.z )
				m_MaxVertex.z = loc.z;
			if ( loc.x < m_MinVertex.x )
				m_MinVertex.x = loc.x;
			if ( loc.y < m_MinVertex.y )
				m_MinVertex.y = loc.y;
			if ( loc.z < m_MinVertex.z )
				m_MinVertex.z = loc.z;
		}
		m_vertices.push_back(loc);
	}
}
void ObjPhysicsReader::ReadFace(tstring& line)
{
	StringUtil::Split(line,_T(" "),parseHelp);
	// number in parseHelp should be 3
	int fis[3];
	if ( parseHelp.size() == 4 )
	{
		vector<tstring>::iterator it = parseHelp.begin();
		// first element is 'f', skip it.
		++it;
		int faceIndex=0;
		while( it != parseHelp.end()){
			StringUtil::Split(*it,_T("/"),parseHelp2,false);
			tstringstream svi(parseHelp2[0]);
			int absVertexIndex = -1;
			svi>>absVertexIndex;
			fis[faceIndex] = (absVertexIndex-1);
			it++;
			faceIndex++;
		}
		// add the face to the triangle list
		m_Faces.push_back(fis[2]);
		m_Faces.push_back(fis[1]);
		m_Faces.push_back(fis[0]);
	}
}

void ObjPhysicsReader::ReadGroup(tstring& line)
{
	StringUtil::Split(line,_T(" "),parseHelp);
	if ( parseHelp.size() >= 2){
		if (m_Faces.size() == 0){
			m_pCurrentMesh = new InputMesh();
			m_pCurrentMesh->SetName(parseHelp[1]);
		}else
			CloseCurrentSubgroup();
		
	}else{
		if ( m_pCurrentMesh != NULL ){
			CloseCurrentSubgroup();
			CloseCurrentMesh();
		}
		m_pCurrentMesh = NULL;
		m_vertices.clear();
		m_Faces.clear();
	}
}

void ObjPhysicsReader::CloseCurrentSubgroup(){
	m_pCurrentMesh->AddTriangleList(m_Faces);
	m_Faces.clear();
}

void ObjPhysicsReader::CloseCurrentMesh(){
	m_pCurrentMesh->SetVertexList(m_vertices);
	m_Meshes.push_back(m_pCurrentMesh);
}

float3& ObjPhysicsReader::GetVertex(int index){
	if ( index < 0 ){
		return m_vertices[m_vertices.size()-index];
	}else{
		return m_vertices[index];
	}
}