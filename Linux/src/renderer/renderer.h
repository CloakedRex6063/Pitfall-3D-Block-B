#pragma once
#include "../window/window.h"
#include "EGL/egl.h"

class OpenGL
{
public:
	OpenGL(GameWindow* window);
	~OpenGL();
	void MakeCurrent() const;
	void SwapBuffers() const;
private:
	void InitEGL();
	void ChooseEGLConfig();
	void CreateEGLContext();
	void CreateEGLSurface();
	static void CheckGLVersion();

	GameWindow* window;
	EGLDisplay eglDisplay;
	EGLConfig eglConfig;
	EGLContext eglContext;
	EGLSurface eglSurface;
};

