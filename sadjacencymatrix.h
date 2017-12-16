#ifndef SADJACENCYMATRIX_H
#define SADJACENCYMATRIX_H
#include <list>
#include <SLib/sabstractfeatures.h>
#include <SLib/smatrix.h>
#include <assert.h>



class SAdjacencyMatrix:public SAbstractFeatures
{
private:
    std::map<int,std::map<int,int>> matrix;
    int radius;
    int elements=0;
    inline void calculate(const SMatrix &img);
    inline void ignoreZero(); //можно оптимизировать
public:
    SAdjacencyMatrix(int radius=1);
    SAdjacencyMatrix(const SMatrix &img,int radius=1,bool ignore_zero=true);
    double energy();
    double entropy();
    double localHomogenity();
    double maxProbability();
    double inertiaMoment();
    double trail();
    double averageBrightness();
    bool operator==(const SAdjacencyMatrix& other)const;

    virtual void rebuild(const SMatrix& img, bool ignore_zero=true);
    std::list<std::string> getHeader(const std::string& predicat="");
    std::list<double> getFeatures();
    //double correlationBrightness(double av=-1)const; //странно работает
};

#endif // SADJACENCYMATRIX_H
