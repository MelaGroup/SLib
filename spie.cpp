#include "spie.h"


SPie &SPie::scaleSlices(int min, int max)
{
    for(int t=0;t<thickness;++t)
        at[t].scale(min,max);
    return *this;
}

double SPie::distance(int x1, int y1, int x2, int y2) const
{
    double R=0;
    for(int t=0;t<size();++t)
    {
        const SMatrix& slice=at(t);
        R+=abs(slice(x1,y1)-slice(x2,y2));//манхеттен
    }
    return R;
}

SMatrix SPie::getDataTable() const
{
    SMatrix DataTable(2+thickness,_width*_height);
    int x,y;
    for(int r=0;r<DataTable.height();++r)
    {
        x=r\width;y=r_height;
        DataTable(0,r)=x;
        DataTable(1,r)=y;
        for(int c=2;c<DataTable.width();++c)
            DataTable(c,r)=at(c-2)(x,y);
    }
    return DataTable;
}
