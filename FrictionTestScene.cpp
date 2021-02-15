#include "stdafx.h"
#include "FrictionTestScene.h"

#include "CubePosColorNorm.h"
#include "FreeCamera.h"

FrictionTestScene::FrictionTestScene() : GameScene(L"FrictionTestScene")
{
}

FrictionTestScene::~FrictionTestScene()
{
	delete m_pCamera;
}

void FrictionTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	using namespace physx;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();

	AddPlatforms();
	AddGroundPlane();
	AddCubes();

	m_pCamera = new FreeCamera();
	m_pCamera->SetPosition(m_CameraPos);
	m_pSceneContext->SetCamera(m_pCamera);
}

void FrictionTestScene::Update()
{
}

void FrictionTestScene::Draw() const
{
}

void FrictionTestScene::SceneActivated()
{
}

void FrictionTestScene::ResetScene()
{
	m_pCamera->SetPosition(m_CameraPos);
}

void FrictionTestScene::AddGroundPlane()
{
	using namespace physx;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxTransform const planeTransform = PxTransform(PxQuat(PxPiDivTwo, PxVec3(0, 0, 1)));
	PxRigidStatic* const pPlaneRigidBody = pxPhysX->createRigidStatic(planeTransform);
	PxMaterial* const pPlaneMaterial = pxPhysX->createMaterial(0.f, 0.f, 1.f);
	pPlaneRigidBody->createShape(PxPlaneGeometry(), *pPlaneMaterial);
	m_pPhysxScene->addActor(*pPlaneRigidBody);
}

void FrictionTestScene::AddPlatforms()
{
	const int length = 40;
	const int width = 20;
	const int height = 10;
	using namespace physx;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();
	PxMaterial* const pPlankMaterial = pxPhysX->createMaterial(0.f, 0.f, 0.f);
	PxBoxGeometry const plankGeometry = PxBoxGeometry(length / 2.f, height / 2.f, width / 2.f);

	GameObject* pPlank_01;
	PxRigidStatic* const pPlankRigidBody_01 = pxPhysX->createRigidStatic(PxTransform::createIdentity());
	pPlankRigidBody_01->createShape(plankGeometry, *pPlankMaterial);
	pPlank_01 = new CubePosColorNorm(length, height, width);
	pPlank_01->AttachRigidActor(pPlankRigidBody_01);
	pPlank_01->Rotate(0, 0, -PxPi / 8.f);
	pPlank_01->Translate(-60, 20, 0);
	AddGameObject(pPlank_01);

	GameObject* pPlank_02;
	PxRigidStatic* const pPlankRigidBody_02 = pxPhysX->createRigidStatic(PxTransform::createIdentity());
	pPlankRigidBody_02->createShape(plankGeometry, *pPlankMaterial);
	pPlank_02 = new CubePosColorNorm(length, height, width);
	pPlank_02->AttachRigidActor(pPlankRigidBody_02);
	pPlank_02->Rotate(0, 0, -PxPi / 8.f);
	pPlank_02->Translate(0, 20, 0);
	AddGameObject(pPlank_02);

	GameObject* pPlank_03;
	PxRigidStatic* const pPlankRigidBody_03 = pxPhysX->createRigidStatic(PxTransform::createIdentity());
	pPlankRigidBody_03->createShape(plankGeometry, *pPlankMaterial);
	pPlank_03 = new CubePosColorNorm(length, height, width);
	pPlank_03->AttachRigidActor(pPlankRigidBody_03);
	pPlank_03->Rotate(0, 0, -PxPi / 8.f);
	pPlank_03->Translate(60, 20, 0);
	AddGameObject(pPlank_03);
	
}

void FrictionTestScene::AddCubes()
{
	const int length = 10;
	const int width = 10;
	const int height = 10;
	using namespace physx;
	PxPhysics* const pxPhysX = PhysxManager::GetInstance()->GetPhysics();	
	PxBoxGeometry const CubeGeometry = PxBoxGeometry(length / 2.f, height / 2.f, width / 2.f);

	PxMaterial* const pCubeMaterial_01 = pxPhysX->createMaterial(0.f, 0.f, 0.f);
	PxRigidDynamic* const pCubeRigidBody_01 = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	pCubeRigidBody_01->createShape(CubeGeometry, *pCubeMaterial_01);
	m_pCube_01 = new CubePosColorNorm(length, height, width);
	m_pCube_01->AttachRigidActor(pCubeRigidBody_01);
	m_pCube_01->Rotate(0, 0, -PxPi / 8.f);
	m_pCube_01->Translate(-60, 40, 0);
	AddGameObject(m_pCube_01);

	PxMaterial* const pCubeMaterial_02 = pxPhysX->createMaterial(0.5f, 0.5f, 0.f);
	PxRigidDynamic* const pCubeRigidBody_02 = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	pCubeRigidBody_02->createShape(CubeGeometry, *pCubeMaterial_02);
	m_pCube_02 = new CubePosColorNorm(length, height, width);
	m_pCube_02->AttachRigidActor(pCubeRigidBody_02);
	m_pCube_02->Rotate(0, 0, -PxPi / 8.f);
	m_pCube_02->Translate(0, 40, 0);
	AddGameObject(m_pCube_02);

	PxMaterial* const pCubeMaterial_03 = pxPhysX->createMaterial(1.f, 1.f, 0.f);
	PxRigidDynamic* const pCubeRigidBody_03 = pxPhysX->createRigidDynamic(PxTransform::createIdentity());
	pCubeRigidBody_03->createShape(CubeGeometry, *pCubeMaterial_03);
	m_pCube_03 = new CubePosColorNorm(length, height, width);
	m_pCube_03->AttachRigidActor(pCubeRigidBody_03);
	m_pCube_03->Rotate(0, 0, -PxPi / 8.f);
	m_pCube_03->Translate(60, 40, 0);
	AddGameObject(m_pCube_03);
}
