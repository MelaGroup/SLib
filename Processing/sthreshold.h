#ifndef STHRESHOLD_H
#define STHRESHOLD_H

#include <limits>
#include <map>
#include <SLib/Processing/sprocessing.h>

int Otsu(const SMatrix& src);


/*!
 * \ingroup Processing
 * \brief Моды работы бинаризации.
 */
enum THRESH_MODE
{
    BINARY, ///< 1 если яркость пикселя < порога, иначе 0
    BINARY_INV,///<  0 если яркость пикселя < порога, иначе 1
    TOZERO,///< яркость пикселя если яркость пикселя < порога, иначе 0
    TOZERO_INV, ///< яркость пикселя если яркость пикселя >= порога, иначе 0
};


/*!
 * \ingroup Processing
 * \brief Класс для пороговой бинаризации.
 * \details Осуществляет преобразование пикселей изображения по формуле: X если яркость пикселя < порога, иначе Y.
 * Класс зашаблонен по возможным модам бинаризации (см THRESH_MODE).
 * Порог бинаризации может быть выбран пользователем или адаптивно методом Отсу.
 */
template <THRESH_MODE mode>
class SThreshold: public SProcessing
{
    int threshold;

public:
    /*!
     * \brief Конструктор по порогу.
     * \param threshold - значение порога бинаризации
     */
    SThreshold(int threshold = std::numeric_limits<int>::min()):threshold(threshold){}

    /*!
     * \brief Обход изображения.
     * \details Осуществляет бинаризацию изображения по заданному в конструкторе порогу и выбранной моде.
     * Если порог не был задан, он вычисляется методом Отсу (Otsu(const SMatrix& src)).
     * Допускается его многократное использование.
     * \param src - полутоновое изображение
     * \return бинаризованное изображение
     */
    SMatrix bypass(const SMatrix& src)
    {
        SMatrix ret(src.width(),src.height());

        if (threshold==std::numeric_limits<int>::min())
            threshold=Otsu(src);

        for (int y=0;y<src.height();++y)
            for (int x=0;x<src.width();++x)
                if (mode == BINARY)
                    ret.set(x,y,src(x,y)<threshold?0:1);
                else if (mode == BINARY_INV )
                    ret(x,y) =  src(x,y)<threshold?1:0;
                else if (mode == TOZERO)
                    ret(x,y) =  src(x,y)<threshold?0:src(x,y);
                else if (mode == TOZERO_INV)
                    ret(x,y) =  src(x,y)<threshold?src(x,y):0;

        threshold=std::numeric_limits<int>::min();
        return ret;
    }
};

#endif // STHRESHOLD_H
