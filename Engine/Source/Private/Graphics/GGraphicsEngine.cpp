#include "Graphics/GGraphicsEngine.h"
#include "Debug/GDebug.h"
#include "Graphics/GMesh.h"

//external libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

 std::vector<GSVertexData> vertexData;
 std::vector<uint32_t> indexData;
 //test mesh for debug
 std::unique_ptr<GMesh> m_mesh;

bool GGraphicsEngine::InitEngine(SDL_Window* sdlWindow, const bool& vsync)
{
	if (sdlWindow == nullptr) {
		GDebug::Log("SDL window was null", LT_ERROR);
		GDebug::Log("graphics engine fail to initialise: ", LT_ERROR);
		return false;
	}

	// create an opengl context
	m_sdlGLContext = SDL_GL_CreateContext(sdlWindow);

	//test if the context failed
	if(m_sdlGLContext == nullptr){

		GDebug::Log("sdl failed to create gl context: " + std::string(SDL_GetError()), LT_ERROR);
		GDebug::Log("graphics engine fail to initialise: ", LT_ERROR);
			return false;
	}

	//make the current  context ative for the sdl window
	//test if it failed
	if (SDL_GL_MakeCurrent(sdlWindow, m_sdlGLContext) != 0) {

		GDebug::Log("sld failed to make gl context current" + std::string(SDL_GetError()),LT_ERROR);
		GDebug::Log("graphics engine fail to initialise: ", LT_ERROR);

		return false;
	}

	if (vsync) {
		//try enable adaptive vsync and test if it failed
		if (SDL_GL_SetSwapInterval(-1) !=0) {
			//try to enable standard vsync and test if it failed
			if (SDL_GL_SetSwapInterval(1) != 0) {
				GDebug::Log("Graphics engine failed to initialise vsync: " + std::string(SDL_GetError()), LT_WARN);
				return false;
			}
		}
	}

	//log the success of the graphics engine init
	GDebug::Log("Successfully initialised graphics engine ", LT_SUCCESS);

	//create debug mesh
	m_mesh = std::make_unique<GMesh>();

	vertexData.resize(3);
	//vertex 1
	vertexData[0].m_pos[0] = 0.0f;
	vertexData[0].m_pos[1] = 0.5f;
	vertexData[0].m_pos[2] = 0.0f;
	//vertex 2
	vertexData[1].m_pos[0] = -0.5f;
	vertexData[1].m_pos[1] = -0.5f;
	vertexData[1].m_pos[2] = 0.0f;
	//vertex 3
	vertexData[2].m_pos[0] = 0.5f;
	vertexData[2].m_pos[1] = -0.5f;
	vertexData[2].m_pos[2] = 0.0f;

	indexData.resize(3);
	indexData[0] = 0;// vertex 1
	indexData[1] = 1;// vertex 2
	indexData[2] = 2;// vertex 3

	//create the mesh and tst if it failed
	if (!m_mesh->CreateMesh(vertexData, indexData)) {
		GDebug::Log("failed to create mesh");
	}

	return true;
}

void GGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	//set a background colour
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	//cleared back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT);

	//rendered custom graphics
	m_mesh->Render();

	//presented the frame to the window
	//swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
	
}
