#pragma once
#include "GameScene.h"
class FrictionTestScene : public GameScene
{
public:
	FrictionTestScene();
	virtual ~FrictionTestScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void SceneActivated() override;
private:
	GameObject* m_pCube_01 = nullptr;
	GameObject* m_pCube_02 = nullptr;
	GameObject* m_pCube_03 = nullptr;

	XMFLOAT3 m_CusePos_01 = {-60,30,0};
	XMFLOAT3 m_CusePos_02 = { 0,30,0 };;
	XMFLOAT3 m_CusePos_03 = { 60,30,0 };;

	BaseCamera* m_pCamera = nullptr;
	XMFLOAT3 m_CameraPos = { 0,20,-200 };

	void ResetScene();
	void AddGroundPlane();
	void AddPlatforms();
	void AddCubes();
};

