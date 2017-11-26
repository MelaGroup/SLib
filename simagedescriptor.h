#ifndef SIMAGEDESCRIPTOR_H
#define SIMAGEDESCRIPTOR_H

#include <SLib/smatrix.h>
#include <SLib/ssegmentationmap.h>
#include <SLib/sabstractfeatures.h>
#include <SLib/sdataframe.h>

class SImageDescriptor
{
    QImage src;
    std::map<std::string,SFunctor> components;
    SSegmentationMap segments;
    std::list<SAbstractFeatures> all_features;
protected:
    bool isReady();

public:
    SImageDescriptor(){}
    void loadImage (const QString& fileName){src = QImage(fileName);}
    bool addComponent(const std::string& name,const SFunctor& component);
    bool addSegmentationMap(const SSegmentationMap& seg_map);
    bool addFeatures(const SAbstractFeatures& features);
    virtual SDataFrame run();
};

#endif // SIMAGEDESCRIPTOR_H
