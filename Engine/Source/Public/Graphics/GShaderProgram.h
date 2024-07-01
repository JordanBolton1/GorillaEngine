#pragma once
#include "EngineTypes.h"

class GTexture;

//enum determine the type of shader
enum GEShaderType : GUi8 {
	ST_VERTEX = 0U,
	ST_FRAGMENT
};

struct GSTransform;

class GShaderProgram {
public:
	GShaderProgram();
	~GShaderProgram();

	//create the shader using a vertex and fragment file
	bool InitShader(const GString& vShaderPath, const GString& fShaderPath);

	//activate the shader to update
	//you cant change values ina shader without activating it
	void Activate();

	//set the transform of the model in the shader
	void SetModelTransform(const GSTransform& transform);

	//set a texture in the shader based on the slot
	void RunTexture(const TShared<GTexture>& texture, const GUi32& slot);

private:
	//import a shader based on the shader type
	bool ImportShaderByType(const GString & filePath, GEShaderType shaderType);

	//convert a file intyo a string
	GString ConvertFileToString(const GString& filePath);
	
	//link the shader to the gpu through open gl
	bool LinkToGPU();

private:
	//store the file paths
	GString m_filePath[2] = { "", "" };

	//store the shader ids
	GUi32 m_shaderIDs[2] = { 0,0 };

	//store the id for the program
	GUi32 m_programID;
}; 