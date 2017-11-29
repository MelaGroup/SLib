#ifndef SFUNCTORS_H
#define SFUNCTORS_H
#include <QImage>
#include <SLib/simagestatistics.h>


class SFunctor
{
protected:
    double r=11/32,g=0.5,b=5/32;
public:
    SFunctor(){}
    SFunctor(double r,double g,double b):r(r),g(g),b(b){}
    SFunctor(const std::initializer_list<double>& args);
    SFunctor(const SVector_3D& vec):r(vec.x),g(vec.y),b(vec.z){}
    SFunctor& operator=(const SFunctor& other);
    int operator()(const QColor& pixel) const;
};

class SMainComponents
{
    SVector_3D eigenvalues;
    SMatrix_3x3 eigenvectors;
public:
    SMainComponents(const QImage& src);
    SFunctor first() const {return eigenvectors.col(1);}
    SFunctor second() const {return eigenvectors.col(2);}
    SFunctor third() const {return eigenvectors.col(3);}
};





#endif // SFUNCTORS_H
