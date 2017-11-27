#ifndef SSEGMENTATIONMAP_H
#define SSEGMENTATIONMAP_H

#include <random>
#include <deque>
#include <algorithm>
#include <SLib/smatrix.h>
#include <assert.h>
#include <typeinfo>


struct SSegment
{
    int x,y,w,h;
    int power;

    SSegment(int x=0,int y=0,int w=0,int h=0,int power = -1):x(x),y(y),w(w),h(h),power(power){}    
    bool operator<(const SSegment& seg) const
        {return power<seg.power;}
    void operator+=(const SSegment& seg);
    QRect toRect(){return {x,y,w,h};}
};




class SSegmentationMap:public SMatrix
{
    std::map<int,SSegment> segments;
public:
    SSegmentationMap(const SMatrix& src):SMatrix(src){}
    SSegmentationMap(const SSegmentationMap& src):SMatrix(src),segments(src.segments){}
    SSegmentationMap(int width=0,int height=0):SMatrix(width,height){}
    SSegmentationMap &operator=(const SSegmentationMap &other);

    SSegment floodFill(int value,int x,int y);
    void join(int id1,int id2);
    int joinToEnviroment(int id);
    void combine(int power_threshold);

    void connectedAreas();

    inline bool isExist(int id) const{return segments.find(id)!=segments.end();}
    inline bool isValid() const{return !(segments.empty());}
    inline bool isComatible(const SMatrix& src) const{return (src.width()==_width && src.height()==_height);}
    inline bool isComatible(const QImage& src) const{return (src.width()==_width && src.height()==_height);}

    std::vector<int> IDs()const;
    int IDsmallest()const;
    int IDlargest()const;
    SMatrix getSegment(const SMatrix& original,int id) const;
    void forgetSegment(int id){segments.erase(id);}


    SSegment& operator[](int id);
    QImage toImage() const;
};

#endif // SSEGMENTATIONMAP_H
