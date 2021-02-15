#include "stdafx.h"
#include "PhysXTestScene.h"

#include "PhysxManager.h"
#include "CubePosColorNorm.h"
#include "Logger.h"

PhysXTestScene::PhysXTestScene() : GameScene(L"PhysXTestScene")
{
}

void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	using namespace physx;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxRigidBody* const pRigidBody = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	XMFLOAT3 dimensions = XMFLOAT3(2.f, 1.f, 0.5f);
	PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
	PxMaterial* const pBoxMaterial = pxPhysX->createMaterial(0.2f, 1.f, 0.5f);
	pRigidBody->createShape(boxGeometry, *pBoxMaterial);
	m_pCube = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z);
	m_pCube->AttachRigidActor(pRigidBody);
	m_pCube->Translate(0.f, 5.f, 0.f);
	m_pCube->Rotate(0.1f, 0.2f, 0.3f);
	AddGameObject(m_pCube);

	AddGroundPlane();

	//Input
	m_pSceneContext->GetInput()->AddInputAction(InputAction((int)InputActionEnum::reset, InputTriggerState::pressed, 'R'));

}

void PhysXTestScene::Update()
{
	if (m_pSceneContext->GetInput()->IsActionTriggered((int)InputActionEnum::reset))
	{
		ResetScene();
	}
}

void PhysXTestScene::Draw() const
{
}

void PhysXTestScene::SceneActivated()
{
	ResetScene();
}

void PhysXTestScene::ResetScene()
{
	Logger::GetInstance()->LogInfo(L"Resetting my scene...");
	m_pCube->Translate(0.f, 5.f, 0.f);
	m_pCube->Rotate(0.1f, 0.2f, 0.3f);
	static_cast<PxRigidDynamic*>(m_pCube->GetRigidActor())->setAngularVelocity(PxVec3(0.f, 0.f, 0.f));
	static_cast<PxRigidDynamic*>(m_pCube->GetRigidActor())->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));
}

void PhysXTestScene::AddGroundPlane()
{
	using namespace physx;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxTransform const planeTransform = PxTransform(PxQuat(PxPiDivTwo, PxVec3(0, 0, 1)));
	PxRigidStatic* const pPlaneRigidBody = pxPhysX->createRigidStatic(planeTransform);
	PxMaterial* const pPlaneMaterial = pxPhysX->createMaterial(0.5f, 0.5f, 0.5f);
	pPlaneRigidBody->createShape(PxPlaneGeometry(), *pPlaneMaterial);
	m_pPhysxScene->addActor(*pPlaneRigidBody);
}
