#include "bilateralfilter.h"

BilateralFilter::BilateralFilter(double sd, double si):sigmaD(sd),sigmaI(si)
{

}

SMatrix BilateralFilter::bypass(const SMatrix &src)
{
    SMatrix ret(src.width(), src.height());
    for (int x=1;x<src.width()-1;++x)
        for (int y=1;y<src.height()-1;++y)
        {
            double I = src(x,y);
            double In = 0, w=0;
            for(int i=-1;i<=1;++i) for(int j=-1;j<=1;++j)
            {
                int xn=x+i; int yn=y+j;
                double tw = exp(-((i*i+j*j)/(2*pow(sigmaD,2)))-(pow((src(xn,yn)-I),2)/(2*pow(sigmaI,2))));
                w+=tw;
                In += src(xn,yn)*tw;
            }
            ret(x,y) = In/w;
        }
    return ret;
}

