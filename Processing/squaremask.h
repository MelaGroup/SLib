#ifndef SQUAREMASK_H
#define SQUAREMASK_H

#include <SLib/Processing/sprocessing.h>
#include <SLib/smatrix.h>

class SquareMask : public SProcessing
{
private:
    int side = 0;
    int radius = side/2;
    int square = side*side;
    int **mask_v = nullptr;
    int **mask_h = nullptr;
    int **mask_r = nullptr;
    int **mask_l = nullptr;
public:
    SquareMask(int a, std::vector<int> profile);
    SMatrix bypass(const SMatrix& src);
    ~SquareMask();
};

#endif // SQUAREMASK_H
