#include "Graphics/GShaderProgram.h"
#include "Debug/GDebug.h"

GShaderProgram::GShaderProgram()
{
	m_programID = 0;
}

GShaderProgram::~GShaderProgram()
{
	GDebug::Log("Shader Program " + std::to_string(m_programID) + "destroyed");
}

bool GShaderProgram::InitShader(const std::string& vShaderPath, const std::string& fShaderPath)
{
	return false;
}

void GShaderProgram::Activate()
{
}

void GShaderProgram::SetModelTransform(const GSTransform& transform)
{
}

bool GShaderProgram::ImportShaderByType(const std::string& filePath, GEShaderType shaderType)
{
	return false;
}

std::string GShaderProgram::ConvertFileToString(const std::string& filePath)
{
	return std::string();
}

bool GShaderProgram::LinkToGPU()
{
	return false;
}
