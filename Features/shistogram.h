#ifndef SHISTOGRAM_H
#define SHISTOGRAM_H
#include <Slib/Features/sabstractfeatures.h>
#include <SLib/Core/smatrix.h>
#include <limits>
/*!
 * \ingroup Features
 * \brief Класс гистограммы яркостей.
 * \details Служит для расчета гистограммных признаков по полутоновому изображению.
 * Минимальная и максимальная яркость не ограничены [0;256).
 * С помощью класса можно вычислить 5 признаков:
 *  - MIN(минимальная яркость),
 *  - MAX(максимальная яркость),
 *  - SPAN(размах по яркости),
 *  - MX(средняя яркость)
 *  - SD(СКО)
 */
class SHistogram:public SAbstractFeatures
{   
    std::map<int,int> bars;
    int min_key,max_key;
    int sum=0;

    inline void ignoreZero();
public:
    SHistogram();
    SHistogram(const SMatrix& src, bool ignore_zero=true);
    double min() const;
    double max() const;
    double span() const;
    double MX();
    double SD(double av=std::numeric_limits<double>::max());

    virtual void rebuild(const SMatrix& src, bool ignore_zero=true);
    std::list<std::string> getHeader(const std::string& predicat="");
    std::list<double> getFeatures();
    virtual QImage toImage();
};

#endif // SHISTOGRAM_H
