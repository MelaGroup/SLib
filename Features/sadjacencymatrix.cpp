#include "sadjacencymatrix.h"


inline void SAdjacencyMatrix::calculate(const SMatrix &img)
{    
    for(int y=0;y<img.height();++y)for(int x=0;x<img.width();++x)
        {
            int center=img(x,y);
            for(int i=-radius;i<=radius;++i)for(int j=-radius;j<=radius;++j)
            {
                int xn=x+i,yn=y+j;                
                if (img.isValidPos(xn,yn))
                {
                    int neighbor=img(xn,yn);
                    ++matrix[center][neighbor];
                }                
            }
            --matrix[center][center];
        }
}

void SAdjacencyMatrix::ignoreZero()
{
    for( auto& m: matrix)
    {
        matrix[0][m.first]=0;
        matrix[m.first][0]=0;
    }
}


SAdjacencyMatrix::SAdjacencyMatrix(int radius):radius(radius)
{}

SAdjacencyMatrix::SAdjacencyMatrix(const SMatrix&img, int radius, bool ignore_zero):radius(radius)
{rebuild(img,ignore_zero);}

double SAdjacencyMatrix::energy()
{
    using namespace std;
    double energy=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            energy+=(matrix[x][y]*matrix[x][y]);
        }
    }
    energy/=double(elements);
    assert(energy>=0);
    return energy;
}

double SAdjacencyMatrix::entropy()
{
    double entropy=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            int N=matrix[x][y];
            if (0<N) entropy+=N*log(N);
        }
    }
    entropy/=double(elements);
    return entropy;
}

double SAdjacencyMatrix::localHomogenity()
{
    double homogenity=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            homogenity+=matrix[x][y]/(1+(x-y)*(x-y));
        }
    }
    homogenity/=double(elements);
    return homogenity;
}

double SAdjacencyMatrix::maxProbability()
{
    double max_p=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            if (max_p<matrix[x][y]) max_p=matrix.at(x).at(y);
        }
    }
    max_p/=double(elements);
    return max_p;
}

double SAdjacencyMatrix::inertiaMoment()
{
    double iner=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            iner+=(x-y)*(x-y)*matrix[x][y];
        }
    }
    iner/=double(elements);
    return iner;
}

double SAdjacencyMatrix::trail()
{
    double tr=0;
    for(auto& r: matrix)
    {
        int i=r.first;
        tr+=matrix[i][i];
    }
    tr/=double(elements);
    assert(tr>=0);
    return tr;
}

double SAdjacencyMatrix::averageBrightness()
{
    double av=0,buffer;
    for(auto& r: matrix)
    {
        int y=r.first;
        buffer=0;
        for(auto& c: matrix)
        {
            int x=c.first;
            buffer+=matrix[x][y];
        }
        av+=y*buffer;
    }
    av/=double(elements);
    assert(av>=0);
    return av;
}

bool SAdjacencyMatrix::operator==(const SAdjacencyMatrix &other) const
{
    return (matrix==other.matrix);
}

void SAdjacencyMatrix::rebuild(const SMatrix &img, bool ignore_zero)
{
    elements=img.width()*img.height();
    calculate(img);
    if (ignore_zero)
    {
        for(int y=0;y<img.height();++y)for(int x=0;x<img.width();++x)
            if (img(x,y)==0)
                --elements;
        assert(elements>=0);
        ignoreZero();
    }
}

std::list<std::__cxx11::string> SAdjacencyMatrix::getHeader(const std::string& predicat)
{
    using namespace std;
    list<string> header;
    string str=predicat+to_string(radius);
    header.push_back(str+"_Energy");
    header.push_back(str+"_ENT");
    header.push_back(str+"_LUN");
    header.push_back(str+"_MPR");
    header.push_back(str+"_CON");
    header.push_back(str+"_TR");
    header.push_back(str+"_AV");
    return header;
}

std::list<double> SAdjacencyMatrix::getFeatures()
{
    std::list<double> features;
    features.push_back(energy());
    features.push_back(entropy());
    features.push_back(localHomogenity());
    features.push_back(maxProbability());
    features.push_back(inertiaMoment());
    features.push_back(trail());
    features.push_back(averageBrightness());
    return features;
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


