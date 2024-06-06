#include "GWindow.h"
#include "Graphics/GGraphicsEngine.h"
#include "Debug/GDebug.h"

//external libs
#include<SDL/SDL.h>

GWindow::GWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;

	std::cout << "window created" << std::endl;
}

GWindow::~GWindow()
{
	//if sdl window exists, destroy it
	if (m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);

	std::cout << "window destroyed" << std::endl;
}

bool GWindow::CreateWindow(const GSWindowParams& params)
{
	//enabling opengl in our sdl window
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

	//assigning the parameters to the member for the window
	m_params = params;

	//add vsync flag if its selected
	if (m_params.vsync)
		windowFlags += SDL_WINDOW_ALLOW_HIGHDPI;

	//set fullc
	if (m_params.fullscreen)
		windowFlags += SDL_WINDOW_FULLSCREEN_DESKTOP;
	else
		windowFlags += SDL_WINDOW_SHOWN;

	m_sdlWindow = SDL_CreateWindow(
		m_params.title.c_str(),
		m_params.x,
		m_params.y,
		m_params.w,
		m_params.h,
		windowFlags
	);

	//check if sdl window was created
	if (!m_sdlWindow) {
		std::cout << "sdl failed: " << SDL_GetError() << std::endl;
		CloseWindow();
		return false;
	}

	//create the graphics engine object
	m_graphicsEngine = std::make_unique<GGraphicsEngine>();

	//initialise the graphics engine and test if it fails
	if (!m_graphicsEngine->InitEngine(m_sdlWindow, m_params.vsync)) {
		GDebug::Log("window failed to initialise graphics engine", LT_ERROR);
		m_graphicsEngine = nullptr;
		return false;
	}

	return true;
}

void GWindow::Render()
{
	//rener the graphics engine if one exists
	if (m_graphicsEngine) {
		m_graphicsEngine->Render(m_sdlWindow);
	}
}
