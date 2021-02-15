#include "stdafx.h"
#include "PhysXMeshTestScene.h"
#include "PhysxManager.h"
#include "MeshObjectTex.h"
#include "ContentManager.h"

using namespace physx;

PhysXMeshTestScene::PhysXMeshTestScene() : GameScene(L"PhysXMeshTestScene")
{
}

void PhysXMeshTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	
	
	auto& physX = m_pPhysxScene->getPhysics();
	//Material
	auto defaultMaterial = physX.createMaterial(.5f, .5f, .6f);

	AddGroundPlane();


	//Load PhysX Meshes
	auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Meshes/Chair.ovpt");
	auto pConvesMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
	

	//Chair triangle Mesh
	auto pChairTriangleMesh = new MeshObjectTex(L"Meshes/Chair.ovm", L"Textures/Chair_Dark.dds");
	m_pKinematicActor = physX.createRigidDynamic(PxTransform::createIdentity());
	m_pKinematicActor->setRigidDynamicFlag(PxRigidBodyFlag::eKINEMATIC, true);
	m_pKinematicActor->createShape(PxTriangleMeshGeometry(pTriangleMesh), *defaultMaterial);
	pChairTriangleMesh->AttachRigidActor(m_pKinematicActor);
	m_KineMaticPosition = { 0.f,1.f,0.f };
	pChairTriangleMesh->Translate(m_KineMaticPosition.x, m_KineMaticPosition.y, m_KineMaticPosition.z);
	AddGameObject(pChairTriangleMesh);

	//Chair Convex Mesh
	auto pChairConvexMesh = new MeshObjectTex(L"Meshes/Chair.ovm", L"Textures/Chair_Dark.dds");
	auto pConvexActor = physX.createRigidDynamic(PxTransform::createIdentity());
	pConvexActor->createShape(PxConvexMeshGeometry(pConvesMesh), *defaultMaterial);
	PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f);
	pChairConvexMesh->AttachRigidActor(pConvexActor);
	pChairConvexMesh->Translate(1.f, 12.f, 0.f);
	AddGameObject(pChairConvexMesh);
	
	//Input Actions
	m_pSceneContext->GetInput()->AddInputAction(InputAction(int(ActionKeys::left), InputTriggerState::down, VK_LEFT));
	m_pSceneContext->GetInput()->AddInputAction(InputAction(int(ActionKeys::right), InputTriggerState::down, VK_RIGHT));
	m_pSceneContext->GetInput()->AddInputAction(InputAction(int(ActionKeys::back), InputTriggerState::down, VK_UP));
	m_pSceneContext->GetInput()->AddInputAction(InputAction(int(ActionKeys::forward), InputTriggerState::down, VK_DOWN));
}

void PhysXMeshTestScene::Update()
{
	float const speed = 5.f;
	float const deltaTime = m_pSceneContext->GetGameTime()->GetElapsed();
	
	if(m_pSceneContext->GetInput()->IsActionTriggered(int(ActionKeys::left)))
	{
		m_KineMaticPosition.x -= speed * deltaTime;
		m_pKinematicActor->setKinematicTarget(PxTransform(m_KineMaticPosition));
	}

	if (m_pSceneContext->GetInput()->IsActionTriggered(int(ActionKeys::right)))
	{
		m_KineMaticPosition.x += speed * deltaTime;
		m_pKinematicActor->setKinematicTarget(PxTransform(m_KineMaticPosition));
	}

	if (m_pSceneContext->GetInput()->IsActionTriggered(int(ActionKeys::forward)))
	{
		m_KineMaticPosition.z += speed * deltaTime;
		m_pKinematicActor->setKinematicTarget(PxTransform(m_KineMaticPosition));
	}

	if (m_pSceneContext->GetInput()->IsActionTriggered(int(ActionKeys::back)))
	{
		m_KineMaticPosition.z -= speed * deltaTime;
		m_pKinematicActor->setKinematicTarget(PxTransform(m_KineMaticPosition));
	}
}

void PhysXMeshTestScene::Draw() const
{
}

void PhysXMeshTestScene::SceneActivated()
{
}

void PhysXMeshTestScene::SceneDeactivated()
{
}

void PhysXMeshTestScene::AddGroundPlane()
{
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxTransform const planeTransform = PxTransform(PxQuat(PxPiDivTwo, PxVec3(0, 0, 1)));
	PxRigidStatic* const pPlaneRigidBody = pxPhysX->createRigidStatic(planeTransform);
	PxMaterial* const pPlaneMaterial = pxPhysX->createMaterial(0.f, 0.f, 1.f);
	pPlaneRigidBody->createShape(PxPlaneGeometry(), *pPlaneMaterial);
	m_pPhysxScene->addActor(*pPlaneRigidBody);
}
