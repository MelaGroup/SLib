#ifndef SCONVEYOR_H
#define SCONVEYOR_H
#include <list>
#include <SLib/Processing/sprocessing.h>

class SConveyor: public SProcessing
{
    std::list<SProcessing*> conveyor;
public:
    SConveyor(){}
    void addProcessing(SProcessing* p);
    SMatrix bypass(const SMatrix& src);
    ~SConveyor();
};

#endif // SCONVEYOR_H
