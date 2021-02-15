#pragma once
#include "GameScene.h"
class PhysXTestScene :
    public GameScene
{
public:
	PhysXTestScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void SceneActivated() override;
private:
	void ResetScene();
	void AddGroundPlane();

	enum class InputActionEnum {reset};
	GameObject* m_pCube = nullptr;


};

