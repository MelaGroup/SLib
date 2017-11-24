#include "sadjacencymatrix.h"


inline void SAdjacencyMatrix::calculate(const SMatrix &img)
{    
    for(int y=0;y<img.height();++y)for(int x=0;x<img.width();++x)
        {
            int center=img(x,y);
            for(int i=-radius;i<=radius;++i)for(int j=-radius;j<=radius;++j)
            {
                int xn=x+i,yn=y+j;
                if (i!=0 && j!=0 && 0<xn && xn<img.width() && 0<yn && yn<img.height())
                {
                    int neighbor=img(xn,yn);
                    ++matrix[center][neighbor];
                }
            }
            //--matrix[center][center];//Заменить условия на более быстрые варианты
        }
}

inline void SAdjacencyMatrix::elementsSum()
{
    for(int x=0;x<256;++x)
        for(int y=0;y<256;++y)
            elements_sum+=matrix[x][y];
    assert(elements_sum!=0);
}

void SAdjacencyMatrix::checkMatrix()
{
    for(int x=0;x<256;++x)
        for(int y=0;y<256;++y)
            assert(matrix[x][y]>=0);
}

SAdjacencyMatrix::SAdjacencyMatrix(int radius)
    :radius(radius)
{
    for(int x=0;x<256;++x)
        for(int y=0;y<256;++y)
            matrix[x][y]=0;
}

SAdjacencyMatrix::SAdjacencyMatrix(const SMatrix&img, int radius)
    :SAdjacencyMatrix(radius)
{
    calculate(img);
    elementsSum();
    checkMatrix();
}

double SAdjacencyMatrix::energy() const
{
    double energy=0;
    for(int x=0;x<256;++x)
        for(int y=0;y<256;++y)
            energy+=(matrix[x][y]^2);
    return energy;
}

double SAdjacencyMatrix::entropy() const
{
    double entropy=0;
    for(int x=0;x<256;++x)
        for(int y=0;y<256;++y)
        {
            int N=matrix[x][y];
            if (0<N) entropy+=N*log(N);
        }
    return entropy;
}

double SAdjacencyMatrix::localHomogenity() const
{
    double homogenity=0;
    for(int x=0;x<256;++x)
        for(int y=0;y<256;++y)
            homogenity+=matrix[x][y]/(1+(x-y)*(x-y));
    return homogenity;
}

double SAdjacencyMatrix::maxProbability() const
{
    double max_p=0;
    for(int x=0;x<256;++x)
        for(int y=0;y<256;++y)
            if (max_p<matrix[x][y]) max_p=matrix[x][y];
    return max_p;
}

double SAdjacencyMatrix::inertiaMoment() const
{
    double iner=0;
    for(int x=0;x<256;++x)
        for(int y=0;y<256;++y)
            iner+=(x-y)*(x-y)*matrix[x][y];
    return iner;
}

double SAdjacencyMatrix::trail() const
{
    double tr=0;
    for(int i=0;i<256;++i)
        tr+=matrix[i][i];
    return tr;
}

double SAdjacencyMatrix::averageBrightness() const
{
    double av=0,buffer;
    for(int x=0;x<256;++x)
    {
        buffer=0;
        for(int y=0;y<256;++y)
            buffer+=matrix[x][y];
        av+=x*buffer;
    }
    return av;
}
/* работает странно
double SAdjacencyMatrix::correlationBrightness(double av) const
{
    if (av<0) av=averageBrightness();
    double nsum,dsum,numerator=0,denominator=0;
    for(int i=0;i<256;++i)
    {
        nsum=0,dsum=0;
        for(int j=0;j<256;++j)
        {
            nsum+=(j-av)*matrix[i][j];
            dsum+=matrix[i][j];
        }
        numerator+=(i-av)*nsum;
        denominator+=(i-av)*(i-av)*dsum;
    }
    double corr=numerator/denominator;
    return corr;
}
*/

