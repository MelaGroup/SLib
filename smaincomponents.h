#ifndef SMAINCOMPONENTS_H
#define SMAINCOMPONENTS_H

#include <QImage>
#include <SLib/sfunctors.h>
#include <SLib/slinearalgebra.h>

class SMainComponents
{
    SVector_3D eigenvalues;
    SMatrix_3x3 eigenvectors;
public:
    SMainComponents(const QImage& src);
    SVector_3D Eigenvalues() const;
    SMatrix_3x3 Eigenvectors()const;
    SFunctor first() const;
    SFunctor second() const;
    SFunctor third() const;
    bool operator==(const SMainComponents& other);
};

#endif // SMAINCOMPONENTS_H
