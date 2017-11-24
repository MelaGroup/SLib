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
void Threshold(SSegmentationMap& src,int threshold)
{
    for(int r=0;r<src.height();++r)
        for(int c=0;c<src.width();++c)
        {
            int &p=src(c,r);
            p=(p<threshold)?1:0; //ввести режимы mode
        }
    src.buildPostThreshold();
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
            {
                throw std::invalid_argument("OtsuThreshold: The matrix used is not scaled (0,255)");
                exit(1);
            }

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

std::vector<int> postThresholdSegmentation(SMatrix &src)
{
    std::list<int> l{-1,-1};//первые два сегмента с id=0 и 1
    int segment_id=2; //segment_id =0 и 1 зарезервированы

    for(int y=0;y<src.height();++y)
        for(int x=0;x<src.width();++x)
        {
            if (src(x,y)==1)
            {
                int area=floodFill(src,segment_id,x,y);
                l.push_back(area);
                ++segment_id;
            }
        }
    return std::vector<int>{std::begin(l),std::end(l)};
}


SMatrix takeSegment(const SMatrix &src,const SMatrix& map,int segment_id)
{
    if (src.width()!=map.width() || src.height()!=map.height())
        throw std::invalid_argument("takeSegment: map size!= src size");

    int x1=map.width(),y1=map.height(),x2=0,y2=0;
    for(int y=0;y<map.height();++y)
        for(int x=0;x<map.width();++x)
            if (map(x,y)==segment_id)
            {
                if (x<x1) x1=x;
                if (y<y1) y1=y;
                if (x2<x) x2=x;
                if (y2<y) y2=y;
            }
    if (x2==0 && y2==0) throw std::invalid_argument("takeSegment - Segment with this id does not exist");

    //единичная рамка
    int rx=x1-1, ry=y1-1 , rw=x2-x1+3, rh=y2-y1+3;
    SMatrix ret=src.copy(rx,ry,rw,rh);
    for(int y=0;y<rh;++y)
        for(int x=0;x<rw;++x)
             if (!map.isValidPos(rx+x,ry+y) || map(rx+x,ry+y)!=segment_id) ret(x,y)=0;
    return ret;
}
