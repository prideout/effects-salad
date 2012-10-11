#pragma once

#include <glm/glm.hpp>

namespace Hilbert {

int PointToIndex(int n, int x, int y);
 
void IndexToPoint(int n, int d, int *x, int *y);

void _Rotate(int n, int *x, int *y, int rx, int ry);


};


