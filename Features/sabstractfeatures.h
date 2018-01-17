#ifndef SABSTRACTFEATURES_H
#define SABSTRACTFEATURES_H

#include <map>
#include <list>
#include <SLib/Core/smatrix.h>

/*!
 * \ingroup Features
 * \brief Абстрактные признаки.
 * \details Абстрактный базовый класс для всех классов-описателей.
 * Обеспечивает единообразие по методам getHeader, getHeader и rebuild.
 */
class SAbstractFeatures
{
public:
    //! \brief Пустой конструктор
    SAbstractFeatures(){}

    //! \brief Перестраивание(перевычисление признаков) класса по новому полутоновому изображению.
    virtual void rebuild(const SMatrix& , bool){}

    //! \brief Список имен классов(пустой).
    virtual std::list<std::string> getHeader(const std::string&){return {};}

    //! \brief Список из вычисленных классом значений признаков(пустой).
    virtual std::list<double> getFeatures(){return {};}

    //! \brief Пустой деструктор(виртуальный).
    virtual ~SAbstractFeatures(){}
};

#endif // SABSTRACTFEATURES_H
