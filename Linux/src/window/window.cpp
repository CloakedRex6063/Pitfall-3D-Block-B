#include "window.h"

#include <chrono>
#include <thread>

#include "iostream"

using namespace std;

GameWindow::GameWindow(const char* title, const int width, const int height)
{
	display = XOpenDisplay(nullptr);
	if (!display)
	{
		cerr << "ERROR::GAMEWINDOW: Failed to initialise Display" << "\n";
	}

	const int screen = DefaultScreen(display);
	const auto root = RootWindow(display, screen);
	XSetWindowAttributes swa;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

	window = XCreateWindow(display, root, 0, 0, width, height, 0, CopyFromParent, InputOutput, nullptr, CWEventMask, &swa);
	if (!window)
	{
		cerr << "ERROR::GAMEWINDOW: Failed to create window" << "\n";
		XCloseDisplay(display);
	}

	XGrabPointer(display, window, True, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
	XMapWindow(display, window);
	XStoreName(display, window, title);

	bRunning = true;
}

void GameWindow::CloseWindow()
{
	bRunning = false;
}

void GameWindow::DestroyWindow()
{
	inputThread.join();
	XUngrabPointer(display, window);
	XDestroyWindow(display, window);
	XCloseDisplay(display);
}

void GameWindow::SetKeyCallbacks(const std::function<void(XKeyEvent&)>& keyPress,
                                 const std::function<void(XKeyEvent&)>& keyRelease)
{
	keyPressCallback = keyPress;
	keyReleaseCallback = keyRelease;
}

void GameWindow::SetMouseCallbacks(const std::function<void(XButtonEvent&)>& mousePress,
	const std::function<void(XButtonEvent&)>& mouseRelease, const std::function<void(XMotionEvent&)>& mouseMove)
{
	mousePressCallback = mousePress;
	mouseReleaseCallback = mouseRelease;
	mouseMoveCallback = mouseMove;
}

void GameWindow::StartHandlingInput()
{
	Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &wmDeleteMessage, 1);

	inputThread = std::thread([this,wmDeleteMessage]
	{
		while (bRunning)
		{
			XEvent event;
			XNextEvent(display, &event);
			switch (event.type)
			{
			case KeyPress:
				keyPressCallback(event.xkey);
				break;
			case KeyRelease:
				keyReleaseCallback(event.xkey);
				break;
			case ButtonPress:
				mousePressCallback(event.xbutton);
				break;
			case ButtonRelease:
				mouseReleaseCallback(event.xbutton);
				break;
			case MotionNotify:
				mouseMoveCallback(event.xmotion);
				break;
			case ClientMessage:
				if (event.xclient.message_type == XInternAtom(display, "WM_PROTOCOLS", False) &&
				static_cast<Atom>(event.xclient.data.l[0]) == wmDeleteMessage)
				{
					CloseWindow();
				}
				break;
			}
		}
	});
}

bool GameWindow::IsRunning() const
{
	return bRunning;
}

Window GameWindow::GetWindow() const
{
	return window;
}

Display* GameWindow::GetDisplay() const
{
	return display;
}

