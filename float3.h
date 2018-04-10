#pragma once

/**
* This class stores the  x, y and z component of a float 3 object.
* @author Koen Samyn.
*/ 
class float3
{
public:
	float x,y,z;
	float3(void);
	float3(float x, float y, float z) : x(x), y(y), z(z) {}
	~float3(void);

private:
	
};
