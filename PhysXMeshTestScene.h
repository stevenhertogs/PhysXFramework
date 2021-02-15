#pragma once
#include "GameScene.h"
class PhysXMeshTestScene : public GameScene
{
public:
	PhysXMeshTestScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void SceneActivated() override;
	void SceneDeactivated() override;
private:
	void AddGroundPlane();

	PxRigidDynamic* m_pKinematicActor = nullptr;
	enum class ActionKeys{left,right,back,forward};
	PxVec3 m_KineMaticPosition = {};
};

