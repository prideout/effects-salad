#include "normalField.h"

/* virtual */
void
NormalField::Init() {
}

/* virtual */
void
NormalField::Draw() {
    lineBuffer.Bind();
    glDrawArrays(GL_LINES, 0, lineBuffer.vertexCount);
    pezCheckGL("NormalField::Draw");
    //std::cout << "vert count: " << _lineCount << std::endl;
}

/* virtual */
void
NormalField::Update() {
}


