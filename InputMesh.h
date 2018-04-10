#pragma once
#include "util.h"
#include "float3.h"


#include <vector>
using namespace std;

typedef unsigned char byte;

/**
* This class contains a VertexList and usually one TriangleList.
* Acts as a container for the point clowd en the triangles
* @author Koen Samyn
*/
class InputMesh
{


public:
	/**
	* Creates an empty InputMesh object.
	*/
	InputMesh();

	/**
	* Creates a new InputMesh object with one VertexList and
	* one TriangleList.
	* @param vertices the list with all the vertices.
	* @param triangle the list with the triangles.
	*/
	InputMesh(vector<float3>&  vertices, vector<int>& triangles);
	/**
	* Destroyes the InputMesh object.
	*/
	virtual ~InputMesh(void);
	/**
	* Sets the vertexlist for this InputMesh.
	* @param vertexlist the list of vertices.
	*/
	void SetVertexList(vector<float3> & vertexList){
		m_VertexList=vertexList;
	}
	/**
	* Adds a triangle list to the list of triangles.
	* @param triangles the triangle list to add.
	*/
	void AddTriangleList(vector<int> & triangles){
		m_TriangleLists.insert(m_TriangleLists.end(),triangles.begin(),triangles.end());
	}
	/**
	* Sets the name for the input mesh
	* @param name the name
	*/
	void SetName(tstring name){m_Name = name;}
	/**
	* Gets the name for the input mesh.
	* @return the name for the input mesh.
	*/
	tstring GetName() const {return m_Name;}
	/**
	* Returns the number of vertices in the input mesh.
	* @return the number of vertices.
	*/
	int GetNumVertices() const {
		return (int)m_VertexList.size();
	}
	/**
	* Returns the number of faces in the input mesh.
	* @return the number of faces.
	*/
	int GetNumFaces() const;
	/**
	* Returns a pointer to the start of the vertices buffer.
	* @return the pointer.
	*/
	void* GetVertexPointer(){
		return &(m_VertexList[0]);
	}

	/**
	* Returns the number of bytes per vertex, in other words, the stride.
	* @return the stride for the vertexlist in bytes.
	*/
	int GetNumBytesPerVertex(){
		return 3*sizeof(float);
	}
	/**
	* Returns a pointer to the start of the trianglelists.
	* If only one triangle list is present, this method will return
	* a pointer to the start of the list, otherwise a new buffer
	* will be created.
	* @return the pointer
	*/
	void* GetTrianglesPointer();

private:
	/**
	* The name for the input mesh
	*/
	tstring m_Name;
	/**
	* The VertexList object that contains the vertices for this
	* Cooking process.
	*/
	vector<float3> m_VertexList;
	/**
	* The triangle lists that contain the triangles.
	*/ 
	vector<int> m_TriangleLists;
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	InputMesh(const InputMesh& t);
	InputMesh& operator=(const InputMesh& t);
};

typedef vector<InputMesh*>::const_iterator MIT;