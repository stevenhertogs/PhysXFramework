#include "stdafx.h"
#include "WallDemolitionScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"

using namespace physx;
using namespace DirectX;

WallDemolitionScene::WallDemolitionScene() : GameScene(L"WallDemolitionScene")
{
}

void WallDemolitionScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();

	const float radius = 2;
	const int slices = 50;
	const int stacks = 50;
	XMFLOAT4 color = { 0.7f,0.7f,0.7f,1 };

	m_pBall = new SpherePosColorNorm(radius, slices, stacks, color);
	PxRigidBody* const pRigidBody_01 = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	PxSphereGeometry sphereGeometry_01 = PxSphereGeometry(radius);
	PxMaterial* const pSphereMaterial_01 = pxPhysX->createMaterial(0.3f, 0.3f, 0.2f);
	pRigidBody_01->createShape(sphereGeometry_01, *pSphereMaterial_01);
	m_pBall->AttachRigidActor(pRigidBody_01);

	m_pBall->Translate(m_BallPos.x, m_BallPos.y, m_BallPos.z);
	AddGameObject(m_pBall);

	AddWall();

	AddGroundPlane();

	m_pSceneContext->GetInput()->AddInputAction(InputAction((int)InputActionEnum::reset, InputTriggerState::pressed, 'R'));
}

void WallDemolitionScene::Update()
{
	if (m_pSceneContext->GetInput()->IsActionTriggered((int)InputActionEnum::reset))
	{
		ResetScene();
	}

	float torque = 5.f;
	float force = 15.f;


	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		const XMFLOAT3 cameraForward = m_pDefaultCamera->GetForward();
		XMFLOAT3 torqueVec{};
		XMStoreFloat3(&torqueVec, XMLoadFloat3(&cameraForward) * -torque);

		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addForce({ force,0,0});
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addTorque({ torqueVec.x,torqueVec.y,torqueVec.z });
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		const XMFLOAT3 cameraForward = m_pDefaultCamera->GetForward();
		XMFLOAT3 torqueVec{};
		XMStoreFloat3(&torqueVec, XMLoadFloat3(&cameraForward) * torque);

		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addForce({ -force,0,0 });
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addTorque({ torqueVec.x,torqueVec.y,torqueVec.z });
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		const XMFLOAT3 cameraRight = m_pDefaultCamera->GetRight();
		XMFLOAT3 torqueVec{};
		XMStoreFloat3(&torqueVec, XMLoadFloat3(&cameraRight) * torque);

		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addForce({ 0,0,force });
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addTorque({ torqueVec.x,torqueVec.y,torqueVec.z });
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		const XMFLOAT3 cameraRight = m_pDefaultCamera->GetRight();
		XMFLOAT3 torqueVec{};
		XMStoreFloat3(&torqueVec, XMLoadFloat3(&cameraRight) * -torque);
		
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addForce({ 0,0,-force });
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addTorque({ torqueVec.x,torqueVec.y,torqueVec.z });
	}

	
	
}

void WallDemolitionScene::Draw() const
{
}

void WallDemolitionScene::SceneActivated()
{
	ResetScene();
}

void WallDemolitionScene::ResetScene()
{
	const float bottom = 5;
	const float cubeSize = 2;;
	for (size_t i = 0; i < 25; ++i)
	{
		float yRot = rand() / float(RAND_MAX) * PxPi;
		m_pCubes[i]->Rotate(0, yRot, 0);
		m_pCubes[i]->Translate((i / 5) * (cubeSize + 1), bottom + cubeSize * (i % 5), 0);
	}

	m_pBall->Translate(m_BallPos.x,m_BallPos.y,m_BallPos.z);
	m_pBall->Rotate(0, 0, 0);
}

void WallDemolitionScene::AddGroundPlane() 
{
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxTransform const planeTransform = PxTransform(PxQuat(PxPiDivTwo, PxVec3(0, 0, 1)));
	PxRigidStatic* const pPlaneRigidBody = pxPhysX->createRigidStatic(planeTransform);
	PxMaterial* const pPlaneMaterial = pxPhysX->createMaterial(0.f, 0.f, 1.f);
	pPlaneRigidBody->createShape(PxPlaneGeometry(), *pPlaneMaterial);
	m_pPhysxScene->addActor(*pPlaneRigidBody);
}

void WallDemolitionScene::AddWall()
{
	const float bottom = 5;
	const float cubeSize = 2;;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	m_pCubes.resize(25);
	for(size_t i = 0;i<25;++i)
	{
		PxRigidBody* const pRigidBody = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
		XMFLOAT3 dimensions = XMFLOAT3(2.f, 2.f, 2.f);
		PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
		PxMaterial* const pBoxMaterial = pxPhysX->createMaterial(0.5f, 0.5f, 0.3f);
		pRigidBody->createShape(boxGeometry, *pBoxMaterial);
		
		m_pCubes[i] = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z);
		m_pCubes[i]->AttachRigidActor(pRigidBody);
		float yRot = rand() / float(RAND_MAX) * PxPi;
		AddGameObject(m_pCubes[i]);
		m_pCubes[i]->Rotate(0, yRot, 0);
		m_pCubes[i]->Translate((i / 5) * (cubeSize + 1), bottom + cubeSize * (i % 5), 0);
	}

}
