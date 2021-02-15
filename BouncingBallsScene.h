#pragma once
#include "GameScene.h"
class BouncingBallsScene : public GameScene
{
public:
	BouncingBallsScene();
	virtual~BouncingBallsScene();

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void SceneActivated() override;

private:
	GameObject* m_pBall_01 = nullptr;
	GameObject* m_pBall_02 = nullptr;	
	GameObject* m_pBall_03 = nullptr;

	XMFLOAT3 m_BallPos_01 = { 0,20,0 };
	XMFLOAT3 m_BallPos_02 = { 20,30,0 };
	XMFLOAT3 m_BallPos_03 = { 40,40,0 };

	BaseCamera* m_pCamera = nullptr;
	XMFLOAT3 m_CameraPos = { 20,20,-100 };

	enum class InputActionEnum {reset};

	void ResetScene();
	void AddGroundPlane();
};

