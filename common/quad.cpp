
#include "quad.h"


Quad::Quad() { }


/* vitural */
void Quad::Init() {
    floats = vector<float>(8, 0);
    floats[0] = -1;
    floats[1] = -1;

    floats[2] =  1;
    floats[3] = -1;

    floats[4] =  -1;
    floats[5] =  1;

    floats[6] =  1;
    floats[7] =  1;

    mesh = Mesh(2, floats);
}


/* vitural */
void Quad::Draw() {
    mesh.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}



