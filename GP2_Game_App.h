#include <vector>
#include "abstractdxgame.h"
#include "FullscreenQuad.h"
#include "DrawableTex2D.h"

class GraphNode;
class GraphCamera;
class Character;
#ifdef _DEBUG
	class PhysicsDebugRenderer;
#endif

class GP2_Game_App : public AbstractDXGame
{
public:

#ifdef _DEBUG
	enum RenderState { Normal, DebugPhysics, MixedMode };
#endif

	static GP2_Game_App* GetSingleton();
	~GP2_Game_App();

	virtual bool CheckDeviceCaps(){return true;}
	virtual void UpdateScene(float dt);
	virtual void DrawScene();
	virtual void KeyPressed(WPARAM cKey);
	virtual void OnResize();
	virtual bool Init();
	virtual void Paint(float dTime);
	virtual void SetWindowParams();
	virtual Character* GetCharacter();

private:
	GP2_Game_App();
	static GP2_Game_App* m_pGP2_Game_App;
	void SetVPMatricesFromActiveCamera();
	FullscreenQuad* m_PostQuad;
	DrawableTex2D* m_TexTarget;

	GraphNode *m_pBaseNode;
	Character *m_pPlayer;
	GraphCamera *m_pGraphCamera;

	float m_Angle;
	int m_iCameraNumber;
	int m_iNumCameras;

#ifdef _DEBUG
	RenderState m_renderstate;
	PhysicsDebugRenderer *m_pPhysicsDebugRenderer;
#endif
	// -------------------------
	// Disabling default copy constructor and default assignment operator.
	// If you get a linker error from one of these functions, your class is internally trying to use them. This is
	// an error in your class, these declarations are deliberately made without implementation because they should never be used.
	// -------------------------
	GP2_Game_App(const GP2_Game_App& t);
	GP2_Game_App& operator=(const GP2_Game_App& t);
};
