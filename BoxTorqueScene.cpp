#include "stdafx.h"
#include "BoxTorqueScene.h"
#include "CubePosColorNorm.h"

using namespace physx;

BoxTorqueScene::BoxTorqueScene() : GameScene(L"BoxTorqueScene")
{
}

void BoxTorqueScene::Initialize()
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

void BoxTorqueScene::Update()
{
	if (m_pSceneContext->GetInput()->IsActionTriggered((int)InputActionEnum::reset))
	{
		ResetScene();
	}

	const float torque = 30;
	//E
	if (GetAsyncKeyState(0x45) & 0x8000)
	{
		static_cast<PxRigidBody*>(m_pCube->GetRigidActor())->addTorque({ 0,torque,0 });
	}
	//Q
	if (GetAsyncKeyState(0x51) & 0x8000)
	{

		static_cast<PxRigidBody*>(m_pCube->GetRigidActor())->addTorque({ 0,-torque,0 });
	}
}

void BoxTorqueScene::Draw() const
{
}

void BoxTorqueScene::SceneActivated()
{
	ResetScene();
}

void BoxTorqueScene::ResetScene()
{

}

void BoxTorqueScene::AddGroundPlane()
{
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxTransform const planeTransform = PxTransform(PxQuat(PxPiDivTwo, PxVec3(0, 0, 1)));
	PxRigidStatic* const pPlaneRigidBody = pxPhysX->createRigidStatic(planeTransform);
	PxMaterial* const pPlaneMaterial = pxPhysX->createMaterial(0.f, 0.f, 1.f);
	pPlaneRigidBody->createShape(PxPlaneGeometry(), *pPlaneMaterial);
	m_pPhysxScene->addActor(*pPlaneRigidBody);
}