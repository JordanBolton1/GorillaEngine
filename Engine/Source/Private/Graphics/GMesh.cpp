#include "Graphics/GMesh.h"
#include "Debug/GDebug.h"

// external libs
#include <GLEW/glew.h>

GMesh::GMesh()
{
	m_vao = m_vbo = m_eao = 0;
	GDebug::Log("mesh created");
}

GMesh::~GMesh()
{
	GDebug::Log("mesh destroyed");
}

bool GMesh::CreateMesh(const std::vector<GSVertexData> vertices, const std::vector<uint32_t>& indices)
{
	//store the vertex data
	m_vertices = vertices;
	m_indices = indices;

	//create a vertex array object
	//assign the id for the object to the m_vao variable
	//stores a reference to any  VBOs attached to the VAO
	glGenVertexArrays(1, &m_vao);

	//test if vao failed
	if (m_vao == 0) {
		//convert the error into a readable string 
		std::string errorMsg = reinterpret_cast<const char*> (glewGetErrorString(glGetError()));
		GDebug::Log("mesh failed to create vao: " + errorMsg, LT_WARN);
		return false;
	}

	//this says use the vao as the active working vao for any vao function
	glBindVertexArray(m_vao);

	//create a vertex buffer object
	//vertex buffer object hold the data for the vertices in the gpu
	glGenBuffers(1, &m_vbo);

	//test if vbo failed
	if (m_vbo == 0) {
		//convert the error into a readable string 
		std::string errorMsg = reinterpret_cast<const char*> (glewGetErrorString(glGetError()));
		GDebug::Log("mesh failed to create vbo: " + errorMsg, LT_WARN);
		return false;
	}

	//bind the buffer object to say this is the working vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//create a element array buffer
	glGenBuffers(1, &m_eao);

	//test if eao failed
	if (m_eao == 0) {
		//convert the error into a readable string 
		std::string errorMsg = reinterpret_cast<const char*> (glewGetErrorString(glGetError()));
		GDebug::Log("mesh failed to create eao: " + errorMsg, LT_WARN);
		return false;
	}

	//bind the eao as the active element array object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eao);

	//set the buffer data
	//start with the vbo which stores the vertex data
	glBufferData(
		GL_ARRAY_BUFFER,//type of data we're storing
		static_cast<GLsizeiptr>(m_vertices.size() * sizeof(GSVertexData)),//size of each data in bytes
		m_vertices.data(),//memory location of the data
		GL_STATIC_DRAW);//this data will not modified frequently

	//set the data for the eao
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,//type of data we're storing
		static_cast<GLsizeiptr>(m_indices.size() * sizeof(uint32_t)),//size of each data in bytes
		m_indices.data(),//memory location of the data
		GL_STATIC_DRAW);//this data will not modified frequently

	//common practice to claear the vao from the gpu
	glBindVertexArray(0);
}

void GMesh::Render()
{
	//biding this mesh as the active vao 
	glBindVertexArray(m_vao);

	//render the vao
	glDrawElements(
		GL_TRIANGLES,//draw the mesh as triangles
		static_cast<GLsizei>(m_indices.size()),//how many vertices are there
		GL_UNSIGNED_INT, //what type of data is the index array
		nullptr//how many are gonna skip
		);

	// clear the vao
	glBindVertexArray(0);

}
