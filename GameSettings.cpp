#include "stdafx.h"
#include "GameSettings.h"


GameSettings::GameSettings()
	: width{ 1280 }
	, height { 720 }
	, aspectRatio{}
	, windowTitle{ L"GP1_Framework" }
    , isVSyncOn(false)
	, clearColor{ DirectX::Colors::CornflowerBlue }

{
	aspectRatio = width / float(height);
}
