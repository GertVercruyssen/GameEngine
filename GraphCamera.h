//=============================================================================
// GraphCamera.h by Frank Luna (C) 2004 All Rights Reserved.
// Adapted for GSP framework by Bart Uyttenhove 
//=============================================================================

#pragma once

#include <d3dx10.h>
#include "GraphNode.h"
#include "Camera.h"

class GraphCamera : public GraphNode, public Camera
{
public:
	// By default, the camera starts out with its basis vectors 
	// aligned with the world space axes, and its origin positioned
	// at the world space origin.
	GraphCamera();
	virtual ~GraphCamera(void);	

	virtual void Tick(float dt);

protected:
	virtual void BuildViewMatrix();

private:

	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	GraphCamera(const GraphCamera& t);
	GraphCamera& operator=(const GraphCamera& t);
};
