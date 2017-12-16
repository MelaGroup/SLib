#ifndef BILATERALFILTER_H
#define BILATERALFILTER_H

#include <cmath>
#include <SLib/Processing/sprocessing.h>
#include <SLib/smatrix.h>

class BilateralFilter : public SProcessing
{
private:
    double sigmaD;
    double sigmaI;
public:
    BilateralFilter(double sd, double si);
    SMatrix bypass (const SMatrix &src);
};

#endif // BILATERALFILTER_H
