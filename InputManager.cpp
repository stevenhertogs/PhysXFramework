#include "stdafx.h"
#include "InputManager.h"
#include "D3D11.h"


bool InputManager::m_Enabled = true;
bool InputManager::m_DisabledToEnabled = false;

InputManager::InputManager()
	: m_pCurrKeyboardState{}
	, m_pOldKeyboardState{}
	, m_pKeyboardState0{}
	, m_pKeyboardState1{}
	, m_KeyboardState0Active{}
	, m_OldMousePosition{ POINT{} }
	, m_CurrMousePosition{ POINT{} }
	, m_MouseMovement{ POINT{} }
{
}


InputManager::~InputManager()
{
	if(m_pKeyboardState0 != nullptr)
	{
		delete [] m_pKeyboardState0;
		delete [] m_pKeyboardState1;

		m_pKeyboardState0 = nullptr;
		m_pKeyboardState1 = nullptr;
		m_pCurrKeyboardState = nullptr;
		m_pOldKeyboardState = nullptr;
	}
}

void InputManager::Initialize()
{
	if(m_pKeyboardState0 == nullptr)
	{
		m_pKeyboardState0 = new BYTE[256];
		m_pKeyboardState1 = new BYTE[256];

		GetKeyboardState(m_pKeyboardState0);
		GetKeyboardState(m_pKeyboardState1);
	}
}

bool InputManager::AddInputAction(InputAction action)
{
	auto it = m_InputActions.find(action.actionID);
	if(it != m_InputActions.end()) return false;

	m_InputActions[action.actionID] = action;

	return true;
}

bool InputManager::IsActionTriggered(int actionID)
{
	return m_InputActions[actionID].isTriggered;
}

void InputManager::UpdateGamepadStates()
{
	DWORD dwResult;
	for(DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		m_OldGamepadState[i] = m_CurrGamepadState[i];

		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(i, &state);
		m_CurrGamepadState[i] = state;

		if(dwResult == ERROR_SUCCESS)
		{
			m_ConnectedGamepads[i] = true;
		}
		else
		{
			m_ConnectedGamepads[i] = false;		
		}
	}
}

bool InputManager::UpdateKeyboardStates()
{
	//Get Current KeyboardState and set Old KeyboardState
	BOOL getKeyboardResult;
	if(m_KeyboardState0Active)
	{
		getKeyboardResult = GetKeyboardState(m_pKeyboardState1);
		m_pOldKeyboardState = m_pKeyboardState0;
		m_pCurrKeyboardState = m_pKeyboardState1;
	}
	else
	{
		getKeyboardResult = GetKeyboardState(m_pKeyboardState0);
		m_pOldKeyboardState = m_pKeyboardState1;
		m_pCurrKeyboardState = m_pKeyboardState0;
	}

	m_KeyboardState0Active = !m_KeyboardState0Active;

	return getKeyboardResult>0?true:false;
}

void InputManager::Update()
{
	if(!m_Enabled)
		return;

	UpdateKeyboardStates();
	UpdateGamepadStates();

	//Reset previous InputAction States
	for(auto it = m_InputActions.begin(); it != m_InputActions.end(); ++it)
	{
		auto currAction = &(it->second);

		//Reset the previous state before updating/checking the new state
		currAction->isTriggered = false;

		switch(currAction->triggerState)
		{
		case InputTriggerState::pressed:
			//KEYBOARD
			if(currAction->keyboardCode > 0x07 && currAction->keyboardCode <= 0xFE)
				if(!IsKeyboardKeyDown_unsafe(currAction->keyboardCode,true) && IsKeyboardKeyDown_unsafe(currAction->keyboardCode))
					currAction->isTriggered = true;

			//MOUSE
			if(!currAction->isTriggered && currAction->mouseButtonCode > 0x0 && currAction->mouseButtonCode <= 0x06)
				if(!IsMouseButtonDown_unsafe(currAction->mouseButtonCode,true) && IsMouseButtonDown_unsafe(currAction->mouseButtonCode))
					currAction->isTriggered = true;

			//GAMEPADS
			if(!currAction->isTriggered && currAction->gamepadButtonCode != 0)
				if(!IsGamepadButtonDown_unsafe(currAction->gamepadButtonCode,currAction->playerIndex,true) && IsGamepadButtonDown_unsafe(currAction->gamepadButtonCode,currAction->playerIndex))
					currAction->isTriggered = true;

			break;

		case InputTriggerState::down:
			//KEYBOARD
			if(currAction->keyboardCode > 0x07 && currAction->keyboardCode <= 0xFE)
				if(IsKeyboardKeyDown_unsafe(currAction->keyboardCode,true) && IsKeyboardKeyDown_unsafe(currAction->keyboardCode))
					currAction->isTriggered = true;

			//MOUSE
			if(!currAction->isTriggered && currAction->mouseButtonCode > 0x0 && currAction->mouseButtonCode <= 0x06)
				if(IsMouseButtonDown_unsafe(currAction->mouseButtonCode,true) && IsMouseButtonDown_unsafe(currAction->mouseButtonCode))
					currAction->isTriggered = true;

			//GAMEPADS
			if(!currAction->isTriggered && currAction->gamepadButtonCode != 0)
				if(IsGamepadButtonDown_unsafe(currAction->gamepadButtonCode,currAction->playerIndex,true) && IsGamepadButtonDown_unsafe(currAction->gamepadButtonCode,currAction->playerIndex))
					currAction->isTriggered = true;
			break;

		case InputTriggerState::released:
			//KEYBOARD
			if(currAction->keyboardCode > 0x07 && currAction->keyboardCode <= 0xFE)
				if(IsKeyboardKeyDown_unsafe(currAction->keyboardCode,true) && !IsKeyboardKeyDown_unsafe(currAction->keyboardCode))
					currAction->isTriggered = true;

			//MOUSE
			if(!currAction->isTriggered && currAction->mouseButtonCode > 0x0 && currAction->mouseButtonCode <= 0x06)
				if(IsMouseButtonDown_unsafe(currAction->mouseButtonCode,true) && !IsMouseButtonDown_unsafe(currAction->mouseButtonCode))
					currAction->isTriggered = true;

			//GAMEPADS
			if(!currAction->isTriggered && currAction->gamepadButtonCode > 0x0 && currAction->gamepadButtonCode<=0x8000)
				if(IsGamepadButtonDown_unsafe(currAction->gamepadButtonCode,currAction->playerIndex,true) && !IsGamepadButtonDown_unsafe(currAction->gamepadButtonCode,currAction->playerIndex))
					currAction->isTriggered = true;
			break;
		}		
	}

	//Mouse Position
	m_OldMousePosition = m_CurrMousePosition;
	if(GetCursorPos(&m_CurrMousePosition))
	{
		ScreenToClient(Locator::GetD3D11()->windowHandle ,&m_CurrMousePosition);
	}
	// when window gains focus, the oldmouseposition is no longer valid TODO
	if (m_DisabledToEnabled)
	{
		m_MouseMovement.x = m_MouseMovement.y = 0;
		m_DisabledToEnabled = false;
	}
	else
	{
		m_MouseMovement.x = m_CurrMousePosition.x - m_OldMousePosition.x;
		m_MouseMovement.y = m_CurrMousePosition.y - m_OldMousePosition.y;
	}
}

bool InputManager::IsKeyboardKeyDown(int key, bool previousFrame)
{
	if(!m_pCurrKeyboardState || !m_pOldKeyboardState)
		return false;

	if(key > 0x07 && key <= 0xFE)
		return IsKeyboardKeyDown_unsafe(key, previousFrame);

	return false;
}

bool InputManager::IsMouseButtonDown(int button, bool previousFrame)
{
	if(button > 0x00 && button <= 0x06)
		return IsMouseButtonDown_unsafe(button, previousFrame);

	return false;
}

bool InputManager::IsGamepadButtonDown(WORD button, GamepadIndex playerIndex, bool previousFrame)
{
	if(button > 0x0000 && button<=0x8000)
		return IsGamepadButtonDown_unsafe(button, playerIndex, previousFrame);

	return false;
}

//NO RANGE CHECKS
bool InputManager::IsKeyboardKeyDown_unsafe(int key, bool previousFrame)
{
	if(previousFrame)
		return (m_pOldKeyboardState[key]&0xF0)!=0;
	else
		return (m_pCurrKeyboardState[key]&0xF0)!=0;
}

//NO RANGE CHECKS
bool InputManager::IsMouseButtonDown_unsafe(int button, bool previousFrame)
{
	if(previousFrame)
		return (m_pOldKeyboardState[button]&0xF0)!=0;
	else
		return (m_pCurrKeyboardState[button]&0xF0)!=0;
}

//NO RANGE CHECKS
bool InputManager::IsGamepadButtonDown_unsafe(WORD button, GamepadIndex playerIndex, bool previousFrame)
{
	if(!m_ConnectedGamepads[int(playerIndex)])
		return false;

	if(previousFrame)
		return (m_OldGamepadState[int(playerIndex)].Gamepad.wButtons&button)!=0;
	else
		return (m_CurrGamepadState[int(playerIndex)].Gamepad.wButtons&button)!=0;
}

DirectX::XMFLOAT2 InputManager::GetThumbstickPosition(bool leftThumbstick, GamepadIndex playerIndex)
{
	DirectX::XMFLOAT2 pos;
	if(leftThumbstick)
	{
		pos = DirectX::XMFLOAT2(m_CurrGamepadState[int(playerIndex)].Gamepad.sThumbLX, m_CurrGamepadState[int(playerIndex)].Gamepad.sThumbLY);
		
		if(pos.x>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pos.x<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)pos.x = 0;
		if(pos.y>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pos.y<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)pos.y = 0;
	}
	else
	{
		pos = DirectX::XMFLOAT2(m_CurrGamepadState[int(playerIndex)].Gamepad.sThumbLX, m_CurrGamepadState[int(playerIndex)].Gamepad.sThumbLY);

		if(pos.x>-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pos.x<XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)pos.x = 0;
		if(pos.y>-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pos.y<XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)pos.y = 0;
	}

	if(pos.x<0)pos.x/=32768;
	else pos.x/=32767;

	if(pos.y<0)pos.y/=32768;
	else pos.y/=32767;
	
	return pos;
}

float InputManager::GetTriggerPressure(bool leftTrigger, GamepadIndex playerIndex)
{
	if(leftTrigger)
	{
		return m_CurrGamepadState[int(playerIndex)].Gamepad.bLeftTrigger/255.0f;
	}
	else
	{
		return m_CurrGamepadState[int(playerIndex)].Gamepad.bRightTrigger/255.0f;
	}
}

void InputManager::SetVibration(float leftVibration, float rightVibration, GamepadIndex playerIndex)
{
	XINPUT_VIBRATION vibration;
	Clamp<float>(leftVibration, 0.0f, 1.0f);
	Clamp<float>(rightVibration, 0.0f, 1.0f);

    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    vibration.wLeftMotorSpeed = static_cast<WORD>(leftVibration * 65535);
    vibration.wRightMotorSpeed = static_cast<WORD>(rightVibration * 65535);

    XInputSetState(int(playerIndex), &vibration);
}

