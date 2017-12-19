#ifndef SLINEARALGEBRA_H
#define SLINEARALGEBRA_H
#include <initializer_list>
#include <QDebug>
#include <math.h>
using init=std::initializer_list<double>;

struct SVector_3D
{
    double x,y,z;

    SVector_3D(double x=0,double y=0,double z=0);
    SVector_3D(const init& xyz);
    SVector_3D operator+(const SVector_3D& other)const;
    SVector_3D operator-(const SVector_3D& other)const;
    double operator*(const SVector_3D& other)const;
    SVector_3D& operator/=(double norma);
    SVector_3D& sort();
    double operator[](int num);
    bool operator==(const SVector_3D& other);
    void print() const;
};

class SMatrix_3x3
{
protected:
    double mat[3][3];   

    inline bool isExist(int x,int y){return 0<x && x<4 && 0<y && y<4;}
    SVector_3D VieteCardanoMethod(double a,double b,double c) const;

public:
    SMatrix_3x3(double diag_value=1):mat{{diag_value,0,0},{0,diag_value,0},{0,0,diag_value}}{}
    SMatrix_3x3(const SVector_3D& col1,const SVector_3D& col2,const SVector_3D& col3);
    SVector_3D row(int i) const;
    SVector_3D col(int j) const;
    SMatrix_3x3 operator-(const SMatrix_3x3& other)const;
    SMatrix_3x3 operator*(const SMatrix_3x3& m) const;

    bool operator==(const SMatrix_3x3& other) const;

    double& a(int x,int y);
    SVector_3D eigenvalues() const;
    SMatrix_3x3 eigenvectors(const SVector_3D& eigenValues) const;
    void print();
};

#endif // SLINEARALGEBRA_H
