#ifndef SSEGMENTATIONMAP_H
#define SSEGMENTATIONMAP_H

#include <random>
#include <set>
#include <deque>
#include <tuple>
#include <typeinfo>
#include <algorithm>
#include <SLib/Core/smatrix.h>
#include <assert.h>



struct SSegment
{
    int x,y,w,h;
    int power;

    SSegment(int x=0,int y=0,int w=0,int h=0,int power = -1):x(x),y(y),w(w),h(h),power(power){}    
    bool operator<(const SSegment& seg) const;
    void operator+=(const SSegment& seg);
    QRect toRect();
};

class SSegmentationMap:protected SMatrix
{
protected:
    std::map<int,SSegment> segments;
    std::default_random_engine& global_urng() const;

    int pick(int l,int r) const;    
    SSegment floodFill(int value,int x,int y);
public:
    SSegmentationMap(const SMatrix& src):SMatrix(src){connectedAreas();}
    SSegmentationMap(SMatrix&& src):SMatrix(src){connectedAreas();}
    SSegmentationMap(const SSegmentationMap& src):SMatrix(src),segments(src.segments){}
    SSegmentationMap(int width=0,int height=0):SMatrix(width,height){}
    SSegmentationMap &operator=(const SSegmentationMap &other);


    void join(int id1,int id2);
    int joinToEnviroment(int id);
    void combine(int power_threshold);

    void connectedAreas();

    bool isExist(int id) const;
    bool isValid() const;
    bool isCompatible(const SMatrix & src) const;
    bool isCompatible(const QImage& src) const;

    std::vector<int> IDs()const;
    int IDsmallest()const;
    int IDlargest()const;
    SMatrix getSegment(const SMatrix& original,int id) const;
    void forgetSegment(int id){segments.erase(id);}


    SSegment& operator[](int id);
    SSegment operator[](int id) const;
    QImage toImage() const;
    virtual void onImage(QImage& img);
    virtual SMatrix toMatrix() const;
};

#endif // SSEGMENTATIONMAP_H
