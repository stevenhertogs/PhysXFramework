#pragma once
#include "GameScene.h"
class TriggersAssignmentScene : public GameScene
{
public:
	TriggersAssignmentScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void SceneActivated() override;
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	
	

private:
	
	enum class InputIds : int
	{
		Jump,
		Reset
	};
	GameObject* m_pBall;
	PxVec3 m_BallPos = { 0.f,8.f,0.f };

	GameObject* m_pRightCube;
	PxVec3 m_RightCubePos = {2.5f,5.f,0.f};

	GameObject* m_pLeftCube;
	PxVec3 m_LeftCubePos = { -2.5f,5.f,0.f };

	GameObject* m_pRightSphere;
	PxVec3 m_RightSpherePos = {2.f,25.f,0.f};

	GameObject* m_pLeftSphere;
	PxVec3 m_LeftSpherePos = { -2.f,25.f,0.f };;

	GameObject* m_pLeftTrigger;
	GameObject* m_pRightTrigger;

	GameObject* m_pLeftHatch;
	GameObject* m_pRightHatch;

	PxVec3 m_LeftHatchPos = {-9,17,0};
	PxVec3 m_RightHatchPos = {9,17,0};

	bool m_bRightTriggerEntered = false;
	bool m_bLeftTriggerEntered = false;

	bool m_bRightHatchOpened = false;
	bool m_bLeftHatchOpened = false;

	FMOD::Channel* m_pChannel;
	FMOD::Sound* m_pBellSound;

	void CreateRightCube();
	void CreateLeftCube();
	void CreateLeftSphere();
	void CreateRightSphere();

	void CreateLeftTrigger();
	void CreateRightTrigger();

	void CreateLeftHatch();
	void CreateRightHatch();

	void ResetScene();
	void ResetVelocity(GameObject* object);
	
};


