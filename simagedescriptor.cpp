#include "simagedescriptor.h"



bool SImageDescriptor::isReady()
{
    const bool valid_src  = !src.isNull();
    const bool valid_comp = !components.empty();
    const bool valid_map  = segments.isValid();
    const bool valid_fit  = segments.isComatible(src);
    const bool valid_fs   = !all_features.empty();
    if (!valid_src ) qDebug()<<"SImageDescriptor: stop - invalid image";
    if (!valid_comp) qDebug()<<"SImageDescriptor: stop - no component ";
    if (!valid_map ) qDebug()<<"SImageDescriptor: stop - SSegmentation map wasn't prepared";
    if (!valid_fit ) qDebug()<<"SImageDescriptor: stop - SSegmentation map does not fit in size";
    if (!valid_fs  ) qDebug()<<"SImageDescriptor: stop - no features";
    return valid_src && valid_comp&& valid_map && valid_fit && valid_fs;
}

bool SImageDescriptor::addComponent(const std::__cxx11::string &name, const SFunctor &component)
{
    if (components.find(name)!=components.end())
    {
        components[name]=component;
        return true;
    }
    return false;
}

bool SImageDescriptor::addSegmentationMap(const SSegmentationMap &seg_map)
{
    if (seg_map.isComatible(src) && seg_map.isValid())
    {
        segments=SSegmentationMap(seg_map);
        return true;
    }
    return false;
}

bool SImageDescriptor::addFeatures(const SAbstractFeatures &features)
{
    if (typeid(features)!= typeid(const SAbstractFeatures&))
    {
        all_features.push_back(features);
        return true;
    }
    return false;
}

SDataFrame SImageDescriptor::run()
{
    using namespace std;
    SDataFrame X;
    if (isReady())
    {
        vector<int> ids=segments.IDs();
        for (auto c:components)
        {
            SMatrix plane(src,c.second);
            for (SAbstractFeatures f:all_features)
            {
                SDataFrame block;
                block.setHeader(f.getHeader(c.first+"_"));
                for (int id:ids)
                {
                    SMatrix segment = segments.getSegment(plane,id);
                    f.rebuild(segment);
                    block.newObject(to_string(id),f.getFeatures());
                }
                X+=block;
            }
        }
    }
    return X;
}
