#include "stdafx.h"
#include "AudioTestScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"
#include "ContentManager.h"
#include "SoundManager.h"
#include "MeshObject.h"

using namespace FMOD;

AudioTestScene::AudioTestScene()
	: GameScene(L"AudioTestScene")
	, m_Volume(1.f)
	, m_pSphere(nullptr)
	, m_LastCamPos(FMOD_VECTOR())
	, m_pChannel3D(nullptr)
	, m_pChannel{}
{
}


AudioTestScene::~AudioTestScene()
{
}

void AudioTestScene::Initialize()
{
	auto physX = &m_pPhysxScene->getPhysics();

	auto defaultMaterial = physX->createMaterial(.5f,.5f,.1f);
	EnablePhysxDebugRendering(true);

	// sound source made visible
	const float radius{ 1.0f };
	const int slices{ 10 }, stacks{ 10 };
	m_pSphere = new SpherePosColorNorm(radius, slices, stacks, XMFLOAT4{ 1,0,0,1 });
	AddGameObject( m_pSphere);

	//GROUND PLANE
	auto groundPlaneActor = physX->createRigidStatic(PxTransform::createIdentity());
	groundPlaneActor->createShape(PxPlaneGeometry(),*defaultMaterial,PxTransform(PxQuat(DirectX::XM_PIDIV2,PxVec3(0,0,1))));
	m_pPhysxScene->addActor(*groundPlaneActor);

	//2D SOUND
	FMOD_RESULT fmodResult;
	auto pFmodSystem = SoundManager::GetInstance()->GetSystem();
	FMOD::Sound *pSound;
	fmodResult = pFmodSystem->createStream("Sounds/wave.mp3", FMOD_DEFAULT, 0, &pSound);		// FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D.
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	pSound->setMode(FMOD_LOOP_NORMAL);

	fmodResult = pFmodSystem->playSound(pSound, 0, true , &m_pChannel);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	//3D SOUND
	FMOD::Sound *p3DSound;
	fmodResult = pFmodSystem->createStream("Sounds/drumloop.wav", FMOD_3D|FMOD_3D_LINEARROLLOFF, 0, &p3DSound);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	fmodResult = p3DSound->setMode(FMOD_LOOP_NORMAL);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	fmodResult = p3DSound->set3DMinMaxDistance(0.f, 50.f);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	fmodResult = pFmodSystem->playSound(p3DSound, 0, true , &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(fmodResult);

	//Actions
	m_pSceneContext->GetInput()->AddInputAction(InputAction(InputIds::Play, InputTriggerState::pressed, VK_SPACE)); //PLAY
	m_pSceneContext->GetInput()->AddInputAction(InputAction(InputIds::Play3D, InputTriggerState::pressed, 'B'));
	m_pSceneContext->GetInput()->AddInputAction(InputAction(InputIds::IncreaseVolume, InputTriggerState::down, VK_UP));
	m_pSceneContext->GetInput()->AddInputAction(InputAction(InputIds::DecreaseVolume, InputTriggerState::down, VK_DOWN));
}

inline FMOD_VECTOR ToFmodVector(DirectX::XMFLOAT3 vec)
{
	auto fVec = FMOD_VECTOR();
	fVec.x = vec.x;
	fVec.y = vec.y;
	fVec.z = vec.z;

	return fVec;
}

void AudioTestScene::Update()
{
	//GLOBAL LISTENER SETTINGS
	auto cam = m_pSceneContext->GetCamera();
	auto forward = ToFmodVector(cam->GetForward());
	auto up = ToFmodVector(cam->GetUp());
	auto pos = ToFmodVector(cam->GetPosition());

	auto vel = FMOD_VECTOR();

	auto deltaT = m_pSceneContext->GetGameTime()->GetElapsed();
	vel.x = (pos.x - m_LastCamPos.x) / (deltaT);
    vel.y = (pos.y - m_LastCamPos.y) / (deltaT);
    vel.z = (pos.z - m_LastCamPos.z) / (deltaT);
	m_LastCamPos = pos;

	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);

	//2D SOUND ACTIONS
	if(m_pSceneContext->GetInput()->IsActionTriggered(InputIds::Play))
	{
		bool bPauzed=false;
		m_pChannel->getPaused(&bPauzed);
		if(bPauzed)
		{
			Logger::GetInstance()->GetInstance()->LogInfo(L"[2D SOUND] Entering play state");
			m_pChannel->setPaused(false);
		}
		else 
		{
			Logger::GetInstance()->LogInfo(L"[2D SOUND] Entering paused state");
			m_pChannel->setPaused(true);
		}
	}

	
	if(m_Volume < 1.f && m_pSceneContext->GetInput()->IsActionTriggered(InputIds::IncreaseVolume))
	{
		m_Volume += 0.5f * m_pSceneContext->GetGameTime()->GetElapsed();
		if(m_Volume > 1.f) m_Volume = 1.f;

		m_pChannel->setVolume(m_Volume);
			
	}

	if(m_Volume > 0.f && m_pSceneContext->GetInput()->IsActionTriggered(InputIds::DecreaseVolume))
	{
		m_Volume -= 0.5f * m_pSceneContext->GetGameTime()->GetElapsed();
		if(m_Volume < 0.f) m_Volume = 0.f;

		m_pChannel->setVolume(m_Volume);
	}

	//3D SOUND ACTIONS
	if(m_pSceneContext->GetInput()->IsActionTriggered(InputIds::Play3D))
	{
		bool bPauzed=false;
		m_pChannel3D->getPaused(&bPauzed);
		if(bPauzed)
		{
			Logger::GetInstance()->LogInfo(L"[3D SOUND] Entering play state");
			m_pChannel3D->setPaused(false);
		}
		else 
		{
			Logger::GetInstance()->LogInfo(L"[3D SOUND] Entering paused state");
			m_pChannel3D->setPaused(true);
		}
	}
}

void AudioTestScene::SceneActivated()
{
	Logger::GetInstance()->LogInfo(L".::AudioTestScene::.");
	Logger::GetInstance()->LogInfo(L"KeyboardInput: SPACE -->Play/Pause 3D Sound (Up/Down for volume control)");
	Logger::GetInstance()->LogInfo(L"KeyboardInput: B     -->Play/Pause 3D Sound");
}

void AudioTestScene::SceneDeactivated()
{
	m_pChannel->setPaused(true);
	m_pChannel3D->setPaused(true);
}