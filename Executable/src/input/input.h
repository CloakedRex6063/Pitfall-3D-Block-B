#pragma once
#include <unordered_map>
#include "X11/Xlib.h"
#include "X11/keysym.h"
#include "managers/input/inputManager.h"

class Input
{
public:
	void KeyPressed(XKeyEvent& keyEvent);
	void KeyReleased(XKeyEvent& keyEvent);
	void MousePressed(const XButtonEvent& buttonEvent) const;
	void MouseReleased(const XButtonEvent& buttonEvent) const;
	void MouseMoved(const XMotionEvent& motionEvent);

	void SetInput(InputManager* inputManager);

private:
	static Keys GetKeyFromX11(KeySym key);
	InputManager* inputManager;
};
