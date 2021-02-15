#pragma once
#include "GameScene.h"
class AudioTestScene : public GameScene
{
public:
	AudioTestScene();
	~AudioTestScene();
protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override{}
	void SceneActivated() override;
	void SceneDeactivated() override;
private:
	enum class InputIds : int
	{
		Play,
		Play3D,
		IncreaseVolume,
		DecreaseVolume
	};
	FMOD::Channel *m_pChannel, * m_pChannel3D;
	FMOD_VECTOR m_LastCamPos;
	float m_Volume;
	


	enum class ActionKeys { left, right, back, forward };

};

