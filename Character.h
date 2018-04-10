#pragma once
#include <d3dx10.h>
class NxController;
class Camera;
class PickableItem;
#include "DX10ObjMesh.h"

using namespace std;
#include <vector>
/**
* Basic Character implementation.
* The functionality of the character class is the following :
* 1) Add the NxCharacter functionality
* 2) Implement movement
* 3) Implement visualization
* 4) Implement inventory
*/
class Character
{
public:
	/**
	* Creates a new Character.
	*/
	Character(D3DXVECTOR3 startPos);

	/**
	* Deletes the character
	*/
	virtual ~Character(void);

	/**
	* Finds the closest shape under the character.
	* Returns not found if no shape was found.
	*/
	tstring Character::GetContactShape();

	/**
	* Checks if there is a solid object under the character.
	*/
	bool HasContact();

	/**
	* Returns the camera associated with this player character.
	* @return the camera pointer.
	*/
	Camera* GetCamera(){return m_pCamera;}

	/**
	* Moves the character according to the input.
	*/
	void Move(float dt);

	/**
	* Sets the mouse scale for the mouse movement.
	*/
	void SetMouseScale(int scale);

	/**
	* Returns the position of this character.
	*/
	D3DXVECTOR3 GetPosition(){return m_PosW;}

	/**
	* Returns the height of this character.
	*/
	float GetHeight(){return m_Height;}

	/**
	* Returns the radius of this character.
	*/
	float GetRadius(){return m_Radius;}

	/**
	* Add item 
	*/
	void AddToInventory(PickableItem* item){
		m_Inventory.push_back(item);
	}

	/**
	* Check if the player has an item.
	* @param id the item to check for.
	* @return true if the player has the item, false otherwise.
	*/
	bool HasItem(int id);

	void SetCharacterMesh(DX10ObjMesh* inputmesh);

	bool HasCharacterMesh();

private:
	/**
	* The start position for the camera
	*/
	D3DXVECTOR3 m_PosW;

	/**
	* The previous player position.
	*/
	D3DXVECTOR3 m_PreviousPosW;

	/**
	* The camera that is coupled with this character object.
	*/
	Camera* m_pCamera;

	/**
	* The controller object for this character to make it run
	* on the geometry.
	*/
	NxController* m_pController;

	/**
	* The approximate radius of this character
	*/
	float m_Radius;

	/**
	* The approximate height of this character.
	*/
	float m_Height;

	/**
	* A vector with inventory items.
	*/
	vector<PickableItem*> m_Inventory;


	// Camera speed.
	float m_Speed;
	//Running multiplies speed by factor m_RunningMultiplicationFactor
	float m_RunningMultiplicationFactor;
	//mousemovement is divided by this scalefactor
	//big makes mouse move slow
	float m_MouseScale;
	DX10ObjMesh* m_pCharacterMesh;
	bool m_HitState;

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	Character(const Character& t);
	Character& operator=(const Character& t);
};
