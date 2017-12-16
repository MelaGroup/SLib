#include "laplaceoperator.h"

LaplaceOperator::LaplaceOperator()
{

}

SMatrix LaplaceOperator::bypass(const SMatrix &src)
{
    SMatrix ret(src);
    int xn,yn,tmp;
    for (int x=0;x<src.width();++i)
        for (int j=0;j<src.height();++j)
        {
            tmp=0;
            for (int i=-1;i<=1;++i)
                for (int j=-1;j<=1;++j)
                {
                    xn=x+i; yn=y+j;
                    tmp+=src.get(xn,yn)*lo[i+1][j+1];
                }
            if (tmp != 0)
                ret(x,y) = 10;
            else
                ret(x,y) = tmp;
        }
    return tmp;
}
