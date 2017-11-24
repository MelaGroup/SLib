#include "ssegmentationmap.h"

using pair=std::pair<int,SSegment>;
SSegment SSegmentationMap::floodFill(int value, int x, int y)
{
    if (!isValidPos(x,y)) throw std::invalid_argument("SSegmentationMap::floodFill - Invalid position");

    int orig=ptr[y][x];
    //if (orig==value) return s;

    std::deque<QPoint> deque{{x,y}};
    ptr[y][x]=value;

    int x1=x,y1=y,x2=x,y2=y;
    int power=1;
    while(!deque.empty())
    {
        int fx=deque.front().x(),fy=deque.front().y();

        for (int dy=-1;dy<=1;++dy)
            for (int dx=-1;dx<=1;++dx)
            {
                int nx=fx+dx,ny=fy+dy;
                if (isValidPos(nx,ny)&& ptr[ny][nx]==orig)
                {
                    ptr[ny][nx]=value;
                            deque.push_back({nx,ny});

                }
            }
        deque.pop_front();
        ++power;
        if (fx<x1) x1=fx;
        if (fy<y1) y1=fy;
        if (x2<fx) x2=fx;
        if (y2<fy) y2=fy;
    }
    return SSegment(x1,y1,x2-x1+1,y2-y1+1,power);
}

SSegment &SSegmentationMap::operator[](int id)
{
    if (!isExist(id)) throw std::invalid_argument("SSegmentation[] - id doesn't exist");
    return map[id];
}

bool SSegmentationMap::isExist(int id)
{return map.find(id)!=map.end();}

void SSegmentationMap::join(int id1, int id2)
{
    if (!isExist(id1) || !isExist(id2))
    {
        qDebug()<<"("<<id1<<";"<<id2<<")";
        throw std::invalid_argument("SSegmentationMap::join -id1 or id2 doesn't exist");
    }
    map[id1]+=map[id2];
    SSegment& seg = map[id2];
    for (int y=seg.y;y<(seg.y+seg.h);++y)
        for(int x=seg.x;x<(seg.x+seg.w);++x)
            if (ptr[y][x]==id2) ptr[y][x]=id1;
    map.erase(id2);
}

int SSegmentationMap::joinToEnviroment(int id)
{

    if (!isExist(id)) throw std::invalid_argument("SSegmentationMap::joinToPrevious - id doesn't exist");

    SSegment& seg = map[id];
    std::map<int,int> counter_map;
    for (int y=seg.y-1;y<(seg.y+seg.h+2);++y)
        for(int x=seg.x-1;x<(seg.x+seg.w+2);++x)
        {
            //assert(ptr[y][x]==0 || ptr[y][x]==1);
            if (isValidPos(x,y))
                ++counter_map[ptr[y][x]];
        }

    counter_map.erase(id);
    int enviroment_id=(counter_map.begin()->first);
    for(auto it=counter_map.begin();it!=counter_map.end();++it)
        if (counter_map[enviroment_id]<it->second) enviroment_id=it->first;

    join(enviroment_id,id);
    return enviroment_id;
}

void SSegmentationMap::combineSmallWithLarge(int power_threshold)
{
    int min_id=map.begin()->first;
    int max_id=map.rbegin()->first;
    for(int id=min_id;id<=max_id;++id)
        if (isExist(id) && map[id].power<power_threshold)
            joinToEnviroment(id);
}

void SSegmentationMap::postThreshold()
{
    int black_id=-1,white_id=2;

    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int pix=ptr[y][x];          
            if (pix==0)
            {
                auto Segment=floodFill(black_id,x,y);
                map.insert(pair(black_id,Segment));
                --black_id;
            }
            if (pix==1)
            {
                auto Segment=floodFill(white_id,x,y);
                map.insert(pair(white_id,Segment));
                ++white_id;
            }
        }
}



QImage SSegmentationMap::toImage() const
{
    QImage img(_width,_height,QImage::Format_RGB888);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,255);
    std::map<int,QColor> colors;

    for (auto it=map.begin();it!=map.end();++it)
    {
        QColor rand_color(dist(gen),dist(gen),dist(gen));
        colors.insert(std::pair<int,QColor>(it->first,rand_color));
    }

    for(int y=0;y<img.height();++y)
        for(int x=0;x<img.width();++x)
            img.setPixelColor(x,y,colors[ptr[y][x]]);

    return img;
}

void SSegment::operator+=(const SSegment &seg)
{
    int x1,y1,x2,y2;
    x1=(x<seg.x)?x:seg.x;
    y1=(y<seg.y)?y:seg.y;
    x2=(x+w<seg.x+seg.w)?(seg.x+seg.w-1):(x+w-1);
    y2=(y+h<seg.y+seg.h)?(seg.y+seg.h-1):(y+h-1);

    x=x1,y=y1,w=x2-x1+1,h=y2-y1+1;
    power+=seg.power;
}
