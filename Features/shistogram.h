#ifndef SHISTOGRAM_H
#define SHISTOGRAM_H
#include <Slib/Features/sabstractfeatures.h>
#include <SLib/Core/smatrix.h>
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
    inline double min() const {return min_key;}
    inline double max() const {return max_key;}
    inline double span() const {return max_key - min_key;}
    double MX();
    double SD(double av=std::numeric_limits<double>::max());

    virtual void rebuild(const SMatrix& src, bool ignore_zero=true);
    std::list<std::string> getHeader(const std::string& predicat="");
    std::list<double> getFeatures();
    virtual QImage toImage();
};

#endif // SHISTOGRAM_H
