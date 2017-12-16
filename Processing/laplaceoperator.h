#ifndef LAPLACEOPERATOR_H
#define LAPLACEOPERATOR_H

#include "SLib/Processing/sprocessing.h"
#include "SLib/smatrix.h"

class LaplaceOperator
{
private:
    int lo [3][3] = { {0,1,0},
                      {1,-4,1},
                      {0,1,0} };
public:
    LaplaceOperator();
    SMatrix bypass(const SMatrix& src);
};

#endif // LAPLACEOPERATOR_H
