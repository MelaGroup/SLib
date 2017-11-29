#include "ssegmentationmap.h"

bool SSegment::operator<(const SSegment &seg) const
{
    return power<seg.power;
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

QRect SSegment::toRect(){return {x,y,w,h};}


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

std::default_random_engine &SSegmentationMap::global_urng() const
{
    static std::default_random_engine u{};
    return u;
}

int SSegmentationMap::pick(int l, int r) const
{
    static std::uniform_int_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(global_urng(),parm_t{l,r});
}

SSegmentationMap &SSegmentationMap::operator=(const SSegmentationMap &other)
{
    SMatrix::operator=(other);
    segments=other.segments;
    return *this;
}

SSegment &SSegmentationMap::operator[](int id)
{
    return segments.at(id);
}

void SSegmentationMap::join(int id1, int id2)
{
    segments.at(id1)+=segments.at(id2);
    SSegment& seg = segments.at(id2);
    for (int y=seg.y;y<(seg.y+seg.h);++y)
        for(int x=seg.x;x<(seg.x+seg.w);++x)
            if (ptr[y][x]==id2) ptr[y][x]=id1;
    segments.erase(id2);
}

int SSegmentationMap::joinToEnviroment(int id)
{
    SSegment& seg = segments.at(id);
    std::map<int,int> counters;
    for (int y=seg.y-1;y<(seg.y+seg.h+2);++y)
        for(int x=seg.x-1;x<(seg.x+seg.w+2);++x)
        {        
            if (isValidPos(x,y))
                ++counters[ptr[y][x]];
        }
    counters.erase(id);
    
    int eid=(counters.begin()->first);
    for(auto c: counters)
        if (counters[eid]<c.second) eid=c.first;

    join(eid,id);
    return eid;
}

std::vector<int> SSegmentationMap::IDs() const
{
    std::vector<int> id_vec;
    id_vec.reserve(segments.size());
    for (auto s:segments)
        id_vec.push_back(s.first);
    return id_vec;
}

int SSegmentationMap::IDsmallest() const
{
    int min_id=segments.begin()->first;
    for (auto pair:segments)
        if (pair.second < segments.at(min_id)) min_id=pair.first;
    return min_id;
}

int SSegmentationMap::IDlargest() const
{

    int max_id=segments.begin()->first;
    for (auto pair:segments)
        if (segments.at(max_id)< pair.second) max_id=pair.first;
    return max_id;
}

SMatrix SSegmentationMap::getSegment(const SMatrix &original, int id) const
{  
    const SSegment& seg=segments.at(id);
    SMatrix ret = original.copy(seg.x,seg.y,seg.w,seg.h);
    for(int y=0;y<ret.height();++y)
        for(int x=0;x<ret.width();++x)
            if (ptr[seg.y+y][seg.x+x]!=id)
                ret(x,y)=0;
    return ret;
}

void SSegmentationMap::combine(int power_threshold)
{
    int min_id=segments.begin()->first;
    int max_id=segments.rbegin()->first;
    for(int id=min_id;id<=max_id;++id)
        if (isExist(id) && segments[id].power<power_threshold)
            joinToEnviroment(id);
}

void SSegmentationMap::connectedAreas()
{
    segments.clear();
    if (max()>=0)(*this)+=(-max()-1);//все оригинальные элементы теперь <0
    int id=0;
    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int pix = ptr[y][x];
            if (pix<0)
            {
                auto seg=floodFill(id,x,y);
                segments.insert({id,seg});
                ++id;
            }
        }
}

bool SSegmentationMap::isExist(int id) const
{
    return segments.find(id)!=segments.end();
}

bool SSegmentationMap::isValid() const
{
    return !(segments.empty());
}

bool SSegmentationMap::isCompatible(const SMatrix &src) const
{
    return SMatrix::isCompatible(src);
}

bool SSegmentationMap::isCompatible(const QImage &src) const
{
    return SMatrix::isCompatible(src);
}



QImage SSegmentationMap::toImage() const
{
    QImage img(_width,_height,QImage::Format_RGB888);

    using namespace std;
    map<int,QColor> colors;

    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int pix=ptr[y][x];
            if (colors.find(pix)==colors.end())
                colors[pix]=QColor(pick(0,255),pick(0,255),pick(0,255));
            img.setPixelColor(x,y,colors.at(pix));
        }

    return img;
}

SMatrix SSegmentationMap::toMatrix() const
{
    return SMatrix(*this);
}

