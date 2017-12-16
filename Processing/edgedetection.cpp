#include "edgedetection.h"

SMatrix SobelOperator::bypass(SMatrix &src)
{
    int xn, yn, gx, gy;
    SMatrix ret(src.width(),src.height());
    for (int x=0; x<src.width(); ++x)
        for (int y=0; y<src.height(); ++y)
        {
            gx = gy = 0;
            for (int i=-1; i<=1; ++i)
                for (int j=-1; j<=1; ++j)
                {
                    xn = x+i; yn = y+j;
                    gx += src.get(xn,yn) * Gx[i+1][j+1];
                    gy += src.get(xn,yn) * Gy[i+1][j+1];
                }
            ret(x,y) = std::sqrt(gx*gx + gy*gy);
        }
    return ret;
}

SMatrix PrewittOperator::bypass(SMatrix &src)
{
    int xn, yn, gx, gy;
    SMatrix ret(src.width(),src.height());
    for (int x=0; x<src.width(); ++x)
        for (int y=0; y<src.height(); ++y)
        {
            gx = gy = 0;
            for (int i=-1; i<=1; ++i)
                for (int j=-1; j<=1; ++j)
                {
                    xn = x+i; yn = y+j;
                    gx += src.get(xn,yn) * Gx[i+1][j+1];
                    gy += src.get(xn,yn) * Gy[i+1][j+1];
                }
            ret(x,y) = std::sqrt(gx*gx + gy*gy);
        }
    return ret;
}

SMatrix RobertsOperator::bypass(SMatrix &src)
{
    int xn, yn, gx, gy;
    SMatrix ret(src.width(),src.height());
    for (int x=0; x<src.width(); ++x)
        for (int y=0; y<src.height(); ++y)
        {
            gx = gy = 0;
            for (int i=-1; i<=0; ++i)
                for (int j=-1; j<=0; ++j)
                {
                    xn = x+i; yn = y+j;
                    gx += src.get(xn,yn) * Gx[i+1][j+1];
                    gy += src.get(xn,yn) * Gy[i+1][j+1];
                }
            ret(x,y) = std::sqrt(gx*gx + gy*gy);
        }
    return ret;
}
