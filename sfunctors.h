#ifndef SFUNCTORS_H
#define SFUNCTORS_H
#include <QImage>
#include <simagestatistics.h>


class SFunctor
{
protected:
    double r=11/32,g=0.5,b=5/32;
public:
    SFunctor(){}
    SFunctor(double r,double g,double b):r(r),g(g),b(b){}
    int operator()(const QColor& pixel) const
    {return int(r*double(pixel.red())+g*double(pixel.green())+b*double(pixel.blue()));}
};

class SMainComponent:public SFunctor
{
    SVector_3D eigenvalues;
    SMatrix_3x3 eigenvectors;
    int component_number=1;
public:
    SMainComponent(const QImage& src);
    void useComponent(int component_1_2or3);
    int operator()(const QColor& pixel) const
    {return int(r*double(pixel.red())+g*double(pixel.green())+b*double(pixel.blue()));}
};




#endif // SFUNCTORS_H
