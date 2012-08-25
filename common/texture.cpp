#include "texture.h"

extern "C" {
#include "lodepng/lodepng.h"
}

Texture::Texture() :
    handle(0)
{
}

void
Texture::Bind(int stage) {
    glActiveTexture(stage + GL_TEXTURE0);
    glBindTexture(target, handle);
}


//
// Buffer Texture
//

BufferTexture::BufferTexture() :
    Texture()
{
    target = GL_TEXTURE_BUFFER;
}

void
BufferTexture::Init(GLenum internalFormat,
                    unsigned int sizeInBytes,
                    const GLvoid* data)
{
    glGenTextures(1, &handle);
    pezCheck(glGetError() == GL_NO_ERROR, "BufferTexture gen failed");

    glBindTexture(target, handle);
    pezCheck(glGetError() == GL_NO_ERROR, "Bind BufferTexture failed");

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(target, vbo);
    glBufferData(target, sizeInBytes, data, GL_STATIC_DRAW);
    pezCheck(glGetError() == GL_NO_ERROR, "TEXTURE_BUFFER population failed");

    glTexBuffer(target, internalFormat, vbo);
    pezCheck(glGetError() == GL_NO_ERROR, "glTexBuffer failed");
}

void
BufferTexture::Init(const FloatList& data)
{
    Init(GL_R32F, data.size() * 4, &data[0]);
}

void
BufferTexture::Init(const Vec3List& data)
{
    Init(GL_RGB32F, data.size() * 12, &data[0].x);
}

void
BufferTexture::Init(const Vec4List& data)
{
    Init(GL_RGBA32F, data.size() * 16, &data[0].x);
}

void
BufferTexture::GenMipmaps() {
    pezFatal("GenMipmaps failed: BufferTexture cannot have mipmaps");
}

//
// Rect Texture
//

RectTexture::RectTexture() :
    Texture()
{
    target = GL_TEXTURE_RECTANGLE;
}

void
RectTexture::Init(GLenum format,
                  GLenum internalFormat,
                  unsigned int numTexels,
                  const GLvoid* data)
{
    glGenTextures(1, &handle);
    glBindTexture(target, handle);
    _width = numTexels > 1024 ? 1024 : numTexels;
    _height = (numTexels + 1023) / 1024;
    glTexImage2D(target,
                 0,
                 internalFormat,
                 _width,
                 _height,
                 0,
                 format,
                 GL_FLOAT,
                 (const GLvoid*) data);
    pezCheckGL("RectTexture::Init Float failed");
}

void
RectTexture::Init(const FloatList& data)
{
    Init(GL_RED, GL_R32F, data.size(), &data[0]);
}

void
RectTexture::Init(const Vec3List& data)
{
    Init(GL_RGB, GL_RGB32F, data.size(), &data[0]);
}

void
RectTexture::Init(const Vec4List& data)
{
    Init(GL_RGBA, GL_RGBA32F, data.size(), &data[0]);
}

void
RectTexture::GenMipmaps()
{
    pezFatal("GenMipmaps failed: RectTexture cannot have mipmaps");
}

//
// Texture2D
//

void
Texture2D::_Init(GLenum format, GLenum internalFormat, const GLvoid* data)
{
    glGenTextures(1, &handle);
    glBindTexture(target, handle);
    glTexImage2D(target,
                 0,
                 internalFormat,
                 _width,
                 _height,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 (const GLvoid*) data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    pezCheckGL("Texture2D::_Init failed");
}

Texture2D::Texture2D()
{
    target = GL_TEXTURE_2D;
}

void
Texture2D::Init(const std::string& pngFilename)
{
    std::string fullpath = "data/" + pngFilename;
    const char* filename = fullpath.c_str();

    unsigned char* buffer;
    unsigned char* image;
    size_t buffersize, imagesize;
    LodePNG_Decoder decoder;
    LodePNG_loadFile(&buffer, &buffersize, filename);
    LodePNG_Decoder_init(&decoder);

    // Inspected format:
    LodePNG_Decoder_inspect(&decoder, buffer, buffersize);
    //int bpp = LodePNG_InfoColor_getBpp(&decoder.infoPng.color);
    int bitDepth = decoder.infoPng.color.bitDepth;
    int colorChannels = LodePNG_InfoColor_getChannels(&decoder.infoPng.color);
    pezCheck(bitDepth == 8, "Can't handle the PNG format in '%s'.", filename);

    // Desired format:
    decoder.infoRaw.color.colorType = decoder.infoPng.color.colorType;
    decoder.infoRaw.color.bitDepth = bitDepth;

    // Perform decoding:
    LodePNG_Decoder_decode(&decoder, &image, &imagesize, buffer, buffersize);
    pezCheck(!decoder.error, "error %u: %s\n", decoder.error, LodePNG_error_text(decoder.error));

    int w = _width = decoder.infoPng.width;
    int h = _height = decoder.infoPng.height;
    pezPrintString("Loaded %s (%d x %d) bufferSize = %d, imageSize = %d, colorChannels = %d\n",
                   filename, w, h, buffersize, imagesize, colorChannels);

    GLenum format = 0;
    switch (colorChannels) {
    case 1: format = GL_RED; break;
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    default: pezFatal("Texture has %d color channels!", colorChannels);
    }

    GLenum internalFormat = format;
    _Init(format, internalFormat, image);

    free(buffer);
    free(image);
}

void
Texture2D::GenMipmaps()
{
    glGenerateMipmap(target);
}

glm::ivec2
Texture2D::GetSize()
{
    return glm::ivec2(_width, _height);
}
