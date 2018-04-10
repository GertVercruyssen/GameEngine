//=============================================================================
// Graph3DObject.h by Bart Uyttenhove 2008 
//=============================================================================
#pragma once
#include "d3dUtil.h"
#include "GraphBase.h"


/**
* The Graph3DObject class defines the basic functionality of a drawable 3D object. 
* It has no children.
* A D3Dobject can be scaled, rotated & translated. The resulting transformation
* matrix is a member of this class.
*/
class Graph3DObject: public GraphBase
{
public:
	/**
	* Creates a new Graph3DObject. Should not be used, this class is meant to be subclassed.
	*/
	Graph3DObject(void);
	/**
	* Destroys the Graph3DObject.
	*/
	virtual ~Graph3DObject(void);

	/**
	* Draw the Graph3DObject in the scene.
	*/
	virtual void Draw() const {};
	/**
	* Draw the Graph3DObject in the scene with the technique.
	* @param technique the technique to use.
	*/
	virtual void Draw(const ID3D10EffectTechnique* pTechnique) const {};
	/**
	* Called before the destructor is called.
	*/
	//virtual void Cleanup(){};
	/**
	* Call when the d3d object is constructed.
	*/
	virtual bool Init(){return true;};
	/**
	* Checks if a ray as defined by the RayPost & RayDir intersects with this 
	* object. The default implementation in this class returns always false.
	* @param RayPos the start position for the ray.
	* @param RayDir the direction for the ray.
	* @return always false.
	*/
	virtual bool HitTest(const D3DXVECTOR3* RayPos,const D3DXVECTOR3* RayDir)const{return false;}
	/**
	* Returns the hit distance of the last hittest.
	* @return the distance, always 0 in this implementation.
	*/
	virtual float GetHitDistance() const {return 0;};
	/**
	* Returns the location of the last hittest.
	* @return a D3DXVECTOR3 , always 0 in this implementation.
	*/
	virtual D3DXVECTOR3 GetHitSpot() const {return D3DXVECTOR3();};
	/**
	* Show the bounding box.
	* @param b pass true to this parameter to show the bounding box, false to hide the bounding box.
	*/
	virtual void ShowBoundingBox(bool b){};
	/**
	* Sets the view matrix for all the objects.
	* @param View the View matrix
	*/
	static void SetView(const D3DXMATRIX * View);//static omdat classe var worden aangepast
	/**
	* Sets the projection matrix for all the objects.
	* @param Projection the Projection matrix.
	*/
	static void SetProjection(const D3DXMATRIX * Projection);//static omdat classe var worden aangepast
	/**
	* Allow or disallow this object to cast shadows.
	* @param castShadow pass true to this parameter if you want this object to cast shadows, false
	* if this object does not cast a shadow.
	*/
	void SetCastShadow(bool castShadow){m_CastShadow = castShadow;}
	/**
	* Returns the value of the castShadow property.
	* @return true if this object casts a shadow, false otherwise.
	*/
	bool IsCastShadow()const {return m_CastShadow;}
	/**
		
	* Enable or disable this object to receive shadows.
	* @param receiveShadow pass true to this parameter if you want this object to receive shadows, false
	* if you do not want this object to receive shadows.
	*/
	void SetReceiveShadow(bool receiveShadow){m_ReceiveShadow = receiveShadow;}
	/**
	* Returns the value of the receiveShadow property.
	* @return true if this object can receive shadows, false otherwise.
	*/
	bool IsReceiveShadow()const  {return m_ReceiveShadow;}

	void SetCameraPosition(D3DXVECTOR3 position)const {m_CameraPos = position;}



protected:
	// Loads and compiles the effect and gets the technique
	virtual void CreateEffect(const tstring& EffectFilename);

	ID3D10Effect*				m_pEffect;
	ID3D10EffectTechnique*  m_pDefaultTechnique;

	//static:  er is één gemeenschappelijk view member voor alle klasses en objecten die hiervan zijn afgeleid
	static D3DXMATRIX m_View;
	static D3DXMATRIX m_Projection;
	static D3DXVECTOR3 m_CameraPos;


	/**
	* Indicates if this object casts a shadow or not.
	*/
	bool m_CastShadow;
	/**
	* Indicates if this object receives a shadow or not.
	*/
	bool m_ReceiveShadow;



private:
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Graph3DObject(const Graph3DObject& t);
	Graph3DObject& operator=(const Graph3DObject& t);
};
