#pragma once
struct GameSettings
{
	GameSettings();
	int width;
	int height;
	float aspectRatio;
	std::wstring windowTitle;
	bool isVSyncOn;
	XMFLOAT3 clearColor;

};

