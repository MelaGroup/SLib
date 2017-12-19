#ifndef SIMAGEDESCRIPTOR_H
#define SIMAGEDESCRIPTOR_H

#include <typeinfo>
#include <SLib/Core/ssegmentationmap.h>
#include <SLib/Features/sabstractfeatures.h>
#include <SLib/Core/sdataframe.h>

class SImageDescriptor
{
    const QImage& src;
    const SSegmentationMap& segments;
    std::map<std::string,SFunctor> components;
    std::list<SAbstractFeatures*> all_features;
protected:
    bool isReady();

public:
    SImageDescriptor(const QImage& img,const SSegmentationMap& seg_map);

    bool addComponent(const std::string& name,SFunctor component);
    bool addFeatures(SAbstractFeatures* features);
    virtual SDataFrame run(const std::string& img_predicat="");
    virtual void reset();
    ~SImageDescriptor(){for(auto f:all_features) delete f;}
};

#endif // SIMAGEDESCRIPTOR_H
