#pragma once

//system libs
#include <string>
#include <cstdint>

//enum determine the type of shader
enum GEShaderType : uint8_t {
	ST_VERTEX = 0U,
	ST_FRAGMENT
};

struct GSTransform;

class GShaderProgram {
public:
	GShaderProgram();
	~GShaderProgram();

	//create the shader using a vertex and fragment file
	bool InitShader(const std::string& vShaderPath, const std::string& fShaderPath);

	//activate the shader to update
	//you cant change values ina shader without activating it
	void Activate();

	//set the transform of the model in the shader
	void SetModelTransform(const GSTransform& transform);

private:
	//import a shader based on the shader type
	bool ImportShaderByType(const std::string& filePath, GEShaderType shaderType);

	//convert a file intyo a string
	std::string ConvertFileToString(const std::string& filePath);
	
	//link the shader to the gpu through open gl
	bool LinkToGPU();

private:
	//store the file paths
	std::string m_filePath[2] = { "", "" };

	//store the shader ids
	uint32_t m_shaderIDs[2] = { 0,0 };

	//store the id for the program
	uint32_t m_programID;
}; 