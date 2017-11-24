#ifndef SIMAGESTATISICS_H
#define SIMAGESTATISICS_H
#include <QImage>
#include <SLib/slinearalgebra.h>


class SCovarienceMatrix:public SMatrix_3x3
{
public:
    SCovarienceMatrix(const QImage& src);
};

#endif // SIMAGESTATISICS_H
