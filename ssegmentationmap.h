#ifndef SSEGMENTATIONMAP_H
#define SSEGMENTATIONMAP_H

#include <random>
#include <deque>
#include <SLib/smatrix.h>
#include <assert.h>



struct SSegment
{
    int x,y,w,h;
    int power;

    SSegment(int x=0,int y=0,int w=0,int h=0,int power = -1):x(x),y(y),w(w),h(h),power(power){}
    bool operator<(const SSegment& seg) const
        {return power<seg.power;}
    void operator+=(const SSegment& seg);
};


class SSegmentationMap:public SMatrix
{
protected:
    std::map<int,SSegment> map;

    SSegment floodFill(int value,int x,int y);
public:
    SSegmentationMap(const SMatrix& src):SMatrix(src){}
    SSegmentationMap(int width=0,int height=0):SMatrix(width,height){}

    SSegment& operator[](int id);

    bool isExist(int id);
    void join(int id1,int id2);
    int joinToEnviroment(int id);
    void combineSmallWithLarge(int power_threshold);

    void postThreshold();

    QImage toImage() const;
};

#endif // SSEGMENTATIONMAP_H
