#pragma once
#include "GameScene.h"
class TestScene : public GameScene
{
public:
	TestScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
private:
	GameObject*  m_pCube = nullptr;
	float m_AngleY = 0.f;
};

