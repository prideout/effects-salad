#include "hilbertUtil.h"

// from wikipedia

int Hilbert::PointToIndex(int n, int x, int y) 
{
    int rx, ry, s, d=0;
    for (s=n/2; s>0; s/=2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        _Rotate(s, &x, &y, rx, ry);
    }
    return d;
}
 
void Hilbert::IndexToPoint(int n, int d, int *x, int *y) 
{
    int rx, ry, s, t=d;
    *x = *y = 0;
    for (s=1; s<n; s*=2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        _Rotate(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}
 
//rotate/flip a quadrant appropriately
void Hilbert::_Rotate(int n, int *x, int *y, int rx, int ry) 
{
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
 
        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}
