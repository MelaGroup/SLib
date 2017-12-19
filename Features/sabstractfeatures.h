#ifndef SABSTRACTFEATURES_H
#define SABSTRACTFEATURES_H

#include <map>
#include <list>
#include <SLib/Core/smatrix.h>

class SAbstractFeatures
{
public:
    SAbstractFeatures(){}
    virtual void rebuild(const SMatrix& , bool){}
    virtual std::list<std::string> getHeader(const std::string&){return {};}
    virtual std::list<double> getFeatures(){return {};}
    virtual ~SAbstractFeatures(){}
};

#endif // SABSTRACTFEATURES_H
