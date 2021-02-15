#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"

//Change this define to activate/deactivate the corresponding scenes
// W01 - W02  (#define ...)

//#define W01
#define W02

#ifdef W01
#include "TestScene.h"
#include "PhysXTestScene.h"
#include "BouncingBallsScene.h"
#include "FrictionTestScene.h"
#include "BoxForceScene.h"
#include "BoxTorqueScene.h"
#include "WallDemolitionScene.h"
#endif

#ifdef W02
#include "PhysXMeshTestScene.h"
#include "AudioTestScene.h"

#endif

namespace dae
{
	void AddScenes(SceneManager * pSceneManager)
	{

#ifdef W01
		pSceneManager->AddGameScene(new WallDemolitionScene());
		pSceneManager->AddGameScene(new BoxTorqueScene());
		pSceneManager->AddGameScene(new BoxForceScene());
	    pSceneManager->AddGameScene(new FrictionTestScene());
		pSceneManager->AddGameScene(new BouncingBallsScene());
		pSceneManager->AddGameScene(new PhysXTestScene());
		pSceneManager->AddGameScene(new TestScene());
	
#endif

#ifdef W02
		pSceneManager->AddGameScene(new AudioTestScene());
		pSceneManager->AddGameScene(new PhysXMeshTestScene());
#endif

	}
}

