#include "slaplaceoperator.h"

SLaplaceOperator::SLaplaceOperator()
{

}

SMatrix SLaplaceOperator::bypass(const SMatrix &src)
{
    SMatrix ret(src);
    int xn,yn,tmp;
    for (int y=0;y<src.height();++y)
        for (int x=0;x<src.width();++x)
        {
            tmp=0;
            for (int i=-1;i<=1;++i)
                for (int j=-1;j<=1;++j)
                {
                    xn=x+i; yn=y+j;
                    tmp+=src.get(xn,yn)*lo[i+1][j+1];
                }
            ret(x,y)=tmp;
        }
    return ret;
}
