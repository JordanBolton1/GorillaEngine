#pragma once

//system libs
#include <iostream>
#include <vector>
#include <cstdint>

class GShaderProgram;
struct GSTransform;

struct GSVertexData {
	float m_pos[3] = { 0.0f, 0.0f, 0.0f };
	float m_colour[3] = { 1.0f,1.0f,1.0f };
	float m_texCoords[2] = { 0.0f, 0.0f };
	float m_normal[3] = { 0.0f, 0.0f, 0.0f };
};

class GMesh {
public:
	GMesh();
	~GMesh();

	//creating a mesh using vertex and index data
	bool CreateMesh(const std::vector<GSVertexData> vertices, const std::vector<uint32_t>& indices);

	//draw the mesh to the renderer
	void Render(const std::shared_ptr<GShaderProgram>& shader, const GSTransform& transform);

private:
	//store verticies
	std::vector<GSVertexData> m_vertices;

	//store the indecies for the vertex data
	std::vector<uint32_t> m_indices;

	//storew the id for the vertex array object
	uint32_t m_vao;
	
	//store the id for the vertex buffer object
	uint32_t m_vbo;

	//store the id for the element array object
	uint32_t m_eao;
};