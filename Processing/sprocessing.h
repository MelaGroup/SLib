#ifndef SPROCESSING_H
#define SPROCESSING_H
#include <deque>
#include <vector>
#include <SLib/Core/smatrix.h>

/*!
 * \ingroup Processing
 * \brief Абстрактная обработка.
 * \details Абстрактный базовый класс для всех классов предобработки.
 * Обеспечивает единоообразие по методу обхода изображений bypass.
 */
class SProcessing
{
public:
    ///\brief Пустой конструктор
    SProcessing(){}
    /*!
     * \brief Обход изображения.
     * \return SMatrix(0,0)
     */
    virtual SMatrix bypass(const SMatrix&){return SMatrix(0,0);}
    ///\brief Пустой деструктор
    virtual ~SProcessing(){}
};

int floodFill(SMatrix& src,int value,int x,int y);

#endif // SPROCESSING_H
