#include "Graphics/GShaderProgram.h"
#include "Debug/GDebug.h"
#include "Math/GSTransform.h"
#include "Graphics/GTexture.h"

//external libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

//System libs
#include <fstream>
#include <sstream>

#define GGET_GLEW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()));

GShaderProgram::GShaderProgram()
{
	m_programID = 0;
}

GShaderProgram::~GShaderProgram()
{
	GDebug::Log("Shader Program " + std::to_string(m_programID) + "destroyed");
}

bool GShaderProgram::InitShader(const std::string& vShaderPath, const GString& fShaderPath)
{
	//create the shader program in open gl
	m_programID = glCreateProgram();

	//test if the create program failed
	if (m_programID == 0) {
		const GString errorMsg = GGET_GLEW_ERROR;
		GDebug::Log("shader failed to initialise, couldnt create program: " + errorMsg);

		return false;
	}

	//if either of the shaders fail to import then fail the whole program
	if (!ImportShaderByType(vShaderPath, ST_VERTEX) || !ImportShaderByType(fShaderPath, ST_FRAGMENT)) {
		GDebug::Log("shader failed to initialise, couldnt import shaders: ");
		return false;
	}

	return LinkToGPU();
}

void GShaderProgram::Activate()
{
	glUseProgram(m_programID);
}

void GShaderProgram::SetModelTransform(const GSTransform& transform)
{
	//translate(move) > rotate > scale
	//initialise a deafault matrix transform
	glm::mat4 matrixT = glm::mat4(1.0f);

	//translate the matrix
	matrixT = glm::translate(matrixT, transform.position);

	//rotate per axis
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	//scale the matrix
	matrixT = glm::scale(matrixT, transform.scale);

	//fins the variable in the shader
	//all unifortm variables aare given an id by gl
	const int varID = glGetUniformLocation(m_programID, "model");
	//update the value
	glUniformMatrix4fv(varID, 1, GL_FALSE, glm::value_ptr(matrixT));

}

void GShaderProgram::RunTexture(const TShared<GTexture>& texture, const GUi32& slot)
{
	//bind the texture
	texture->BindTexture(slot);

	//the id for the variable in the shader
	int varID = 0;

	//get the id depending on the slot
	switch (slot)
	{
	case 0:
		varID = glGetUniformLocation(m_programID, "colourMap");
		break;
	default:
		break;
	}

	//update the shader
	glUniform1i(varID, slot);

}

bool GShaderProgram::ImportShaderByType(const GString& filePath, GEShaderType shaderType)
{
	//convert the shader to a string
	const GString shaderStr = ConvertFileToString(filePath);

	//make sure there is a string path
	if (shaderStr.empty()) {
		//error that string failed to import
		GDebug::Log("shader failed to import",LT_ERROR);
		return false;
	}

	//set and create an id for the shader based on the shader type
	switch (shaderType)
	{
	case ST_VERTEX:
		m_shaderIDs[shaderType] = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ST_FRAGMENT:
		m_shaderIDs[shaderType] = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		break;
	}

	//make sure there is a string path
	if (m_shaderIDs[shaderType] ==0) {
		//error that string failed to import
		const GString errorMsg = GGET_GLEW_ERROR;
		GDebug::Log("shader program could not assign shader id: " + errorMsg, LT_ERROR);
		return false;
	}

	//compile the shader onto the gpu
	const char* shaderCStr = shaderStr.c_str();
	glShaderSource(m_shaderIDs[shaderType], 1, &shaderCStr, nullptr);
	glCompileShader(m_shaderIDs[shaderType]);

	//test if teh compile worked
	GLint success;
	glGetShaderiv(m_shaderIDs[shaderType], GL_COMPILE_STATUS, &success);

	if (!success) {
		//create empty log
		char infoLog[512];
		//fill the log with info from gl about what happened
		glGetShaderInfoLog(m_shaderIDs[shaderType], 512, nullptr, infoLog);
		//log it
		GDebug::Log("shader compilation error: " + GString(infoLog),LT_ERROR);
		return false;
	}

	//attach the shader to the program ID
	glAttachShader(m_programID, m_shaderIDs[shaderType]);

	return true;
}

GString GShaderProgram::ConvertFileToString(const GString& filePath)
{
	//convert the file path into an ifstream
	std::ifstream shaderSource(filePath);

	//test if we can open file
	if (!shaderSource.is_open()) {
		GDebug::Log("shader failed to open file: " + filePath, LT_ERROR);
		return {};
	}

	//initialise a string stream
	std::stringstream shaderStream;

	//convert the fstream into a sstream
	//basically turns the file into a string readable by our code
	shaderStream << shaderSource.rdbuf();

	//close the file since we dont need it anymore
	shaderSource.close();

	return shaderStream.str();
}

bool GShaderProgram::LinkToGPU()
{
	//link programm to the gpu
	glLinkProgram(m_programID);

	//test if teh compile worked
	GLint success;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

	if (!success) {
		//create empty log
		char infoLog[512];
		//fill the log with info from gl about what happened
		glGetShaderInfoLog(m_programID, 512, nullptr, infoLog);
		//log it
		GDebug::Log("shader link error: " + GString(infoLog), LT_ERROR);
		return false;
	}

	GDebug::Log("Shader successfully initialised and linked at index " + std::to_string(m_programID));

	return true;
}
