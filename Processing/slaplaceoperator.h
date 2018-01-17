#ifndef SLAPLACEOPERATOR_H
#define SLAPLACEOPERATOR_H

#include "SLib/Processing/sprocessing.h"
#include "SLib/Core/smatrix.h"

/*!
 * \brief Класс оператора Лапласа.
 * \details Оператор Лапласа служит для вычерчивания границ объектов. Его маска (3x3) задана следующим образом:
 * \code
 *  int lo [3][3] = { {0,1,0},
 *                    {1,-4,1},
 *                    {0,1,0} };
 * \endcode
 */
class SLaplaceOperator:public SProcessing
{
private:
    int lo [3][3] = { {0,1,0},
                      {1,-4,1},
                      {0,1,0} };
public:
    SLaplaceOperator();
    SMatrix bypass(const SMatrix& src);
};

#endif // SLAPLACEOPERATOR_H
