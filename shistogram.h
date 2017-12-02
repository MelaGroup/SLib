#ifndef SHISTOGRAM_H
#define SHISTOGRAM_H
#include <Slib/sabstractfeatures.h>
#include <SLib/smatrix.h>
#include <limits>

class SHistogram:public SAbstractFeatures
{   
    std::map<int,int> bars;
    int min_key,max_key;
    int sum=0;

    inline void ignoreZero();
public:
    SHistogram(){}
    SHistogram(const SMatrix& src, bool ignore_zero=true);
    inline int min() const {return min_key;}
    inline int max() const {return max_key;}
    inline int span() const {return max_key - min_key;}
    int MX();
    int DX(int av=std::numeric_limits<int>::max());

    virtual void rebuild(const SMatrix& src, bool ignore_zero=true);
    std::list<std::string> getHeader(const std::string& predicat="");
    std::list<double> getFeatures();
    virtual QImage toImage();
};

#endif // SHISTOGRAM_H
