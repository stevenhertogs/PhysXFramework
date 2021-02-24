#include "stdafx.h"
#include "TriggersAssignmentScene.h"
#include "ContentManager.h"
#include "MeshObject.h"
#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"
#include "SoundManager.h"

using namespace physx;

TriggersAssignmentScene::TriggersAssignmentScene() : GameScene(L"PhysXMeshTestScene")
{
}

void TriggersAssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	auto& physX = m_pPhysxScene->getPhysics();
	
	auto defaultMaterial = physX.createMaterial(1.f, 1.f, 0.f);

	//Create Level
	auto pLevelMesh = new MeshObject(L"Meshes/Level.ovm");

	auto pLevelActor = physX.createRigidDynamic(PxTransform::createIdentity());
	pLevelActor->setRigidDynamicFlag(PxRigidBodyFlag::eKINEMATIC, true);
	auto pLevelTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Meshes/Level.ovpt");
	pLevelActor->createShape(PxTriangleMeshGeometry(pLevelTriangleMesh), *defaultMaterial);
	pLevelMesh->AttachRigidActor(pLevelActor);
	AddGameObject(pLevelMesh);
	pLevelMesh->Translate(0, 0, 0);

	//create player
	const float radius = 1.f;
	const int slices = 50;
	const int stacks = 50;
	XMFLOAT4 color = { 1.f,1.f,1.f,1 };
	
	m_pBall = new SpherePosColorNorm(radius, slices, stacks, color);
	PxRigidBody* const pRigidBody_01 = physX.createRigidDynamic(PxTransform::createIdentity());
	PxSphereGeometry sphereGeometry_01 = PxSphereGeometry(radius);
	PxMaterial* const pSphereMaterial_01 = physX.createMaterial(0.3f, 0.3f, 0.f);
	pRigidBody_01->createShape(sphereGeometry_01, *pSphereMaterial_01);
	m_pBall->AttachRigidActor(pRigidBody_01);
	pRigidBody_01->setMass(20);
	
	
	m_pBall->Translate(m_BallPos.x, m_BallPos.y, m_BallPos.z);
	AddGameObject(m_pBall);
	//lock ball z-axis
	auto d6Joint = PxD6JointCreate(physX, nullptr, PxTransform::createIdentity(), m_pBall->GetRigidActor(), PxTransform::createIdentity());
	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	
	CreateRightCube();
	CreateLeftCube();
	CreateLeftSphere();
	CreateRightSphere();
	CreateLeftTrigger();
	CreateRightTrigger();
	CreateLeftHatch();
	CreateRightHatch();

	FMOD_RESULT fmodResult;
	auto pFmodSystem = SoundManager::GetInstance()->GetSystem();
	fmodResult = pFmodSystem->createStream("Sounds/bell.mp3", FMOD_DEFAULT, 0, &m_pBellSound);		// FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D.
	SoundManager::GetInstance()->ErrorCheck(fmodResult);
	m_pBellSound->setMode(FMOD_LOOP_OFF);

	m_pSceneContext->GetInput()->AddInputAction(InputAction((int)InputIds::Jump, InputTriggerState::pressed , VK_SPACE));
	m_pSceneContext->GetInput()->AddInputAction(InputAction((int)InputIds::Reset, InputTriggerState::pressed, 'R'));
	
}

void TriggersAssignmentScene::Update()
{
	float torque = 100.f;
	float force = 70.f;


	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		const XMFLOAT3 cameraForward = m_pDefaultCamera->GetForward();
		XMFLOAT3 torqueVec{};
		XMStoreFloat3(&torqueVec, XMLoadFloat3(&cameraForward) * -torque);

		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addForce({ force,0,0 });
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

	//rotate hatches
	if(m_bRightTriggerEntered && !m_bRightHatchOpened)
	{
		m_pRightHatch->RotateEuler(0, 0, -90);
		auto rightHatchPos = m_pRightHatch->GetPosition();
		auto rightHatchPosVec = XMLoadFloat3(&rightHatchPos);
		XMVECTOR offset = { 2,-1,0 };
		XMStoreFloat3(&rightHatchPos, rightHatchPosVec + offset);
		m_pRightHatch->Translate(rightHatchPos.x,rightHatchPos.y,rightHatchPos.z);

		m_bRightHatchOpened = true;
		
		FMOD_RESULT fmodResult;
		auto pFmodSystem = SoundManager::GetInstance()->GetSystem();
		fmodResult = pFmodSystem->playSound(m_pBellSound, 0, true, &m_pChannel);
		SoundManager::GetInstance()->ErrorCheck(fmodResult);
		m_pChannel->setPaused(false);
	}

	if (m_bLeftTriggerEntered && !m_bLeftHatchOpened)
	{
		m_pLeftHatch->RotateEuler(0, 0, 90);
		auto LeftHatchPos = m_pLeftHatch->GetPosition();
		auto LeftHatchPosVec = XMLoadFloat3(&LeftHatchPos);
		XMVECTOR offset = { -2,-1,0 };
		XMStoreFloat3(&LeftHatchPos, LeftHatchPosVec + offset);
		m_pLeftHatch->Translate(LeftHatchPos.x, LeftHatchPos.y, LeftHatchPos.z);

		m_bLeftHatchOpened = true;

		FMOD_RESULT fmodResult;
		auto pFmodSystem = SoundManager::GetInstance()->GetSystem();
		fmodResult = pFmodSystem->playSound(m_pBellSound, 0, true, &m_pChannel);
		SoundManager::GetInstance()->ErrorCheck(fmodResult);
		m_pChannel->setPaused(false);		
	}

	

	if (m_pSceneContext->GetInput()->IsActionTriggered((int)InputIds::Jump))
	{
		const float JumpForce = 10000;
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addForce({ 0,JumpForce,0 });
	}

	if(m_pSceneContext->GetInput()->IsActionTriggered((int)InputIds::Reset))
	{
		ResetScene();
	}
}

void TriggersAssignmentScene::Draw() const
{
}

void TriggersAssignmentScene::SceneActivated()
{
	ResetScene();
}

void TriggersAssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for(PxU32 i = 0; i<count;i++)
	{
		if(pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
			continue;
		const PxTriggerPair& cp = pairs[i];
		auto triggerActor = cp.triggerShape->getActor();

		if(cp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if(cp.triggerActor == m_pLeftTrigger->GetRigidActor() && cp.otherActor == m_pLeftCube->GetRigidActor())
			{
				m_bLeftTriggerEntered = true;
			}
			else if(cp.triggerActor == m_pRightTrigger->GetRigidActor() && cp.otherActor == m_pRightCube->GetRigidActor())
			{
				m_bRightTriggerEntered = true;
			}
		}
	}
}

void TriggersAssignmentScene::CreateRightCube()
{
	auto& physX = m_pPhysxScene->getPhysics();
	XMFLOAT3 dimensions = XMFLOAT3(2.f, 2.f, 2.f);
	m_pRightCube = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z,{1,0,0,1});
	PxRigidBody* const pRigidBody = physX.createRigidDynamic(PxTransform::createIdentity());
	PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
	PxMaterial* const pBoxMaterial = physX.createMaterial(0.5f, 0.5f, 0.3f);
	pRigidBody->createShape(boxGeometry, *pBoxMaterial);
	m_pRightCube->AttachRigidActor(pRigidBody);
	AddGameObject(m_pRightCube);
	m_pRightCube->Translate(m_RightCubePos.x, m_RightCubePos.y,m_RightCubePos.z);
}

void TriggersAssignmentScene::CreateLeftCube()
{
	auto& physX = m_pPhysxScene->getPhysics();
	XMFLOAT3 dimensions = XMFLOAT3(2.f, 2.f, 2.f);
	m_pLeftCube = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z, { 0,0,1,1 });
	PxRigidBody* const pRigidBody = physX.createRigidDynamic(PxTransform::createIdentity());
	PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
	PxMaterial* const pBoxMaterial = physX.createMaterial(0.5f, 0.5f, 0.3f);
	pRigidBody->createShape(boxGeometry, *pBoxMaterial);
	m_pLeftCube->AttachRigidActor(pRigidBody);
	AddGameObject(m_pLeftCube);
	m_pLeftCube->Translate(m_LeftCubePos.x, m_LeftCubePos.y, m_LeftCubePos.z);
}

void TriggersAssignmentScene::CreateLeftSphere()
{
	auto& physX = m_pPhysxScene->getPhysics();
	const float radius = 1.f;
	const int slices = 50;
	const int stacks = 50;
	XMFLOAT4 color = { 0.f,0.f,1.f,1 };
	m_pLeftSphere = new SpherePosColorNorm(radius, slices, stacks, color);
	auto actor = physX.createRigidDynamic(PxTransform::createIdentity());
	auto shape = PxSphereGeometry(radius);
	auto mat = physX.createMaterial(0.5f, 0.5f, 0.5f);
	actor->createShape(shape,*mat);
	m_pLeftSphere->AttachRigidActor(actor);
	AddGameObject(m_pLeftSphere);
	m_pLeftSphere->Translate(m_LeftSpherePos.x, m_LeftSpherePos.y, m_LeftSpherePos.z);
	
}

void TriggersAssignmentScene::CreateRightSphere()
{
	auto& physX = m_pPhysxScene->getPhysics();
	const float radius = 1.f;
	const int slices = 50;
	const int stacks = 50;
	XMFLOAT4 color = { 1.f,0.f,0.f,1 };
	m_pRightSphere = new SpherePosColorNorm(radius, slices, stacks, color);
	auto actor = physX.createRigidDynamic(PxTransform::createIdentity());
	auto shape = PxSphereGeometry(radius);
	auto mat = physX.createMaterial(0.5f, 0.5f, 0.5f);
	actor->createShape(shape, *mat);
	m_pRightSphere->AttachRigidActor(actor);
	AddGameObject(m_pRightSphere);
	m_pRightSphere->Translate(m_RightSpherePos.x, m_RightSpherePos.y, m_RightSpherePos.z);
}

void TriggersAssignmentScene::CreateLeftTrigger()
{
	auto& physX = m_pPhysxScene->getPhysics();
	XMFLOAT3 dimensions = XMFLOAT3(2.f, 2.f, 4.f);
	m_pLeftTrigger = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z, { 0,0,1,1 });
	auto const pRigidBody = physX.createRigidStatic(PxTransform::createIdentity());
	PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
	PxMaterial* const pBoxMaterial = physX.createMaterial(0.5f, 0.5f, 0.3f);
	auto shape = pRigidBody->createShape(boxGeometry, *pBoxMaterial);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	
	m_pLeftTrigger->AttachRigidActor(pRigidBody);
	AddGameObject(m_pLeftTrigger);
	m_pLeftTrigger->Translate(-7,2,0);
}

void TriggersAssignmentScene::CreateRightTrigger()
{
	auto& physX = m_pPhysxScene->getPhysics();
	XMFLOAT3 dimensions = XMFLOAT3(2.f, 2.f, 4.f);
	m_pRightTrigger = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z, { 1,0,0,1 });
	auto const pRigidBody = physX.createRigidStatic(PxTransform::createIdentity());
	PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
	PxMaterial* const pBoxMaterial = physX.createMaterial(0.5f, 0.5f, 0.3f);
	auto shape = pRigidBody->createShape(boxGeometry, *pBoxMaterial);
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pRightTrigger->AttachRigidActor(pRigidBody);
	AddGameObject(m_pRightTrigger);
	m_pRightTrigger->Translate(7, 2, 0);
}

void TriggersAssignmentScene::CreateLeftHatch()
{
	auto& physX = m_pPhysxScene->getPhysics();
	XMFLOAT3 dimensions = XMFLOAT3(2.f, 0.3f, 4.f);
	m_pLeftHatch = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z, { 0,0,1,1 });
	auto pRigidBody = physX.createRigidDynamic(PxTransform::createIdentity());
	pRigidBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
	PxMaterial* const pBoxMaterial = physX.createMaterial(0.5f, 0.5f, 0.3f);
	pRigidBody->createShape(boxGeometry, *pBoxMaterial);
	m_pLeftHatch->AttachRigidActor(pRigidBody);
	AddGameObject(m_pLeftHatch);
	m_pLeftHatch->Translate(m_LeftHatchPos.x,m_LeftHatchPos.y,m_LeftHatchPos.z);
}

void TriggersAssignmentScene::CreateRightHatch()
{
	auto& physX = m_pPhysxScene->getPhysics();
	XMFLOAT3 dimensions = XMFLOAT3(2.f, 0.3f, 4.f);
	m_pRightHatch = new CubePosColorNorm(dimensions.x, dimensions.y, dimensions.z, { 1,0,0,1 });
	auto pRigidBody = physX.createRigidDynamic(PxTransform::createIdentity());
	pRigidBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxBoxGeometry const boxGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);
	PxMaterial* const pBoxMaterial = physX.createMaterial(0.5f, 0.5f, 0.3f);
	pRigidBody->createShape(boxGeometry, *pBoxMaterial);
	m_pRightHatch->AttachRigidActor(pRigidBody);
	AddGameObject(m_pRightHatch);
	m_pRightHatch->Translate(m_RightHatchPos.x,m_RightHatchPos.y,m_RightHatchPos.z);
}

void TriggersAssignmentScene::ResetScene()
{
	m_pBall->Translate(m_BallPos.x, m_BallPos.y, m_BallPos.z);
	ResetVelocity(m_pBall);
	
	m_pRightHatch->RotateEuler(0, 0, 0);
	m_pLeftHatch->RotateEuler(0, 0, 0);
	m_pLeftSphere->Translate(m_LeftSpherePos.x,m_LeftSpherePos.y,m_LeftSpherePos.z);
	m_pRightSphere->Translate(m_RightSpherePos.x, m_RightSpherePos.y, m_RightSpherePos.z);

	ResetVelocity(m_pRightSphere);
	ResetVelocity(m_pLeftSphere);
	

	m_pRightCube->Translate(m_RightCubePos.x, m_RightCubePos.y, m_RightCubePos.z);
	m_pLeftCube->Translate(m_LeftCubePos.x, m_LeftCubePos.y, m_LeftCubePos.z);

	ResetVelocity(m_pRightCube);
	ResetVelocity(m_pLeftCube);

	m_pRightHatch->Translate(m_RightHatchPos.x, m_RightHatchPos.y, m_RightHatchPos.z);
	m_pLeftHatch->Translate(m_LeftHatchPos.x, m_LeftHatchPos.y, m_LeftHatchPos.z);

	m_bLeftHatchOpened = false;
	m_bRightHatchOpened = false;
	m_bLeftTriggerEntered = false;
	m_bRightTriggerEntered = false;
	
}

void TriggersAssignmentScene::ResetVelocity(GameObject* object)
{
	static_cast<PxRigidDynamic*>(object->GetRigidActor())->setAngularVelocity(PxVec3(0.f, 0.f, 0.f));
	static_cast<PxRigidDynamic*>(object->GetRigidActor())->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));
	object->Rotate(0, 0, 0);
}

