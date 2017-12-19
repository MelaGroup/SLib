#ifndef SEDGEDETECTION_H
#define SEDGEDETECTION_H

#include "SLib/Core/smatrix.h"
#include "SLib/Processing/sprocessing.h"

class SSobelOperator : public SProcessing
{
private:
    int Gx[3][3] = { {1,0,-1},
                     {2,0,-2},
                     {1,0,-1} };
    int Gy[3][3] = { {1,2,1},
                     {0,0,0},
                     {-1,-2,-1} };
public:
    SSobelOperator(){}
    SMatrix bypass(SMatrix &src);
};

class SPrewittOperator : public SProcessing
{
private:
    int Gx[3][3] = { {-1,0,1},
                     {-1,0,1},
                     {-1,0,1} };
    int Gy[3][3] = { {-1,-1,-1},
                     {0,0,0},
                     {1,1,1} };
public:
    SPrewittOperator(){}
    SMatrix bypass(SMatrix &src);
};

class SRobertsOperator : public SProcessing
{
private:
    int Gx[2][2] = { {1,0},
                     {0,-1} };
    int Gy[2][2] = { {0,1},
                     {-1,0} };
public:
    SRobertsOperator(){}
    SMatrix bypass(SMatrix &src);
};



#endif // EDGEDETECTION_H
