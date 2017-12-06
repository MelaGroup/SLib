#include "sprocessing.h"




int floodFill(SMatrix &src, int value, int x, int y)
{
    if (src.isValidPos(x,y))
    {
        std::deque<QPoint> deque{{x,y}};
        int original_val=src(x,y);
        if (original_val==value) return -1;
        src(x,y)=value;
        int counter=0;

        while(!deque.empty())
        {
            int fx=deque.front().x(),fy=deque.front().y();
            for (int dy=-1;dy<=1;++dy)
                for (int dx=-1;dx<=1;++dx)
                {
                    int nx=fx+dx,ny=fy+dy;
                    if (src.isValidPos(nx,ny)&& src(nx,ny)==original_val)
                    {
                        src(nx,ny)=value;
                        deque.push_back({nx,ny});
                    }
                }
            deque.pop_front();
            ++counter;
        }
        return counter;
    }
    else qDebug()<<"bad position("<<x<<","<<y<<") in SMatrix";
    return -1;
}


void binLaplace(SMatrix &src)
{
    SMatrix copy(src.width(),src.height());
    for(int r=0;r<src.height();++r)
        for(int c=0;c<src.width();++c)
        {
            copy(c,r)=0;
            copy(c,r)-=4*src(c,r);
            copy(c,r)+=src.get(c+1,r);
            copy(c,r)+=src.get(c,r+1);
            copy(c,r)+=src.get(c-1,r);
            copy(c,r)+=src.get(c,r-1);
            if(copy(c,r)!=0) copy(c,r)=70;
        }
    src.swap(copy);
}
