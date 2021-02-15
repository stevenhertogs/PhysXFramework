#pragma once
#include "GameScene.h"
#include <vector>
class WallDemolitionScene : public GameScene
{
public:
	WallDemolitionScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void SceneActivated() override;
private:
	std::vector<GameObject*> m_pCubes;
	GameObject* m_pBall = nullptr;
	XMFLOAT3 m_BallPos = { 0,10,-10 };
	enum class InputActionEnum { reset };
	void ResetScene();
	void AddGroundPlane();
	void AddWall();
};

