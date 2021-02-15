#pragma once
#include "D3D11.h"
struct Gamesettings;
struct D3D11;
class SceneContext;
class BaseCamera;
class GameObject;
class DebugRenderer;

#pragma warning(disable : 4100) // disable warning C4100: 'some variable': unreferenced formal parameter

class GameScene : public PxSimulationEventCallback
{
public:
	GameScene(const std::wstring& sceneName);
	virtual ~GameScene();
	GameScene(const GameScene& t) = delete;
	GameScene& operator=(const GameScene& t) = delete;

	//GET
	SceneContext* GetSceneContext() const { return m_pSceneContext; }
	PxScene* GetPhysxScene() const { return m_pPhysxScene; }
	std::wstring GetName() const  { return m_Name; }

	//SET
	void SetCamera(BaseCamera* pCamera);
	void EnablePhysxDebugRendering(bool enable) { m_DrawPhysx = enable; }

	//METHODS
	void AddGameObject(  GameObject* pObject);
	void RemoveGameObject(GameObject* pObject);

	//Debug Methods
	static void EnablePhysXFrameStepping(bool enable) { m_PhysXFrameStepping = enable; }
	static void SetPhysicsStepTime(float time) { m_PhysXStepTime = time; }

	friend class SceneManager;

protected:

	//Game specific pure virtual functions
	virtual void Initialize() = 0;
	virtual void Update() {};
	virtual void Draw() const = 0;
	virtual void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {};
	virtual void SceneActivated() {}
	virtual void SceneDeactivated() {}

	//PhysX SimulationEventCallback overridess
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {}
	virtual void onWake(PxActor **actors, PxU32 count) override {}
	virtual void onSleep(PxActor **actors, PxU32 count) override {}
	virtual void onContact(const PxContactPairHeader & pairHeader, const PxContactPair *pairs, PxU32 nbPairs) override {}
	virtual void onTrigger(PxTriggerPair *pairs, PxU32 count) override {}

	std::vector<GameObject*> m_GameObjects;
	bool m_IsInitialized;

	//Physx
	PxScene *m_pPhysxScene;

	//Contexts
	SceneContext* m_pSceneContext;

protected:
	// DEBUG RENDERER
	std::shared_ptr<DebugRenderer> m_pDebugRenderer;
	void SetDebugRenderer(std::shared_ptr<DebugRenderer> pDebugRenderer);
	BaseCamera* m_pDefaultCamera;

private:
	// functions
	void RootInitialize();
	void RootUpdate();
	void RootDraw();
	void RootSceneActivated();
	void RootSceneDeactivated();
	void RootWindowStateChanged(bool active);

	// data
	bool m_DrawPhysx;

	static bool m_PhysXFrameStepping;
	static float m_PhysXStepTime;

	// https://gafferongames.com/post/fix_your_timestep/
	float m_MaxDeltaTime, m_AccumulatedPhysicsTime;
	const float m_PhysicsTimeStep; // fixed physx time step

	std::wstring m_Name;

};

