#pragma once


typedef void* SDL_GLContext;
struct SDL_Window;

class GGraphicsEngine {
public:
	GGraphicsEngine() = default;
	~GGraphicsEngine() = default;

	//initialise the graphics
	bool InitEngine(SDL_Window* sdlWindow);

private:
	//storing memory location for opengl context
	SDL_GLContext m_sdlGLContext;
};