#include "Graphics/GGraphicsEngine.h"
#include "Debug/GDebug.h"

//external libs
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

bool GGraphicsEngine::InitEngine(SDL_Window* sdlWindow)
{
	if (sdlwindow == nullptr) {

		GDebug::Log("SDL window was null, graphics engine failed to initialised", LT_ERROR);
		return false;
	}

	m_sdlGLContext = SDL_GL_CreateContext(sdlWindow);

	if(m_sdlGLContext == nullptr){

		GDebug::Log("sdl failed to create gl context", LT_ERROR);
		GDebug::Log("sdl failed to create gl context, graphics engine fail to initialise", LT_ERROR);
			return false;
	}

	if (SDL_GL_MakeCurrent(sdlWindow, m_sdlGLContext) != 0) {

		GDebug::Log("sld failed to make gl context current, graphics");
	}

	return false;
}

