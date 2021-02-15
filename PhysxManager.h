#pragma once

#include "Singleton.h"

class PhysxAllocator;
class PhysxErrorCallback;
class GameScene;

#pragma warning(disable : 4100) // disable warning C4100: 'some variable': unreferenced formal parameter

class PhysxManager : public Singleton<PhysxManager>, public PxVisualDebuggerConnectionHandler
{
public:
	~PhysxManager();
	PhysxManager( const PhysxManager &obj) = delete;	
	PhysxManager& operator=( const PhysxManager& obj ) = delete;

	//GETTERS
	PxPhysics* GetPhysics() const { return m_pPhysics; }
	
	//Methods
	PxScene* CreateScene(GameScene* pScene);

	//PVD
	virtual void onPvdSendClassDescriptions(PxVisualDebuggerConnection& connection) {};
	virtual void onPvdConnected(PxVisualDebuggerConnection& connection);
	virtual void onPvdDisconnected(PxVisualDebuggerConnection& connection) {};

	bool ToggleVisualDebuggerConnection();

protected:
	PhysxManager();
	friend Singleton<PhysxManager>; // allow Singleton to call the constructor

private:

	void Init();
	void CleanUp();

	PhysxAllocator* m_pDefaultAllocator;
	PhysxErrorCallback* m_pDefaultErrorCallback;
	PxFoundation* m_pFoundation;
	PxProfileZoneManager* m_pProfileZoneManager;
	PxPhysics* m_pPhysics;
	PxDefaultCpuDispatcher* m_pDefaultCpuDispatcher;

};

