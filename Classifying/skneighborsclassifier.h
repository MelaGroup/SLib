#ifndef SKNEIGHBORSCLASSIFIER_H
#define SKNEIGHBORSCLASSIFIER_H
#include <functional>
#include <Slib/Classifying/sabstractclassifier.h>

using DistFunc=std::function<double(const std::vector<double>&,const std::vector<double>&)>;
/*!
 * \ingroup Classifying
 * \brief Класс метрики в признаковом пространстве.
 * \details Класс, по сути, является обобщенным указателем на функцию,
 * поэтому может содержать любое осмысленное правило вычисления расстояний между двумя точками признакового пространства.
 */
class SMetric
{
    std::string name;
    DistFunc func;
public:
    SMetric(DistFunc func,std::string name=""):name(name),func(func){}
    double operator()(const std::vector<double>& f, const std::vector<double>& s)
    {return func(f,s);}
    std::string& Name(){return name;}
};

namespace S
{
    SMetric euclidean();
    SMetric manhattan();
    SMetric chebyshev();
}
/*!
 * \ingroup Classifying
 * \brief Класс классификатора,на основе метода k ближайших соседей.
 * \details Суть метода подробно описана на http://www.machinelearning.ru/wiki/index.php?title=KNN
 */
class SKNeighborsClassifier:public SAbstractClassifier
{
    int n_neighbors;
    SMetric metric;
    std::vector<int> answers;
    SDataFrame table;
public:
    SKNeighborsClassifier(int n_neighbors=5,SMetric metric = S::euclidean());
    void fit(const SDataFrame& X,const std::vector<int>& Y);
    std::vector<int> predict(const SDataFrame& X);   
};

#endif // SKNEIGHBORSCLASSIFIER_H
