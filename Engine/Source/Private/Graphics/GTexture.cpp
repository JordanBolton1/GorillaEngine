#include "Graphics/GTexture.h"

//external libs
#include <GLEW/glew.h>
#include <STB_IMAGE/stb_image.h>

GTexture::GTexture()
{
    m_path = m_fileName = "";
    m_ID = 0U;
    m_width = m_height = m_channels = 0;
}

GTexture::~GTexture()
{
    //as long as an id was generateed, delete the texture from open gl
    if(m_ID > 0)
    glDeleteTextures(1, &m_ID);

    GDebug::Log("texture destroyed: " + m_fileName);
}

bool GTexture::LoadTexture(const GString fileName, const GString path)
{
    //assign the filename and path
    m_fileName = fileName;
    m_path = path;

    //stb image imports images upside down
    //but actually open gl reads them in an inverted state
    stbi_set_flip_vertically_on_load(true);

    //load the image into a computer readable format
    unsigned char* data = stbi_load(
        m_path.c_str(),//path to the image
        &m_width, &m_height,//width and height of image
        &m_channels,//rgba
        0// do we want to specifically require a certain amount of channels, 0 = no limit
    );

    //test if the data imported or not
    if (data == nullptr) {
        GString error = "failed to load texture: " + m_fileName + ": " + stbi_failure_reason();
        GDebug::Log(error, LT_ERROR);

        return false;
    }

    //test the amount of channels
    if (m_channels > 4 || m_channels < 3) {
        GDebug::Log("failed to impot texture: " + m_fileName + ": incorrect number of channels must have 3 or 4 channels");
        return false;
    }

    //generate the texture id in open gl
    glGenTextures(1, &m_ID);

    //test if the generate failed
    if (m_ID == 0) {
        GString error = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
        GString errorMsg = "failed to generate texture ID: " + m_fileName + ": " + error;
        GDebug::Log(errorMsg, LT_ERROR);

        return false;
    }

    //bind the texture
    //tlls open gl that we want to use this texture
    glBindTexture(GL_TEXTURE_2D, m_ID);

    //set some default paramenters for the texture
    //set the texture wrapping parameters
    //if the texture doesnt fit the model, repeat the texture 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//X
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//Y

    //set the filtering parameters
    //how much to blur pixels
    //the resolution of the texture is lower than the size of the model
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //set the default format at 3 channels
    GLint intFormat = GL_RGB;

    //if the channels of the imported image = 4 then set the import type to rgba
    if (m_channels == 4) {
        intFormat = GL_RGBA;
    }

    //load the image data into the texture that we just updated
    glTexImage2D(
        GL_TEXTURE_2D,// use a 2d texture
        0,//levels
        intFormat,//format of the texture(internal)
        m_width, m_height,
        0,//border around the image (legacy)
        intFormat,//format of the texture (external)
        GL_UNSIGNED_BYTE,//type of data passed in
        data//image data we got from stbi
    );

    //generate mipmaps
    //lower resolustion versions of the texture
    glGenerateMipmap(GL_TEXTURE_2D);

    //unbind the texture from open gl
    //makes room for the next texture
    Unbind();

    //clear stbi image data
    stbi_image_free(data);

    //log the success of the import
    GDebug::Log("successfully imported texture: " + m_fileName, LT_SUCCESS);

    return true;
}

void GTexture::BindTexture(const GUi32& textureNumber)
{
    glActiveTexture(GL_TEXTURE0 + textureNumber);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void GTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
