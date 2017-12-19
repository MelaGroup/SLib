#ifndef SGAUSSFILTER_H
#define SGAUSSFILTER_H

#include <SLib/Core/smatrix.h>
#include <SLib/Processing/sprocessing.h>

enum SIZE
{
    THREE,
    FIVE
};

template <SIZE size>
class SGaussFilter: public SProcessing
{
private:
    int mask_3x3 [3][3] = { {1,2,1},
                            {2,4,2},
                            {1,2,1} };
    int mask_5x5 [5][5] = { {1,4,6,4,1},
                            {4,16,24,16,4},
                            {6,24,36,24,6},
                            {4,16,24,16,4},
                            {1,4,6,4,1}    };
public:
    SGaussFilter(){}
    SMatrix bypass (const SMatrix &src)
    {
        int xn, yn, temp, radius, w;
        SMatrix ret(src.width(), src.height());
        if (size == THREE)
        {
            radius = 1;
            w = 16;
        }
        else if (size == FIVE)
        {
            radius = 2;
            w = 256;
        }
        for (int x=0; x<src.width(); ++x)
            for (int y=0; y<src.height(); ++y)
            {
                temp = 0;
                for (int i=-radius; i<=radius; ++i)
                    for (int j=-radius; j<=radius; ++j)
                    {
                        xn = x+i; yn = y+j;
                        if (size == THREE)
                            temp += src.get(xn,yn) * mask_3x3[i+radius][j+radius];
                        else if (size == FIVE)
                            temp += src.get(xn,yn) * mask_5x5[i+radius][j+radius];
                    }
                ret(x,y) = temp/w;
            }
        return ret;
    }
};

#endif // SGAUSSFILTER_H
