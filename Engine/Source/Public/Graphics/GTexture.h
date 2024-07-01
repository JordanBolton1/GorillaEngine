#pragma once
#include "EngineTypes.h"

class GTexture {
public:
	GTexture();
	~GTexture();

	//import a file and cnvert it to a texture
	bool LoadTexture(const GString fileName,const GString path);

	//activate the texture to use for open gl
	void BindTexture(const GUi32& textureNumber);
	
	//deactivates the texture in opewn gl
	void Unbind();

	//gets the import path of the teture
	GString GetImportPath() const { return m_path; }

	//get thew custom file name of the texture
	GString GetName() const { return m_fileName; }

	//get the id of the texture
	GUi32 GetID() const { return m_ID; }

private:
	//import path of the image
	GString m_path;

	//custom name of the texture
	GString m_fileName;

	//the id for the texture in open gl
	GUi32 m_ID;

	//texture parameters
	int m_width, m_height, m_channels;

};