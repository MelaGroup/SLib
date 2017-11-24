#ifndef SADJACENCYMATRIX_H
#define SADJACENCYMATRIX_H

#include <smatrix.h>
#include <assert.h>



class SAdjacencyMatrix
{
private:
    int matrix[256][256];
    int radius;
    int elements_sum=0;
    inline void calculate(const SMatrix &img);
    inline void elementsSum();
    inline void checkMatrix();
public:
    SAdjacencyMatrix(int radius=1);
    SAdjacencyMatrix(const SMatrix &img,int radius=1);
    double energy() const;
    double entropy()const;
    double localHomogenity()const;
    double maxProbability()const;
    double inertiaMoment()const;
    double trail()const;
    double averageBrightness()const;
    //double correlationBrightness(double av=-1)const; //странно работает

};

#endif // SADJACENCYMATRIX_H
