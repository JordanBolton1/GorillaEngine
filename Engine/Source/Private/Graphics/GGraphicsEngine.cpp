#include "Graphics/GGraphicsEngine.h"
#include "Debug/GDebug.h"
#include "Graphics/GMesh.h"
#include "Graphics/GShaderProgram.h"
#include "Math/GSTransform.h"

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
	 if (!m_shader->InitShader(
		 "Shaders/SimpleShader/SimpleShader.vertex",
		 "Shaders/SimpleShader/SimpleShader.frag")) {

		 GDebug::Log("Graphics engine failed to initialise due to shader failure");
		 return false;
	 }

	//log the success of the graphics engine init
	GDebug::Log("Successfully initialised graphics engine ", LT_SUCCESS);

	//create debug mesh
	m_mesh = std::make_unique<GMesh>();

	vertexData.resize(4);
	//vertex 1
	vertexData[0].m_pos[0] = -0.5f;
	vertexData[0].m_pos[1] = 0.5f;
	//colour for v1
	vertexData[0].m_colour[0] = 1.0f;//r
	vertexData[0].m_colour[1] = 0.0f;//g
	vertexData[0].m_colour[2] = 0.0f;//b
	
	//vertex 2
	vertexData[1].m_pos[0] = -0.5f;
	vertexData[1].m_pos[1] = -0.5f;
	//colour for v2
	vertexData[1].m_colour[0] = 0.0f;
	vertexData[1].m_colour[1] = 1.0f;
	vertexData[1].m_colour[2] = 0.0f;
	
	//vertex 3
	vertexData[2].m_pos[0] = 0.5f;
	vertexData[2].m_pos[1] = 0.5f;
	//colour for v3
	vertexData[2].m_colour[0] = 0.0f;
	vertexData[2].m_colour[1] = 0.0f;
	vertexData[2].m_colour[2] = 1.0f;

	//vertex 4
	vertexData[3].m_pos[0] = 0.5f;
	vertexData[3].m_pos[1] = -0.5f;
	//colour for v4
	vertexData[3].m_colour[0] = 1.0f;
	vertexData[3].m_colour[1] = 0.0f;
	vertexData[3].m_colour[2] = 1.0f;
	

	indexData.resize(6);
	// First triangle (vertices 0, 1, 2)
	indexData[0] = 0; // Vertex 1
	indexData[1] = 1; // Vertex 2
	indexData[2] = 2; // Vertex 3

	// Second triangle (vertices 2, 1, 3)
	indexData[3] = 2; // Vertex 3
	indexData[4] = 1; // Vertex 2
	indexData[5] = 3; // Vertex 4

	//create the mesh and tst if it failed
	if (!m_mesh->CreateMesh(vertexData, indexData)) {
		GDebug::Log("failed to create mesh");
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
	static float speedX = 0.005f;  // Speed for x-axis movement
	static float speedY = 0.005f;  // Speed for y-axis movement

	transform.position.x += speedX;
	transform.position.y += speedY;

	// Check and handle boundary conditions for x-axis
	if (transform.position.x > 0.70f) {
		speedX = 0.0f;  // Stop x-axis movement
		speedY = -0.005f;  // Move downward (rotate counterclockwise)
		transform.position.x = 0.70f;  // Limit position to boundary
	}
	else if (transform.position.x < -0.70f) {
		speedX = 0.0f;  // Stop x-axis movement
		speedY = 0.005f;  // Move upward (rotate counterclockwise)
		transform.position.x = -0.70f;  // Limit position to boundary
	}

	// Check and handle boundary conditions for y-axis
	if (transform.position.y > 0.70f) {
		speedY = 0.0f;  
		speedX = 0.005f;  
		transform.position.y = 0.70f;  
	}
	else if (transform.position.y < -0.70f) {
		speedY = 0.0f; 
		speedX = -0.005f;  
		transform.position.y = -0.70f;  
	}


	transform.rotation.z += 0.5f;
	transform.scale = glm::vec3(0.5f);

	//rendered custom graphics
	m_mesh->Render(m_shader, transform);

	//presented the frame to the window
	//swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
	
}
