#ifndef SABSTRACTFEATURES_H
#define SABSTRACTFEATURES_H

#include <map>
#include <list>
#include <SLib/smatrix.h>

class SAbstractFeatures
{
public:
    SAbstractFeatures(){}
    virtual void rebuild(const SMatrix& src, bool ignore_zero=true){}
    virtual std::list<std::string> getHeader(const std::string& predicat){}
    virtual std::list<double> getFeatures(){}
};

#endif // SABSTRACTFEATURES_H
