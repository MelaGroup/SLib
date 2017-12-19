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
/*
SSegment SSegmentationMap::floodLines(int x, int y,int value,int orig)
{
    int llim=x,rlim=x;
    //(at(llim,y,!orig)==orig)
    while (isValidPos(llim,y) && get(llim,y)==orig) --llim;
    while (isValidPos(rlim,y) && get(rlim,y)==orig) ++rlim;
    ++llim;--rlim;
    for (int nx=llim;nx<=rlim;++nx)
        ptr[y][nx]=value;

    SSegment segment(llim,y,rlim-llim+1,1,rlim-llim+1);
    if (y-1>=0)
    {
        for (int nx = llim;nx<=rlim;++nx)
            if (ptr[y-1][nx]!=value && ptr[y-1][nx]==orig)
                segment+=floodLines(nx,y-1,value,orig);
    }
    if (y+1<height())
    {
        for (int nx = llim;nx<=rlim;++nx)
            if (ptr[y+1][nx]!=value && ptr[y+1][nx]==orig)
                segment+=floodLines(nx,y+1,value,orig);
    }
    return segment;
}
*/
SSegment SSegmentationMap::floodFill(int value, int x, int y)
{
    using namespace std;
    if (!isValidPos(x,y)) throw invalid_argument("SSegmentationMap::floodFill - Invalid position");
    int orig=ptr[y][x];
    if (orig==value) throw invalid_argument("SSegmentationMap::floodFill - fill and original values must be different");
    int x1=x,y1=y,x2=x,y2=y;
    int power=0;

    deque<QPoint> points({QPoint(x,y)});
    do
    {
        QPoint p = points.front();
        points.pop_front();
        int fy = p.y(),llim=p.x(),rlim=p.x();
        while (isValidPos(llim,fy) && ptr[fy][llim]==orig) --llim;
        while (isValidPos(rlim,fy) && ptr[fy][rlim]==orig) ++rlim;
        ++llim;--rlim;

        for (int nx=llim;nx<=rlim;++nx)
            ptr[fy][nx]=value;

        power+=rlim-llim+1;
        if (llim<x1) x1=llim;
        if (fy<y1) y1=fy;
        if (x2<rlim) x2=rlim;
        if (y2<fy) y2=fy;

        if (fy-1>=0)
        {
            if (ptr[fy-1][llim]==orig)
                points.push_back({llim,fy-1});
            for (int nx = llim+1;nx<=rlim;++nx)
                if (ptr[fy-1][nx]==orig && ptr[fy-1][nx-1]!=orig)
                    points.push_back({nx,fy-1});
        }
        if (fy+1<height())
        {
            if (ptr[fy+1][llim]==orig)
                points.push_back({llim,fy+1});
            for (int nx = llim+1;nx<=rlim;++nx)
                if (ptr[fy+1][nx]==orig && ptr[fy+1][nx-1]!=orig)
                    points.push_back({nx,fy+1});
        }
    }
    while(!points.empty());
    return SSegment(x1,y1,x2-x1+1,y2-y1+1,power);
    /*
    std::deque<QPoint> deque{{x,y}};
    ptr[y][x]=value;

    int x1=x,y1=y,x2=x,y2=y;
    int power=1;
    while(!deque.empty())
    {
        int fx=deque.front().x(),fy=deque.front().y();
        vector<QPoint> neigh{{fx+1,fy},{fx,fy+1},{fx-1,fy},{fx,fy-1}};
        for (QPoint p:neigh)
            if (isValidPos(p.x(),p.y())&& ptr[p.y()][p.x()]==orig)
            {
                ptr[p.y()][p.x()]=value;
                deque.push_back(p);
            }
        deque.pop_front();
        ++power;
        if (fx<x1) x1=fx;
        if (fy<y1) y1=fy;
        if (x2<fx) x2=fx;
        if (y2<fy) y2=fy;
    }

    return SSegment(x1,y1,x2-x1+1,y2-y1+1,power);*/
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
SSegment SSegmentationMap::operator[](int id) const
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

void SSegmentationMap::onImage(QImage &img)
{
    using namespace std;
    if (!isCompatible(img))
        throw invalid_argument("SSegmentationMap::onImage - img and segmentation map must be compatible");
    map<int,QColor> colors;

    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int pix=ptr[y][x];
            if (colors.find(pix)==colors.end())
                colors[pix]=QColor(pick(0,255),pick(0,255),pick(0,255));

            for (int ny=-3;ny<=3;++ny)
                for (int nx=-3;nx<=3;++nx)
                    if (pix!=get(x+nx,y+ny,!pix))
                    {
                        img.setPixelColor(x,y,colors.at(pix));
                        break;
                    }
        }
}

SMatrix SSegmentationMap::toMatrix() const
{
    return SMatrix(*this);
}

