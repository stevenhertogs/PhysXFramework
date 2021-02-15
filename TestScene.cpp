#include "stdafx.h"
#include "TestScene.h"

#include "CubePosColorNorm.h"

TestScene::TestScene() : GameScene(L"TestScene")
{
}

void TestScene::Initialize()
{
	m_pCube = new CubePosColorNorm(2.f, 1.f, 0.5f);
	AddGameObject(m_pCube);

	BaseCamera* const pCamera = m_pSceneContext->GetCamera();
	pCamera->SetPosition({ 0.f,2.f,-5.f });
	pCamera->SetForward({ 0.f,-.3f,1.f });
}

void TestScene::Update()
{
	float const deltaTime = m_pSceneContext->GetGameTime()->GetElapsed();
	m_AngleY += deltaTime;
	m_pCube->Rotate(0.f, m_AngleY, 0.f);
}

void TestScene::Draw() const
{
}
