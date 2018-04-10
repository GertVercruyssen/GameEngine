struct Light{
	float3 pos;
	float3 dir;
	float3 att;
	float4 color;
	float spotPower;
	float range;
};

struct SurfaceInfo
{
	float3 pos;
	float3 normal;
	float4 diffuse;
	float4 spec;
};

float3 PointLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = float3(0.0f,0.0f,0.0f);
	float3 lightVec =L.pos-v.pos; 
	float d = length(lightVec);
	
	lightVec /= d;
	if  (d > L.range){
		return litColor;
	
	}
	float diffuseFactor = dot(-lightVec, v.normal );
	[branch]
	if  (diffuseFactor > 0.0f) 
	{
		float specPower = max( v.spec.a, 1.0f);
		float3 toEye = normalize(eyePos-v.pos);
		float3 R = reflect(-lightVec,v.normal);
		float specFactor = pow(max(dot(R,toEye),0.0f),specPower*100);
		litColor += diffuseFactor * v.diffuse * L.color;
		litColor += specFactor * v.spec * L.color;
	}
	return litColor / dot(L.att,float3(1.0f,d,d*d));
}

float3 SpotLight(SurfaceInfo v, Light L, float3 eyePos)
{
	float3 litColor = PointLight(v,L,eyePos);
	float3 lightVec = normalize(L.pos - v.pos);
	float s = pow ( max ( dot (-lightVec,L.dir),0.0f),L.spotPower);
	return s*litColor;
}