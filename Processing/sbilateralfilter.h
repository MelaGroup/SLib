#ifndef SBILATERALFILTER_H
#define SBILATERALFILTER_H

#include <cmath>
#include <SLib/Processing/sprocessing.h>
#include <SLib/Core/smatrix.h>

class SBilateralFilter : public SProcessing
{
private:
    double sigmaD;
    double sigmaI;
public:
    SBilateralFilter(double sd, double si);
    SMatrix bypass (const SMatrix &src);
};

#endif // BILATERALFILTER_H
