#include "sconveyor.h"



void SConveyor::addProcessing(SProcessing *p)
{
    conveyor.push_back(p);
}

SMatrix SConveyor::bypass(const SMatrix &src)
{
    SMatrix ret(src);
    for (SProcessing* p:conveyor)
        ret=p->bypass(ret);
    return ret;
}

SConveyor::~SConveyor()
{
    for (SProcessing* p:conveyor)
        delete p;
}
