#ifndef SABSTRACTCLASSIFIER_H
#define SABSTRACTCLASSIFIER_H
#include <SLib/Core/sdataframe.h>
/*!
 * \ingroup Classifying
 * \brief Абстрактный класс классификатора, обеспечивающий единообразие процедур обучения(fit) и классификации(predict)
 */
class SAbstractClassifier
{
public:
    //! \brief Пустой конструктор
    SAbstractClassifier(){}

    //! \brief Обучает классификатор, используя таблицу"Объекты-признаки" (SDataFrame) и столбец ответов (vector<int>)
    virtual void fit(const SDataFrame&,const std::vector<int>&)
    {}

    //! \brief Проводит классификацию, используя таблицу "Объекты-признаки" (SDataFrame), в виде столбеца предсказаний(vector<int>)
    virtual std::vector<int> predict(const SDataFrame&)
    {return {};}

    //! \brief Пустой деструктор
    virtual ~SAbstractClassifier(){}
};

#endif // SABSTRACTCLASSIFIER_H
