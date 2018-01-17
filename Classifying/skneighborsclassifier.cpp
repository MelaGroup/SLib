#include "skneighborsclassifier.h"
/*!
 * \ingroup Classifying
 * \brief Метрика Евклида.
 * \details Стандартная метрика в евклидовой геометрии.
 * Расстояние между точками A и B есть среднее квадратическое проекций вектора AB.
 * \return Класс метрики, в последствии используемой классификатором.
 */
SMetric S::euclidean()
{
    using namespace std;
    auto ret = [](const vector<double>& f,const vector<double>& s)
    {
        double dist=-1;
        if(f.size()==s.size())
        {
            dist=0;
            for(size_t i=0;i<f.size();++i)
                dist+=(f[i]-s[i])*(f[i]-s[i]);
            dist=sqrt(dist);
        }
        return dist;
    };
    return SMetric(ret,"euclidean");
}
/*!
 * \ingroup Classifying
 * \brief Метрика Манхэттена.
 * \details Расстояние между точками A и B есть сумма модулей проекций вектора AB.
 * \return Класс метрики, в последствии используемой классификатором.
 */
SMetric S::manhattan()
{
    using namespace std;
    auto ret = [](const vector<double>& f,const vector<double>& s)
    {
        double dist=-1;
        if(f.size()==s.size())
        {
            dist=0;
            for(size_t i=0;i<f.size();++i)
                dist+=fabs(f[i]-s[i]);
        }
        return dist;
    };
    return SMetric(ret,"manhattan");
}
/*!
 * \ingroup Classifying
 * \brief Метрика Чебышева.
 * \details Расстояние между точками A и B есть максимальное значение из модулей проекций вектора AB.
 * \return Класс метрики, в последствии используемой классификатором.
 */
SMetric S::chebyshev()
{
    using namespace std;
    auto ret = [](const vector<double>& f,const vector<double>& s)
    {
        double dist=-1;
        if(f.size()==s.size())
        {
            dist=0;
            for(size_t i=0;i<f.size();++i)
                if (dist<fabs(f[i]-s[i]))
                    dist=fabs(f[i]-s[i]);
        }
        return dist;
    };
    return SMetric(ret,"chebyshev");
}

/*!
 * \brief Конструктор классификатора
 * \param n_neighbors - число соседей (по умолчанию 5)
 * \param metric - используемая метрика (по умолчанию - Евклида)
 * \warning Отказы от классификации (например: на четном k) никак не контролируются.
 * \throw std::invalid_argument - при неположительном k
 */
SKNeighborsClassifier::SKNeighborsClassifier(int n_neighbors, SMetric metric)
    :n_neighbors(n_neighbors),metric(metric)
{
    if (n_neighbors<1)
        throw std::invalid_argument("SKNeighborsClassifier: the number of neighbours must be greater than 0");
}

/*!
 * \brief Производит обучение классификатора по прецедентам.
 * \param X - таблица "Объекты-Признаки"
 * \param Y - соответствующий столбец ответов
 * \warning Следите за сохранением порядка и соответствия между строками X и метками классов в Y.
 * \throw std::invalid_argument - если число строк в X и Y не совпадает
 */
void SKNeighborsClassifier::fit(const SDataFrame &X, const std::vector<int> &Y)
{
    if (X.rows()!=int(Y.size()))
        throw std::invalid_argument("SKNeighborsClassifier: table objects-attributes and a vector of responses not compatible");
    table=X,answers=Y;
}
/*!
 * \brief Производит классификацию объектов.
 * \param X - таблица "Объекты-Признаки"
 * \warning Отказы от классификации (например: на четном k) никак не контролируются.
 * \return Столбец предсказаний (№ строки в X соответствует № элемента)
 */
std::vector<int> SKNeighborsClassifier::predict(const SDataFrame &X)
{
    using namespace std;
    std::vector<int> predictions(X.rows());
    for (int j=0;j<X.rows();++j)
    {
        multimap<double,int> distancies;
        for (int i=0;i<table.rows();++i)
            distancies.insert(make_pair(metric(X.row(j),table.row(i)),i));

        map<int,int> counters;
        auto It=distancies.begin();
        for (int i=0;i<n_neighbors;++i,++It)
            ++counters[answers[It->second]];

        pair<int,int> max_counter{0,0};
        for (pair<int,int> c:counters)
            if (max_counter.second<c.second) max_counter=c;

        predictions[j]=max_counter.first;
    }
    return predictions;
}

