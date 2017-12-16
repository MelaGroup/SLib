#ifndef SABSTRACTCLASSIFIER_H
#define SABSTRACTCLASSIFIER_H

#include <SLib/sdataframe.h>

class SAbstractClassifier
{
public:
    SAbstractClassifier(){}
    virtual void fit(const SDataFrame&,const std::vector<int>&)
    {}
    virtual std::vector<int> predict(const SDataFrame&)
    {return {};}
    virtual ~SAbstractClassifier(){}
};

#endif // SABSTRACTCLASSIFIER_H
