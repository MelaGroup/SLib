#ifndef SADJACENCYMATRIX_H
#define SADJACENCYMATRIX_H
#include <list>
#include <SLib/Features/sabstractfeatures.h>
#include <SLib/Core/smatrix.h>
#include <assert.h>

/*!
 * \ingroup Features
 * \brief Класс матрицы пространственной смежности(МПС).
 * \details МПС - матрица, на пересечении i строки и j столбца указывается удвоенное число пар соседей с яркостями i и j.
 * Соседство определяется радиусом смежности: соседями пикселя(x,y) являются все пиксели в квадратной окрестности(x-r,y-r,2r+1,2r+1).
 * С помощью класса можно вычислить 7 признаков:
 *  - Energy(энергия),
 *  - ENT(энтропия),
 *  - LUN(локальная однородность),
 *  - MPR(максимальная вероятность),
 *  - CON(момент инерции),
 *  - TR(след МПС),
 *  - AV(средняя яркость)
 */
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
