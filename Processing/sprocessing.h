#ifndef SPROCESSING_H
#define SPROCESSING_H
#include <deque>
#include <vector>
#include <SLib/Core/smatrix.h>

class SProcessing
{
public:
    SProcessing(){}
    virtual SMatrix bypass(const SMatrix&){return SMatrix(0,0);}
    virtual ~SProcessing(){}
};


// Надо сделать из функций наследников
int floodFill(SMatrix& src,int value,int x,int y);
void binLaplace(SMatrix& src);


#endif // SPROCESSING_H
