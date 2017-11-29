#include "slinearalgebra.h"

SVector_3D::SVector_3D(const init &xyz)
{
    auto i=xyz.begin();
    x=*i;++i;
    y=*i;++i;
    z=*i;++i;
}

SVector_3D &SVector_3D::operator/=(double norma)
{
    x/=norma;y/=norma;z/=norma;
    return *this;
}

double SVector_3D::max() const
{
    double max=(x<y)?y:x;
    max=(max<z)?z:max;
    return max;
}

SVector_3D& SVector_3D::sort()
{
    double nx=x,ny=y,nz=z,buf;
    do{
        if (nx<ny)
        {buf=nx,nx=ny,ny=buf;}
        if (ny<nz)
        {buf=ny,ny=nz,nz=buf;}
    }while(!(nx>=ny && ny>=nz));
    x=nx;y=ny;z=nz;
    return *this;
}

double SVector_3D::operator[](int num)
{
    switch (num) {
    case 1:
        return x;
        break;
    case 2:
        return y;
        break;
    case 3:
        return z;
        break;
    default:
        throw std::invalid_argument("SVector_3D::[] - Bad index in SVector_3D");
        return -1;
        break;
    }
}

/*int SVector_3D::posMax() const
{
    if (y<x && z<x) return 1;
    if (x<y && z<y) return 2;
    return 3;
}*/

void SVector_3D::print() const
{
    qDebug()<<"{"<<x<<","<<y<<","<<z<<"};";
}

SVector_3D SMatrix_3x3::VieteCardanoMethod(double a, double b, double c) const
{
    SVector_3D roots;
    double q,r,r2,q3;
    double M_2PI=2.*M_PI;
    q=(a*a-3.*b)/9.; r=(a*(2.*a*a-9.*b)+27.*c)/54.;
    r2=r*r; q3=q*q*q;
    if(r2<q3)
    {
        double t=acos(r/sqrt(q3));
        a/=3.; q=-2.*sqrt(q);
        roots.x=q*cos(t/3.)-a;
        roots.y=q*cos((t+M_2PI)/3.)-a;
        roots.z=q*cos((t-M_2PI)/3.)-a;

    }
    else
    {
        double aa,bb;
        if(r<=0.) r=-r;
        aa=-pow(r+sqrt(r2-q3),1./3.);
        if(aa!=0.) bb=q/aa;
        else bb=0.;
        a/=3.; q=aa+bb; r=aa-bb;
        roots.x=q-a;
        roots.y=(-0.5)*q-a;
        roots.z=(sqrt(3.)*0.5)*fabs(r);
    }
    return roots;
}

SMatrix_3x3::SMatrix_3x3(const SVector_3D &col1, const SVector_3D &col2, const SVector_3D &col3)
{
    mat[0][0]=col1.x,mat[1][0]=col1.y,mat[2][0]=col1.z;
    mat[0][1]=col2.x,mat[1][1]=col2.y,mat[2][1]=col2.z;
    mat[0][2]=col3.x,mat[1][2]=col3.y,mat[2][2]=col3.z;
}

SVector_3D SMatrix_3x3::row(int i) const
{
    if (i<1 || i>3) throw std::invalid_argument("SMatrix_3x3::row - Row doesn't exist");
    return SVector_3D(mat[i-1][0],mat[i-1][1],mat[i-1][2]);
}

SVector_3D SMatrix_3x3::col(int j) const
{
    if (j<1 || j>3) throw std::invalid_argument("SMatrix_3x3::col - Column doesn't exist");
    return SVector_3D(mat[0][j-1],mat[1][j-1],mat[2][j-1]);
}

SMatrix_3x3 SMatrix_3x3::operator-(const SMatrix_3x3 &other) const
{
    SMatrix_3x3 sub;
    for (int i=0;i<3;++i)
        for (int j=0;j<3;++j)
            sub.mat[i][j]=mat[i][j]-other.mat[i][j];
    return sub;
}

SMatrix_3x3 SMatrix_3x3::operator*(const SMatrix_3x3& m)const
{
    SMatrix_3x3 dst;
    for(int i=1;i<4;++i)
        for(int j=1;j<4;++j)
            dst.mat[i-1][j-1]=row(i)*m.col(j);
    return dst;
}

double &SMatrix_3x3::a(int x, int y)
{
    if (!isExist(x,y))
        throw  std::invalid_argument("SMatrix_3x3::a - Element with selected indexes does not exist");
    return mat[y-1][x-1];
}

SVector_3D SMatrix_3x3::eigenvalues() const
{
    double A=-mat[0][0]-mat[1][1]-mat[2][2];
    double Buf0=(mat[1][2]*mat[2][1]);
    double Buf1=(mat[0][2]*mat[2][0]);
    double Buf2=(mat[0][1]*mat[1][0]);
    double B=(mat[0][0]*mat[1][1])+(mat[0][0]*mat[2][2])+(mat[1][1]*mat[2][2])
            -Buf2-Buf1-Buf0;

    double C=(mat[0][0]*Buf0)+(mat[1][1]*Buf1)+(mat[2][2]*Buf2)
            -(mat[0][0]*mat[1][1]*mat[2][2])
            -(mat[0][1]*mat[1][2]*mat[2][0])
            -(mat[0][2]*mat[1][0]*mat[2][1]);

    return VieteCardanoMethod(A,B,C).sort();
}

SMatrix_3x3 SMatrix_3x3::eigenvectors(const SVector_3D &eigenValues) const
{
    SMatrix_3x3 fir=(*this)-SMatrix_3x3(eigenValues.x);
    SMatrix_3x3 sec=(*this)-SMatrix_3x3(eigenValues.y);
    SMatrix_3x3 thd=(*this)-SMatrix_3x3(eigenValues.z);

    SMatrix_3x3 fs=fir*sec;
    SMatrix_3x3 ft=fir*thd;
    SMatrix_3x3 st=sec*thd;

    SVector_3D F=st.row(1)+st.row(2)+st.row(3);
    SVector_3D S=ft.row(1)+ft.row(2)+ft.row(3);
    SVector_3D T=fs.row(1)+fs.row(2)+fs.row(3);

    F/=fabs(F.x)+fabs(F.y)+fabs(F.z);
    S/=fabs(S.x)+fabs(S.y)+fabs(S.z);
    T/=fabs(T.x)+fabs(T.y)+fabs(T.z);

    return SMatrix_3x3(F,S,T);
}

/*SVector_3D SMatrix_3x3::eigenvector(double eigenvalue) const
{
    SMatrix_3x3 m=(*this)-SMatrix_3x3(eigenvalue);
    SVector_3D b;
    b.x=(m.mat[0][0]*m.mat[1][2] - m.mat[0][2]*m.mat[1][0])
            *(m.mat[1][0]*m.mat[2][1] - m.mat[2][0]*m.mat[1][1]);

    b.y=(m.mat[1][1]*m.mat[2][2] - m.mat[1][2]*m.mat[2][1])
            *(m.mat[0][1]*m.mat[1][0] - m.mat[1][1]*m.mat[0][0]);

    b.z=(m.mat[0][1]*m.mat[1][0] - m.mat[1][1]*m.mat[0][0])
            *(m.mat[1][0]*m.mat[2][1] - m.mat[2][0]*m.mat[1][1]);
    double sum=b.x+b.y+b.z;
    b.x/=sum;b.y/=sum;b.z/=sum;
    return b;
}*/
