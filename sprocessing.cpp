#include "sprocessing.h"


void Threshold(SMatrix &src,int threshold)
{
    for(int r=0;r<src.height();++r)
        for(int c=0;c<src.width();++c)
        {
            int &p=src(c,r);
            p=(p<threshold)?1:0; //ввести режимы mode
        }
}

int OtsuThreshold(const SMatrix &src)
{

    int histogram[256];
    for (int i=0;i<256;++i) histogram[i]=0;

    for(int r=0;r<src.height();++r)
        for(int c=0;c<src.width();++c)
            if (-1<src(c,r) && src(c,r)<256)
                ++(histogram[src(c,r)]);
            else
                throw std::invalid_argument("OtsuThreshold: The matrix used is not scaled (0,255)");

    int n=0,m=0;
    for(int i=0;i<256;++i)
    {
        n+=histogram[i];
        m+=i*histogram[i];
    }

    int max_sigma=0,threshold=0;
    int a1=0,b1=0;

    for(int t=0;t<256;++t)
    {
        a1+=t*histogram[t];
        b1+=histogram[t];

        float w1=(float)b1/n;
        float a=(float)a1/b1 - (float)(m - a1)/(n - b1);
        float sigma=w1*(1-w1)*a*a;
        if (sigma>max_sigma)
        {
            max_sigma=sigma;
            threshold=t;
        }
    }

    return threshold;
}

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
            copy(c,r)+=src.at(c+1,r);
            copy(c,r)+=src.at(c,r+1);
            copy(c,r)+=src.at(c-1,r);
            copy(c,r)+=src.at(c,r-1);
            if(copy(c,r)!=0) copy(c,r)=20;
        }
    src.swap(copy);
}
