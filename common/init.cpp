
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
    pezCheck(compileSuccess, "Can't compile vshader [%s]:\n%s", vsKey, spew);
    glAttachShader(programHandle, vsHandle);

    if (gsKey) {
        const char* gsSource = pezGetShader(gsKey);
        pezCheck(gsSource != 0, "Can't find gshader: %s\n", gsKey);
        GLuint gsHandle = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gsHandle, 1, &gsSource, 0);
        glCompileShader(gsHandle);
        glGetShaderiv(gsHandle, GL_COMPILE_STATUS, &compileSuccess);
        glGetShaderInfoLog(gsHandle, sizeof(spew), 0, spew);
        pezCheck(compileSuccess, "Can't compile gshader [%s]:\n%s", gsKey, spew);
        glAttachShader(programHandle, gsHandle);
    }

    const char* fsSource = pezGetShader(fsKey);
    pezCheck(fsSource != 0, "Can't find fshader: %s\n", fsKey);
    GLuint fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsHandle, 1, &fsSource, 0);
    glCompileShader(fsHandle);
    glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &compileSuccess);
    glGetShaderInfoLog(fsHandle, sizeof(spew), 0, spew);
    pezCheck(compileSuccess, "Can't compile fshader [%s]:\n%s", fsKey, spew);
    glAttachShader(programHandle, fsHandle);

    glBindFragDataLocation(programHandle, 0, "FragColor");
    glBindFragDataLocation(programHandle, 1, "Normal");
    glBindFragDataLocation(programHandle, 2, "Position");

    glLinkProgram(programHandle);
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    glGetProgramInfoLog(programHandle, sizeof(spew), 0, spew);
    pezCheck(linkSuccess, "Can't link shaders [fs: %s, vs: %s]:\n%s", fsKey, vsKey, spew);
    glUseProgram(programHandle);

    bool Verbose = false;
    if (Verbose) {
        int fragColor = glGetFragDataLocation(programHandle, "FragColor");
        int normal = glGetFragDataLocation(programHandle, "Normal");
        int position = glGetFragDataLocation(programHandle, "Position");
        if (fragColor > -1) printf("%s has a FragColor output at %d.\n", fsKey, fragColor);
        if (normal > -1) printf("%s has a Normal output at %d.\n", fsKey, normal);
        if (position > -1) printf("%s has a Position output at %d.\n", fsKey, position);
    }

    return programHandle;
}

GLuint InitVao(int componentCount, const FloatList& verts, const IndexList& indices) 
{
    GLuint vbo, vao, ibo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
    pezCheck(glGetError() == GL_NO_ERROR, "vao-vbo setup failed");

    // indices
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
    pezCheck(glGetError() == GL_NO_ERROR, "vao-ibo setup failed");

    // setup the "Position" attribute
    glVertexAttribPointer(AttrPosition, componentCount, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrPosition);
    return vao;
}

GLuint InitVao(int componentCount, const FloatList& verts) 
{
    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
    glVertexAttribPointer(AttrPosition, componentCount, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(AttrPosition);
    
    pezCheck(glGetError() == GL_NO_ERROR, "vao failed");

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



