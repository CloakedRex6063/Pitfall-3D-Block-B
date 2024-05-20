#include "input.h"

#include "imgui.h"
#include "iostream"
#include "engine/framework/gameInstance.h"

void Input::KeyPressed(XKeyEvent& keyEvent)
{
	const auto key = GetKeyFromX11(XLookupKeysym(&keyEvent, 0));
	inputManager->keyStates[key] = true;
}

void Input::KeyReleased(XKeyEvent& keyEvent)
{
	const auto key = GetKeyFromX11(XLookupKeysym(&keyEvent, 0));
	inputManager->keyStates[key] = false;
}

void Input::MousePressed(const XButtonEvent& buttonEvent) const
{
	auto buttonID = buttonEvent.button - 1;
	switch (buttonID)
	{
	case 0:
		inputManager->keyStates[KEY_MouseLeft] = true;
		break;
	case 1:
		buttonID = 2;
		inputManager->keyStates[KEY_MouseRight] = true;
		break;
	case 2:
		buttonID = 1;
		inputManager->keyStates[KEY_MouseMiddle] = true;
		break;
	case 3:
		ImGui::GetIO().AddMouseWheelEvent(0, 1.0f);
		break;
	case 4:
		ImGui::GetIO().AddMouseWheelEvent(0, -1.0f);
		break;
	case 7:
		buttonID = 3;
		inputManager->keyStates[KEY_Mouse4] = true;
		break;
	case 8:
		buttonID = 4;
		inputManager->keyStates[KEY_Mouse5] = true;
		break;
	default:
		break;
	}
	ImGui::GetIO().MouseDown[buttonID] = true;
}

void Input::MouseReleased(const XButtonEvent& buttonEvent) const
{
	auto buttonID = buttonEvent.button - 1;
	switch (buttonID)
	{
	case 0:
		inputManager->keyStates[KEY_MouseLeft] = false;
		break;
	case 1:
		buttonID = 2;
		inputManager->keyStates[KEY_MouseRight] = false;
		break;
	case 2:
		buttonID = 1;
		inputManager->keyStates[KEY_MouseMiddle] = false;
		break;
	case 7:
		buttonID = 3;
		inputManager->keyStates[KEY_Mouse4] = false;
		break;
	case 8:
		buttonID = 4;
		inputManager->keyStates[KEY_Mouse5] = false;
		break;
	default:
		break;
	}
	ImGui::GetIO().MouseDown[buttonID] = false;
}

void Input::MouseMoved(const XMotionEvent& motionEvent)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(static_cast<float>(motionEvent.x), static_cast<float>(motionEvent.y));
}

void Input::SetInput(InputManager* newInputManager)
{
	inputManager = newInputManager;
}

Keys Input::GetKeyFromX11(KeySym key)
{
	switch (key)
	{
	case XK_A: case XK_a: return KEY_A;
	case XK_B: case XK_b: return KEY_B;
	case XK_C: case XK_c: return KEY_C;
	case XK_D: case XK_d: return KEY_D;
	case XK_E: case XK_e: return KEY_E;
	case XK_F: case XK_f: return KEY_F;
	case XK_G: case XK_g: return KEY_G;
	case XK_H: case XK_h: return KEY_H;
	case XK_I: case XK_i: return KEY_I;
	case XK_J: case XK_j: return KEY_J;
	case XK_K: case XK_k: return KEY_K;
	case XK_L: case XK_l: return KEY_L;
	case XK_M: case XK_m: return KEY_M;
	case XK_N: case XK_n: return KEY_N;
	case XK_O: case XK_o: return KEY_O;
	case XK_P: case XK_p: return KEY_P;
	case XK_Q: case XK_q: return KEY_Q;
	case XK_R: case XK_r: return KEY_R;
	case XK_S: case XK_s: return KEY_S;
	case XK_T: case XK_t: return KEY_T;
	case XK_U: case XK_u: return KEY_U;
	case XK_V: case XK_v: return KEY_V;
	case XK_W: case XK_w: return KEY_W;
	case XK_X: case XK_x: return KEY_X;
	case XK_Y: case XK_y: return KEY_Y;
	case XK_Z: case XK_z: return KEY_Z;
	case XK_Escape: return KEY_Escape;
	case XK_Return: return KEY_Enter;
	case XK_space: return KEY_Space;
	case XK_Tab: return KEY_Tab;
	case XK_BackSpace: return KEY_Backspace;
	case XK_Left: return KEY_Left;
	case XK_Right: return KEY_Right;
	case XK_Up: return KEY_Up;
	case XK_Down: return KEY_Down;
	case XK_Shift_L: return KEY_Shift_Left;
	case XK_Shift_R: return KEY_Shift_Right;
	case XK_Control_L: return KEY_Ctrl_Left;
	case XK_Control_R: return KEY_Ctrl_Right;
	case XK_Alt_L: return KEY_Alt_Left;
	case XK_Alt_R: return KEY_Alt_Right;
	default: return KEY_None;
	}
}
