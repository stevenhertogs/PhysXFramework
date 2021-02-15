#include "stdafx.h"
#include "BouncingBallsScene.h"

#include "SpherePosColorNorm.h"
#include "Logger.h"
#include "FreeCamera.h"

BouncingBallsScene::BouncingBallsScene() : GameScene(L"BouncingBallsScene")
{
}

BouncingBallsScene::~BouncingBallsScene()
{
	delete m_pCamera;
}

void BouncingBallsScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	using namespace physx;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	
	const float radius = 4;
	const int slices = 50;
	const int stacks = 50;
	XMFLOAT4 color = { 0.7f,0.7f,0.7f,1 };

	//first ball
	m_pBall_01 = new SpherePosColorNorm(radius, slices, stacks, color);
	PxRigidBody* const pRigidBody_01 = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	PxSphereGeometry sphereGeometry_01 = PxSphereGeometry(radius);
	PxMaterial* const pSphereMaterial_01 = pxPhysX->createMaterial(0.f, 0.f, 0.f);
	pRigidBody_01->createShape(sphereGeometry_01, *pSphereMaterial_01);
	m_pBall_01->AttachRigidActor(pRigidBody_01);

	m_pBall_01->Translate(m_BallPos_01.x,m_BallPos_01.y,m_BallPos_01.z);
	AddGameObject(m_pBall_01);

	//second ball
	m_pBall_02 = new SpherePosColorNorm(radius, slices, stacks, color);
	PxRigidBody* const pRigidBody_02 = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	PxSphereGeometry sphereGeometry_02 = PxSphereGeometry(radius);
	PxMaterial* const pSphereMaterial_02 = pxPhysX->createMaterial(0.f, 0.f, 0.5f);
	pRigidBody_02->createShape(sphereGeometry_02, *pSphereMaterial_02);
	m_pBall_02->AttachRigidActor(pRigidBody_02);

	m_pBall_02->Translate(m_BallPos_02.x, m_BallPos_02.y, m_BallPos_02.z);
	AddGameObject(m_pBall_02);

	//third ball
	m_pBall_03 = new SpherePosColorNorm(radius, slices, stacks, color);
	PxRigidBody* const pRigidBody_03 = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	PxSphereGeometry sphereGeometry_03 = PxSphereGeometry(radius);
	PxMaterial* const pSphereMaterial_03 = pxPhysX->createMaterial(0.f, 0.f, 1.f);
	pRigidBody_03->createShape(sphereGeometry_03, *pSphereMaterial_03);
	m_pBall_03->AttachRigidActor(pRigidBody_03);

	m_pBall_03->Translate(m_BallPos_03.x, m_BallPos_03.y, m_BallPos_03.z);
	AddGameObject(m_pBall_03);

	//ground plane
	AddGroundPlane();

	//camera
	m_pCamera = new FreeCamera();
	m_pSceneContext->SetCamera(m_pCamera);
	m_pCamera->SetPosition(m_CameraPos);
}

void BouncingBallsScene::Update()
{
	if (m_pSceneContext->GetInput()->IsActionTriggered((int)InputActionEnum::reset))
	{
		ResetScene();
	}
}

void BouncingBallsScene::Draw() const
{
}

void BouncingBallsScene::SceneActivated()
{
	ResetScene();
}

void BouncingBallsScene::ResetScene()
{
	Logger::GetInstance()->LogInfo(L"Resetting my scene...");
	m_pBall_01->Translate(m_BallPos_01.x, m_BallPos_01.y, m_BallPos_01.z);
	m_pBall_02->Translate(m_BallPos_02.x, m_BallPos_02.y, m_BallPos_02.z);
	m_pBall_03->Translate(m_BallPos_03.x, m_BallPos_03.y, m_BallPos_03.z);

	static_cast<PxRigidDynamic*>(m_pBall_01->GetRigidActor())->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));
	static_cast<PxRigidDynamic*>(m_pBall_02->GetRigidActor())->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));
	static_cast<PxRigidDynamic*>(m_pBall_03->GetRigidActor())->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));

	m_pCamera->SetPosition(m_CameraPos);

}

void BouncingBallsScene::AddGroundPlane()
{
	using namespace physx;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxTransform const planeTransform = PxTransform(PxQuat(PxPiDivTwo, PxVec3(0, 0, 1)));
	PxRigidStatic* const pPlaneRigidBody = pxPhysX->createRigidStatic(planeTransform);
	PxMaterial* const pPlaneMaterial = pxPhysX->createMaterial(0.f, 0.f, 1.f);
	pPlaneRigidBody->createShape(PxPlaneGeometry(), *pPlaneMaterial);
	m_pPhysxScene->addActor(*pPlaneRigidBody);
}
