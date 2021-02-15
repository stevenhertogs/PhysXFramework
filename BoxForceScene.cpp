#include "stdafx.h"
#include "BoxForceScene.h"
#include "CubePosColorNorm.h"

using namespace physx;

BoxForceScene::BoxForceScene() : GameScene(L"BoxForceScene")
{
}

void BoxForceScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxRigidBody* const pRigidBody = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	XMFLOAT3 dimensions = XMFLOAT3(2.f, 2.f, 2.f);
	PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
	PxMaterial* const pBoxMaterial = pxPhysX->createMaterial(0.2f, 1.f, 0.5f);
	pRigidBody->createShape(boxGeometry, *pBoxMaterial);
	m_pCube = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z);
	m_pCube->AttachRigidActor(pRigidBody);
	AddGameObject(m_pCube);

	AddGroundPlane();
}

void BoxForceScene::Update()
{
	if (m_pSceneContext->GetInput()->IsActionTriggered((int)InputActionEnum::reset))
	{
		ResetScene();
	}

	const float force = 30;
	//E
	if (GetAsyncKeyState(0x45) & 0x8000)
	{

		static_cast<PxRigidBody*>(m_pCube->GetRigidActor())->addForce({ force,0,0 });
	}
	//Q
	if (GetAsyncKeyState(0x51) & 0x8000)
	{

		static_cast<PxRigidBody*>(m_pCube->GetRigidActor())->addForce({ -force,0,0 });
	}
	m_pCube->Rotate(0,0,0);
}

void BoxForceScene::Draw() const
{
}

void BoxForceScene::SceneActivated()
{
	ResetScene();
}

void BoxForceScene::ResetScene()
{

}

void BoxForceScene::AddGroundPlane()
{
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxTransform const planeTransform = PxTransform(PxQuat(PxPiDivTwo, PxVec3(0, 0, 1)));
	PxRigidStatic* const pPlaneRigidBody = pxPhysX->createRigidStatic(planeTransform);
	PxMaterial* const pPlaneMaterial = pxPhysX->createMaterial(0.f, 0.f, 1.f);
	pPlaneRigidBody->createShape(PxPlaneGeometry(), *pPlaneMaterial);
	m_pPhysxScene->addActor(*pPlaneRigidBody);
}
