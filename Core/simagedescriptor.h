#ifndef SIMAGEDESCRIPTOR_H
#define SIMAGEDESCRIPTOR_H

#include <typeinfo>
#include <QApplication>
#include <SLib/Core/ssegmentationmap.h>
#include <SLib/Features/sabstractfeatures.h>
#include <SLib/Core/sdataframe.h>

class SImageDescriptor//: public QObject
{
   // Q_OBJECT


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
    virtual void reset();
    virtual SDataFrame run(const std::string& img_predicat="");
    ~SImageDescriptor(){for(auto f:all_features) delete f;}

//signals:
    //void status(QString);

};

#endif // SIMAGEDESCRIPTOR_H
