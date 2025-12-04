#pragma once
#include <atomic>
#include <thread>

#include "functional"
#include "X11/Xlib.h"

struct InputThreadData;

class GameWindow
{
public:
	/**
	 * \brief Creates a new window
	 * \param title Title of the window being created
	 * \param width How wide the window is
	 * \param height How tall the window is
	 */
	GameWindow(const char* title, const int width, const int height);

	/**
	 * \brief Close the window and clears up any memory
	 */
	void CloseWindow();

	/*
	 * \brief Destroys the window
	 */
	void DestroyWindow();

	/**
	 * \brief Assigns key callbacks for the window
	 * \param keyPress Key press callback
	 * \param keyRelease Key release callback
	 */
	void SetKeyCallbacks(const std::function<void(XKeyEvent&)>& keyPress, const std::function<void(XKeyEvent&)>& keyRelease);

	/**
	 * \brief Assigns mouse callbacks for the window
	 * \param mousePress Mouse button press callback
	 * \param mouseRelease Mouse button release callback
	 * \param mouseMove Mouse move callback
	 */
	void SetMouseCallbacks(const std::function<void(XButtonEvent&)>& mousePress, const std::function<void(XButtonEvent&)>& mouseRelease,
	                       const std::function<void(XMotionEvent&)>& mouseMove);

	/**
	 * \brief Starts handling input
	 */
	void StartHandlingInput();

	/**
	 * \brief Checks if the window is still running
	 * \return True if the window is still running, false if it has been closed	
	 */
	bool IsRunning() const;

	/**
	 * \brief Returns the window being used
	 * \return Window
	 */
	Window GetWindow() const;

	/**
	 * \brief Returns the display that the window is using
	 * \return Display*
	 */
	Display* GetDisplay() const;

private:
	std::function<void(XKeyEvent&)> keyPressCallback;
	std::function<void(XKeyEvent&)> keyReleaseCallback;
	std::function<void(XButtonEvent&)> mousePressCallback;
	std::function<void(XButtonEvent&)> mouseReleaseCallback;
	std::function<void(XMotionEvent&)> mouseMoveCallback;

	Display* display;
	Window window;
	std::thread inputThread = {};
	std::atomic<bool> bRunning;
};

