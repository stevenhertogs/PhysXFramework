#include "stdafx.h"
#include "GameScene.h"
#include "SceneContext.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "PhysxHelpers.h"
#include "Logger.h"
#include "DebugRenderer.h"

bool GameScene::m_PhysXFrameStepping = false;
float GameScene::m_PhysXStepTime{ 1.0f / 60 };


GameScene::GameScene(const std::wstring& sceneName)
	: m_Name{sceneName}
	, m_IsInitialized{false}
	, m_pSceneContext{}	
	, m_pDefaultCamera{nullptr}
	, m_pPhysxScene{nullptr}
	, m_DrawPhysx{false}
	, m_MaxDeltaTime{ 0.1f }
	, m_AccumulatedPhysicsTime{}
	, m_PhysicsTimeStep{ 1.0f / 60 }
{

}

GameScene::~GameScene()
{
	for (GameObject* pObject : m_GameObjects)
	{
		SafeDelete(pObject);
	}

	SafeDelete(m_pSceneContext);
	SafeDelete(m_pDefaultCamera);
}

void GameScene::SetCamera(BaseCamera* pCamera)
{
	if (!pCamera)
		pCamera = m_pDefaultCamera;

	m_pSceneContext->SetCamera(pCamera);
}

void GameScene::AddGameObject(  GameObject* pObject)
{
	auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), pObject);
	if (it == m_GameObjects.end())
	{
		m_GameObjects.push_back(pObject);
		pObject->m_pScene = this;
		pObject->m_pParent = nullptr;

		if (m_IsInitialized)
			pObject->RootInitialize();
	}
}

void GameScene::RemoveGameObject(GameObject* pObject)
{
	auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), pObject);
	

	if (it != m_GameObjects.end())
	{
		m_GameObjects.erase(it);
		pObject->m_pScene = nullptr;
		pObject->m_pParent = nullptr;

		//TODO: check if pObject needs to be deleted
		SafeDelete(*it);
	}
}

void GameScene::SetDebugRenderer(std::shared_ptr<DebugRenderer> pDebugRenderer)
{
	m_pDebugRenderer = pDebugRenderer;
}

void GameScene::RootInitialize()
{
	if (m_IsInitialized)
		return;

	//Initialize Context
	m_pSceneContext = new SceneContext();
	m_pSceneContext->GetGameTime()->Reset();
	m_pSceneContext->GetGameTime()->Stop();
	m_pSceneContext->GetInput()->Initialize();

	//Initialize PhysX Scene
	m_pPhysxScene = PhysxManager::GetInstance()->CreateScene(this);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
	m_pPhysxScene->setSimulationEventCallback(this);

	//Default Camera
	m_pDefaultCamera = new FreeCamera();
	m_pSceneContext->SetCamera(m_pDefaultCamera);

	Initialize();

	for (GameObject* pObject : m_GameObjects)
	{
		pObject->RootInitialize();
	}

	m_IsInitialized = true;
}

void GameScene::RootUpdate()
{
	m_pSceneContext->GetGameTime()->Update();
	m_pSceneContext->GetInput()->Update();

	if (m_pSceneContext->GetCamera())
		m_pSceneContext->GetCamera()->Update(*m_pSceneContext);

	Update(); //scenecontext is member, no param here

	for (GameObject* pObject : m_GameObjects)
	{
		pObject->RootUpdate(*m_pSceneContext);
	}

	for (GameObject* pObject : m_GameObjects)
	{
		pObject->CalculateWorldMatrix(DirectX::XMMatrixIdentity()); //or inside GameObject itself??
	}

	if (m_pSceneContext->GetGameTime()->IsRunning())
	{
		// for slow motion physics debugging
		if (m_PhysXFrameStepping)
		{
			if (m_PhysXStepTime > 0.f)
			{
				m_pPhysxScene->simulate(m_PhysXStepTime);
				m_pPhysxScene->fetchResults(true);
				m_PhysXStepTime = 0.f;
			}
		}
		else // normal running situation
		{
			m_AccumulatedPhysicsTime += m_pSceneContext->GetGameTime()->GetElapsed();
			while (m_AccumulatedPhysicsTime > m_PhysicsTimeStep)
			{
				// start the physics calculations
				m_pPhysxScene->simulate(m_PhysicsTimeStep);

				// wait for physx to finish
				m_pPhysxScene->fetchResults(true);

				// consume one timestep
				m_AccumulatedPhysicsTime -= m_PhysicsTimeStep;
			}
		}
	}


	//Send Camera to PVD
	if (m_pPhysxScene->getPhysics().getPvdConnectionManager())
	{
		auto cam = m_pSceneContext->GetCamera();
		m_pPhysxScene->getPhysics().getPvdConnectionManager()->setCamera("SceneCam", ToPxVec3(cam->GetPosition()), ToPxVec3(cam->GetUp()), ToPxVec3(cam->GetTarget()));
	}
}

void GameScene::RootDraw()
{
	Draw();

	for (GameObject* pObject : m_GameObjects)
	{
		pObject->RootDraw(*m_pSceneContext);
	}

	//Physx Debug Rendering
	if (m_DrawPhysx)
	{
		m_pDebugRenderer->DrawPhysX(m_pPhysxScene);
		m_pDebugRenderer->DrawGrid(*m_pSceneContext);
	}
}

void GameScene::RootWindowStateChanged(bool active)
{
	//TIMER
	if (active)m_pSceneContext->GetGameTime()->Start();
	else m_pSceneContext->GetGameTime()->Stop();
}

void GameScene::RootSceneActivated()
{
	//Stop Timer
	m_pSceneContext->GetGameTime()->Start();

	SceneActivated();
}

void GameScene::RootSceneDeactivated()
{
	//Start Timer
	m_pSceneContext->GetGameTime()->Stop();

	SceneDeactivated();
}
