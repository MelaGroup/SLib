#ifndef STHRESHOLD_H
#define STHRESHOLD_H

#include <limits>
#include <map>
#include <SLib/Processing/sprocessing.h>

int Otsu(const SMatrix& src);

enum THRESH_MODE
{
    BINARY,
    BINARY_INV,
    TOZERO,
    TOZERO_INV,
};

template <THRESH_MODE mode>
class SThreshold: public SProcessing
{
    int threshold;

public:
    SThreshold(int threshold = std::numeric_limits<int>::min()):threshold(threshold){}
    SMatrix bypass(const SMatrix& src)
    {
        SMatrix ret(src.width(),src.height());

        if (threshold==std::numeric_limits<int>::min())
            threshold=Otsu(src);

        for (int y=0;y<src.height();++y)
            for (int x=0;x<src.width();++x)
                if (mode == BINARY)
                    ret.set(x,y,src(x,y)<threshold?0:1);
                else if (mode == BINARY_INV )
                    ret(x,y) =  src(x,y)<threshold?1:0;
                else if (mode == TOZERO)
                    ret(x,y) =  src(x,y)<threshold?0:src(x,y);
                else if (mode == TOZERO_INV)
                    ret(x,y) =  src(x,y)<threshold?src(x,y):0;

        return ret;
    }
};


#endif // STHRESHOLD_H
