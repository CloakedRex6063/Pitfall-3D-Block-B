#include "GLES3/gl3.h"
#include "iostream"
#include "renderer.h"
#include "util/common.h"

using namespace std;

OpenGL::OpenGL(GameWindow* window) : window(window)
{
	InitEGL();
	ChooseEGLConfig();
	CreateEGLContext();
	CreateEGLSurface();
	MakeCurrent();
	CheckGLVersion();
}

OpenGL::~OpenGL()
{
	eglDestroySurface(eglDisplay, eglSurface);
	eglDestroyContext(eglDisplay, eglContext);
	eglTerminate(eglDisplay);
}

void OpenGL::InitEGL()
{
	eglDisplay = eglGetDisplay(window->GetDisplay());
	if (eglDisplay == EGL_NO_DISPLAY)
	{
		cerr << "RENDERER::OPENGL: No Display" << "\n";
	}

	EGLint major, minor;
	if (!eglInitialize(eglDisplay, &major, &minor))
	{
		cerr << "RENDERER::OPENGL: Failed to initialize EGL" << "\n";
	}
}

void OpenGL::ChooseEGLConfig()
{
	EGLint numConfigs;
	const EGLint configAttrib[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};
	if (!eglChooseConfig(eglDisplay,configAttrib,&eglConfig,1,&numConfigs))
	{
		cerr << "RENDERER::OPENGL: Failed to choose config" << "\n";
	}
}

void OpenGL::CreateEGLContext()
{

	const EGLint contextAttrib[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE
	};

	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttrib);
	if (eglContext == EGL_NO_CONTEXT)
	{
		std::cerr << "ERROR::EGL::CREATE_CONTEXT" << '\n';
	}
}

void OpenGL::CreateEGLSurface()
{
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, window->GetWindow(), nullptr);
	if (eglSurface == EGL_NO_SURFACE)
	{
		std::cerr << "ERROR::EGL::CREATE_WINDOW_SURFACE" << '\n';
	}
}

void OpenGL::MakeCurrent() const
{
	if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext))
	{
		std::cerr << "ERROR::EGL::MAKE_CONTEXT_CURRENT" << '\n';
	}
}

void OpenGL::CheckGLVersion()
{
	if (const char* versionStr = reinterpret_cast<const char*>(glGetString(GL_VERSION)))
	{
		std::cout << "OpenGL ES Version: " << versionStr << '\n';
	}
	else
	{
		std::cerr << "ERROR::GL::GET_STRING" << '\n';
	}
}

void OpenGL::SwapBuffers() const
{
	eglSwapBuffers(eglDisplay, eglSurface);
}
