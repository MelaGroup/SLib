#ifndef STEACHER_H
#define STEACHER_H
#include <SLib/smatrix.h>
#include <SLib/sabstractfeatures.h>
class STeacher: public SAbstractFeatures
{
    std::map<int,int> marks;
    int segment_class=0;
    double necessity_thrs=0.10;
public:
    STeacher ();
    STeacher(std::map<int,int>&& marks,double necessity_threshold=0.10);
    void rebuild(const SMatrix& img, bool ignore_zero=true);
    std::list<std::string> getHeader(const std::string& predicat="");
    std::list<double> getFeatures();
};

#endif // STEACHER_H
