#ifndef SADJACENCYMATRIX_H
#define SADJACENCYMATRIX_H
#include <list>
#include <SLib/smatrix.h>
#include <assert.h>



class SAdjacencyMatrix
{
private:
    int matrix[256][256];
    int radius;
    int elements_sum=0;
    inline void calculate(const SMatrix &img);
    inline void ignoreZero();
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
    virtual std::list<std::string> getHeader()
    {
        std::list<std::string> header;
        header.push_back("Energy");
        header.push_back("Entropy");
        header.push_back("Local_Homogenity");
        header.push_back("Max_Probability");
        header.push_back("Moment_Inertia");
        header.push_back("Trail");
        header.push_back("Average_Brightness");
        return header;
    }

    //double correlationBrightness(double av=-1)const; //странно работает

};

#endif // SADJACENCYMATRIX_H
