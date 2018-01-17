#include "slaplaceoperator.h"
///\brief Пустой конструктор
SLaplaceOperator::SLaplaceOperator()
{

}

/*!
 * \brief Обход изображения.
 * Осуществляет обход изображения оператором Лапласа по указанному изображению.
 * Обращаем внимание что обработка идет по всему изображению, включая граничные пиксели.
 * При выпадении яркость пикселя считается нулевой.
 * \param src - полутоновое изображение
 * \return обработанную копию изображения
 * \todo Реализовать вторую маску оператора Лапласа (c -8 в центре и 1 в соседних) и
 * обеспечить более осмысленную обработку граничных пикселей.
 */
SMatrix SLaplaceOperator::bypass(const SMatrix &src)
{
    SMatrix ret(src);
    int xn,yn,tmp;
    for (int y=0;y<src.height();++y)
        for (int x=0;x<src.width();++x)
        {
            tmp=0;
            for (int i=-1;i<=1;++i)
                for (int j=-1;j<=1;++j)
                {
                    xn=x+i; yn=y+j;
                    tmp+=src.get(xn,yn)*lo[i+1][j+1];
                }
            ret(x,y)=tmp;
        }
    return ret;
}
