#ifndef SSQUAREMASK_H
#define SSQUAREMASK_H

#include <SLib/Processing/sprocessing.h>
#include <SLib/Core/smatrix.h>

class SSquareMask : public SProcessing
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
    SSquareMask(int a, std::vector<int> profile);//!переписать через initialaizer_list
    SMatrix bypass(const SMatrix& src);
    ~SSquareMask();
};

#endif // SSQUAREMASK_H
