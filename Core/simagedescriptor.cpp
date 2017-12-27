#include "simagedescriptor.h"



bool SImageDescriptor::isReady()
{ 
    const bool valid_comp = !components.empty();
    const bool valid_fs   = !all_features.empty();

    if (!valid_comp) qDebug()<<"SImageDescriptor: stop - no component ";   
    if (!valid_fs  ) qDebug()<<"SImageDescriptor: stop - no features";

    return valid_comp && valid_fs;
}

SImageDescriptor::SImageDescriptor(const QImage &img, const SSegmentationMap &seg_map):
    src(img),segments(seg_map)
{
    if (!seg_map.isCompatible(img))
        throw std::invalid_argument("SImageDescriptor: img and seg_map must be compatible");
    if (!seg_map.isValid())
        throw std::invalid_argument("SImageDescriptor: seg_map must be valid");
    if (img.isNull())
        throw std::invalid_argument("SImageDescriptor: img must be valid");
}

bool SImageDescriptor::addComponent(const std::__cxx11::string &name, SFunctor component)
{
    if (components.find(name)==components.end())
    {
        components[name]=component;
        return true;
    }
    return false;
}


bool SImageDescriptor::addFeatures(SAbstractFeatures *features)
{
    if (features!=nullptr)
    {
        all_features.push_back(features);
        return true;
    }
    return false;
}

SDataFrame SImageDescriptor::run(const std::__cxx11::string &img_predicat)
{
    using namespace std;
    SDataFrame X;
    if (isReady())
    {
        //emit status(".start:"+QString::fromStdString(img_predicat));
        qApp->processEvents();
        vector<int> ids=segments.IDs();       
        for (const pair<string,SFunctor>& c:components)
        {
            //emit status(".."+QString::fromStdString(c.first));
            qApp->processEvents();
            SMatrix plane(src,c.second);
            for (SAbstractFeatures* f:all_features)
            {                
                //emit status("..."+QString::fromStdString(typeid(*f).name()));
                qApp->processEvents();
                SDataFrame block;
                auto header=f->getHeader(c.first+"_");
                block.setHeader(header);
                for (int id:ids)
                {
                    //emit status("...."+id);
                    qApp->processEvents();
                    SMatrix segment = segments.getSegment(plane,id);
                    f->rebuild(segment,true);
                    block.newObject(img_predicat+to_string(id),f->getFeatures());
                }
                X+=block;
            }
        }
        //emit status(".end:"+QString::fromStdString(img_predicat));
    }   
    return X;
}

void SImageDescriptor::reset()
{
    components.clear();
    for(auto f:all_features) delete f;
    all_features.clear();
}
