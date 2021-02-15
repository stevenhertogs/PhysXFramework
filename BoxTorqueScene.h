#pragma once
#include "GameScene.h"
class BoxTorqueScene final :public GameScene
{
public:
	BoxTorqueScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void SceneActivated() override;
private:
	GameObject* m_pCube;
	enum class InputActionEnum { reset };
	void ResetScene();
	void AddGroundPlane();
};

