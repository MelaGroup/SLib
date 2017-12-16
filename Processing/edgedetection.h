#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H

#include "SLib/smatrix.h"
#include "SLib/Processing/sprocessing.h"

class SobelOperator : public SProcessing
{
private:
    int Gx[3][3] = { {1,0,-1},
                     {2,0,-2},
                     {1,0,-1} };
    int Gy[3][3] = { {1,2,1},
                     {0,0,0},
                     {-1,-2,-1} };
public:
    SobelOperator(){}
    SMatrix bypass(SMatrix &src);
};

class PrewittOperator : public SProcessing
{
private:
    int Gx[3][3] = { {-1,0,1},
                     {-1,0,1},
                     {-1,0,1} };
    int Gy[3][3] = { {-1,-1,-1},
                     {0,0,0},
                     {1,1,1} };
public:
    PrewittOperator(){}
    SMatrix bypass(SMatrix &src);
};

class RobertsOperator : public SProcessing
{
private:
    int Gx[2][2] = { {1,0},
                     {0,-1} };
    int Gy[2][2] = { {0,1},
                     {-1,0} };
public:
    RobertsOperator(){}
    SMatrix bypass(SMatrix &src);
};



#endif // EDGEDETECTION_H
