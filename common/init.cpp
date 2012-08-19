
#include "init.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;

GLuint InitProgram(const char* fsKey, const char* vsKey, const char* gsKey)
{
    GLchar spew[256];
    GLint compileSuccess;
    GLuint programHandle = glCreateProgram();

    const char* vsSource = pezGetShader(vsKey);
    pezCheck(vsSource != 0, "Can't find vshader: %s\n", vsKey);
    GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsHandle, 1, &vsSource, 0);
    glCompileShader(vsHandle);
    glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &compileSuccess);
    glGetShaderInfoLog(vsHandle, sizeof(spew), 0, spew);
    pezCheck(compileSuccess, "Can't compile vshader:\n%s", spew);
    glAttachShader(programHandle, vsHandle);

    if (gsKey) {
        const char* gsSource = pezGetShader(gsKey);
        pezCheck(gsSource != 0, "Can't find gshader: %s\n", gsKey);
        GLuint gsHandle = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gsHandle, 1, &gsSource, 0);
        glCompileShader(gsHandle);
        glGetShaderiv(gsHandle, GL_COMPILE_STATUS, &compileSuccess);
        glGetShaderInfoLog(gsHandle, sizeof(spew), 0, spew);
        pezCheck(compileSuccess, "Can't compile gshader:\n%s", spew);
        glAttachShader(programHandle, gsHandle);
    }

    const char* fsSource = pezGetShader(fsKey);
    pezCheck(fsSource != 0, "Can't find fshader: %s\n", fsKey);
    GLuint fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsHandle, 1, &fsSource, 0);
    glCompileShader(fsHandle);
    glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &compileSuccess);
    glGetShaderInfoLog(fsHandle, sizeof(spew), 0, spew);
    pezCheck(compileSuccess, "Can't compile fshader:\n%s", spew);
    glAttachShader(programHandle, fsHandle);

    glLinkProgram(programHandle);
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    glGetProgramInfoLog(programHandle, sizeof(spew), 0, spew);
    pezCheck(linkSuccess, "Can't link shaders:\n%s", spew);
    glUseProgram(programHandle);
    return programHandle;
}


//GLuint InitVao(int componentCount, const FloatList& verts) {
GLuint InitVao(int componentCount, const FloatList& verts) {
    /*
    // Stretch to fit:
    float q[] = {
        -1, -1, 0, 1,
        +1, -1, 1, 1,
        -1, +1, 0, 0,
        +1, +1, 1, 0 };
        
    if (sourceHeight < 0) {
        sourceHeight = -sourceHeight;
        q[3] = 1-q[3];
        q[7] = 1-q[7];
        q[11] = 1-q[11];
        q[15] = 1-q[15];
    }

    float sourceRatio = (float) sourceWidth / sourceHeight;
    float destRatio = (float) destWidth  / destHeight;
    
    // Horizontal fit:
    if (sourceRatio > destRatio) {
        q[1] = q[5] = -destRatio / sourceRatio;
        q[9] = q[13] = destRatio / sourceRatio;

    // Vertical fit:    
    } else {
        q[0] = q[8] = -sourceRatio / destRatio;
        q[4] = q[12] = sourceRatio / destRatio;
    }
    */

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, componentCount, GL_FLOAT, GL_FALSE, 0, 0);
    //glVertexAttribPointer(AttrPosition, componentCount, GL_FLOAT, GL_FALSE, verts.size(), 0);
    //glVertexAttribPointer(AttrTexCoord, 2, GL_FLOAT, GL_FALSE, 16, offset(8));
    glEnableVertexAttribArray(0);
    pezCheck(glGetError() == GL_NO_ERROR, "vao failed");
    //glEnableVertexAttribArray(AttrPosition);
    //glEnableVertexAttribArray(AttrTexCoord);

    return vao;

}

void ReadBinaryFile(string filename, Blob* destination)
{
    ifstream binFile(filename.c_str(), ios::binary);
    vector<char> blob((istreambuf_iterator<char>(binFile)), 
                      (istreambuf_iterator<char>()));
    destination->resize(blob.size());
    memcpy(&destination[0], &blob[0], blob.size());
}

void ReadJsonFile(string filename, Json::Value* root)
{
    ifstream jsonFile(filename.c_str());
    string jsonString((istreambuf_iterator<char>(jsonFile)),
                      istreambuf_iterator<char>());
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonString.c_str(), *root);
    if (!parsingSuccessful) {
        cerr  << "Failed to parse JSON file: "
              << filename << endl
              << reader.getFormatedErrorMessages();
        exit(1);
    }
}



