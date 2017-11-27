#ifndef SIMAGEDESCRIPTOR_H
#define SIMAGEDESCRIPTOR_H

#include <typeinfo>
#include <SLib/smatrix.h>
#include <SLib/ssegmentationmap.h>
#include <SLib/sabstractfeatures.h>
#include <SLib/sdataframe.h>

class SImageDescriptor
{
    QImage src;
    std::map<std::string,SFunctor> components;
    SSegmentationMap segments;
    std::list<SAbstractFeatures*> all_features;
protected:
    bool isReady();

public:
    SImageDescriptor(){}
    void loadImage (const QString& fileName){src = QImage(fileName);}
    void loadImage (const QImage& img){src=img;}
    bool addComponent(const std::string& name,const SFunctor& component);
    bool addSegmentationMap(const SSegmentationMap& seg_map);
    bool addFeatures(SAbstractFeatures* features);
    virtual SDataFrame run(const std::string& img_predicat="");
    virtual void reset();

    ~SImageDescriptor(){for(auto f:all_features) delete f;}
};

#endif // SIMAGEDESCRIPTOR_H
