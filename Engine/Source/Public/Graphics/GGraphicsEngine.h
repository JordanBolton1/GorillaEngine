#pragma once
#include <iostream>


typedef void* SDL_GLContext;
struct SDL_Window;
class GMesh;
class GShaderProgram;

class GGraphicsEngine {
public:
	GGraphicsEngine() = default;
	~GGraphicsEngine() = default;

	//initialise the graphics
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	//render the graphics engine
	void Render(SDL_Window* sdlWindow);

private:
	//storing memory location for opengl context
	SDL_GLContext m_sdlGLContext;

	//store the shader for the engine
	std::shared_ptr<GShaderProgram>m_shader;
};