#include "Graphics/GGraphicsEngine.h"
#include "Debug/GDebug.h"
#include "Graphics/GMesh.h"
#include "Graphics/GShaderProgram.h"
#include "Math/GSTransform.h"
#include "Graphics/GTexture.h"

//external libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

const std::vector<GSVertexData> vertexData = {
		//x     //y   //z        //r   //g   //b      //tx  //ty
	{ { -0.5f,  0.5f, 0.0f  }, { 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f} },// vertex data 1 - top left - 0
	{ {  0.5f,  0.5f, 0.0f  }, { 1.0f, 1.0f, 1.0f }, {1.0f, 1.0f} },// vertex data 2 - top right - 1
	{ { -0.5f, -0.5f, 0.0f  }, { 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} },// vertex data 3 - bottom left - 2
	{ {  0.5f, -0.5f, 0.0f  }, { 1.0f, 1.0f, 1.0f }, {1.0f, 0.0f} } // vertex data 4 - bottom right - 3
};

const std::vector<uint32_t> indexData = {
	 0, 1, 2, //triangle 1
	 1, 2, 3  //triangle 2
 };

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
	 if (m_sdlGLContext == nullptr) {

		 GDebug::Log("sdl failed to create gl context: " + std::string(SDL_GetError()), LT_ERROR);
		 GDebug::Log("graphics engine fail to initialise: ", LT_ERROR);
		 return false;
	 }

	 //make the current  context ative for the sdl window
	 //test if it failed
	 if (SDL_GL_MakeCurrent(sdlWindow, m_sdlGLContext) != 0) {

		 GDebug::Log("sld failed to make gl context current" + std::string(SDL_GetError()), LT_ERROR);
		 GDebug::Log("graphics engine fail to initialise: ", LT_ERROR);

		 return false;
	 }

	 if (vsync) {
		 //try enable adaptive vsync and test if it failed
		 if (SDL_GL_SetSwapInterval(-1) != 0) {
			 //try to enable standard vsync and test if it failed
			 if (SDL_GL_SetSwapInterval(1) != 0) {
				 GDebug::Log("Graphics engine failed to initialise vsync: " + std::string(SDL_GetError()), LT_WARN);
				 return false;
			 }
		 }
	 }

	 //initialise glew
	 GLenum glewResults = glewInit();

	 //test if glew failed
	 if (glewResults != GLEW_OK) {
		 std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glewResults));
		 GDebug::Log("Graphics engine failed to initialise GLEW:" + errorMsg);
		 return false;
	 }

	 m_shader = std::make_shared<GShaderProgram>();

	 //attempt to init shader and test if it failed
	 if (!m_shader->InitShader("Shaders/SimpleShader/SimpleShader.vertex", "Shaders/SimpleShader/SimpleShader.frag")) {

		 GDebug::Log("Graphics engine failed to initialise due to shader failure");
		 return false;
	 }

	//log the success of the graphics engine init
	GDebug::Log("Successfully initialised graphics engine ", LT_SUCCESS);

	//create debug mesh
	m_mesh = std::make_unique<GMesh>();

	//create the mesh and tst if it failed
	if (!m_mesh->CreateMesh(vertexData, indexData)) {
		GDebug::Log("failed to create mesh");
	}

	//create the texture object
	TShared<GTexture> defaultTexture = TMakeShared<GTexture>();

	//add the texture to the mesh if it successfully created
	if (defaultTexture->LoadTexture("Default Grid", "Textures/T_DefaultGrid.png")) {
		m_mesh->SetTexture(defaultTexture);
	}

	return true;
}

void GGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	//there is a wireframe shader by deafault, this activtes it
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//set a background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//cleared back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT);

	static GSTransform transform;
	//transform.rotation.z += 0.5f;
	

	//rendered custom graphics
	m_mesh->Render(m_shader, transform);

	//presented the frame to the window
	//swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
	
}
