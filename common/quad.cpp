
#include "quad.h"
#include "init.h"


Quad::Quad() 
{ 
}


/* vitural */
void Quad::Init() 
{
    // XXX: We should pack these guys into one single VBO
    float v[] = { -1, -1,
                   1, -1,
                  -1,  1,
                   1,  1 };

    float t[] = {  0,  1,
                   1,  1,
                   0,  0,
                   1,  0 };

    vao = Vao(2, &v[0], 4);
    vao.AddVertexAttribute(AttrTexCoord, 2, FloatList(t, t+8));
}


/* vitural */
void Quad::Draw() 
{
    vao.Bind();
    //glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    pezCheckGL("Quad::Draw");
}



