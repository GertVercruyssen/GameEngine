#pragma once
#include "float3.h"
#include "InputMesh.h"
#include <fstream>
#include <string>
#include <vector>

/**
* Reads and parses an obj file. This reader ignores
* normals and texture coordinates, and takes only the
* vertices and faces into account.
* @author Koen Samyn
*/
class ObjPhysicsReader
{
public:
	/**
	* Creates a new ObjPhysicsReader object for the provided file.
	* @param fileName the file to read.
	*/
	ObjPhysicsReader(tstring fileName);
	/**
	* Destroys the obj reader object.
	*/
	virtual ~ObjPhysicsReader(void);
	/**
	* Instructs the reader to read the file.
	* @return true if the file was read succesfully, false otherwise.
	*/
	bool Read();
	/**
	* Gets the center point.
	*/
	float3 GetCenterPoint();
	/**
	* Gets the height of the object 
	*/
	float GetHeight();
	/**
	* Returns the start of the vector of InputMesh objects.
	* @return the begin of the vector.
	*/
	MIT GetInputMeshBegin(){
		return m_Meshes.begin();
	}
	/**
	* Returns the end of the vector of InputMesh objects.
	* @return the end of the vector.
	*/
	MIT GetInputMeshEnd(){
		return m_Meshes.end();
	}
private:
	/**
	* The filename (.obj file)
	*/
	tstring m_FileName;
	
	/**
	* The file to read
	*/
	tifstream m_Input;
	
	/**
	* Reads the position data.
	* @param line the line to parse.
	*/
	void ReadVertex(tstring&);
	/**
	* Reads the face.
	* @param line the line to parse.
	*/
	void ReadFace(tstring&);
	/**
	* Reads a group definition.
	* @param line the line to parse.
	*/
	void ReadGroup(tstring& line);
	/**
	* Closes the current subgroup.
	*/
	void CloseCurrentSubgroup();
	/**
	* Closes the current mesh.
	*/
	void CloseCurrentMesh();

	vector<tstring> parseHelp;
	vector<tstring> parseHelp2;

	float3& GetVertex(int index);

	vector<float3> m_vertices;

	vector<int>  m_Faces;

	vector<InputMesh*>  m_Meshes;

	InputMesh* m_pCurrentMesh;

	size_t m_VertexOffset;

	/**
	* the maximum vertex position.
	*/
	float3 m_MaxVertex;
	/**
	* The minimum vertex position
	*/
	float3 m_MinVertex;
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	ObjPhysicsReader(const ObjPhysicsReader& t);
	ObjPhysicsReader& operator=(const ObjPhysicsReader& t);
};
