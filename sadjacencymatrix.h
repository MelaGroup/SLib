#ifndef SADJACENCYMATRIX_H
#define SADJACENCYMATRIX_H
#include <list>
#include <SLib/sabstractfeatures.h>
#include <SLib/smatrix.h>
#include <assert.h>



class SAdjacencyMatrix:public SAbstractFeatures
{
private:
    int matrix[256][256];
    int radius;
    int elements_sum=0;
    inline void calculate(const SMatrix &img);
    inline void ignoreZero(); //можно оптимизировать
    inline void elementsSum();
    inline void checkMatrix();
public:
    SAdjacencyMatrix(int radius=1);
    SAdjacencyMatrix(const SMatrix &img,int radius=1,bool ignore_zero=true);
    double energy() const;
    double entropy()const;
    double localHomogenity()const;
    double maxProbability()const;
    double inertiaMoment()const;
    double trail()const;
    double averageBrightness()const;
    virtual void rebuild(const SMatrix& img, bool ignore_zero=true);
    std::list<std::string> getHeader(const std::string& predicat="");
    std::list<double> getFeatures();
    //double correlationBrightness(double av=-1)const; //странно работает
};

#endif // SADJACENCYMATRIX_H
