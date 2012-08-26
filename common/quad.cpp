
#include "quad.h"
#include "init.h"


Quad::Quad() { 
    // TODO: allow the quad to take height/width parameters

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

}


/* vitural */
void Quad::Init() {

    // XXX: We should pack these guys into one single VBO

    float v[] = { -1, -1,
                   1, -1,
                  -1,  1,
                   1,  1 };

    float t[] = {  0,  1,
                   1,  1,
                   0,  0,
                   1,  0 };

    vao = Vao(2, &v[0], 8);
    vao.AddVertexAttribute(AttrTexCoord, 2, FloatList(t, t+8));
}


/* vitural */
void Quad::Draw() {
    vao.Bind();
    //glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    pezCheckGL("Quad::Draw");
}



