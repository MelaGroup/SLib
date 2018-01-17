#include "sedgedetection.h"
/*!
 * \brief Обход изображения.
 * \details Применяет горизонтальный и вертикальный оператор Собеля на всем(включая граничные пиксели) выбранном полутоновом изображении.
 * Яркость пикселя получается как корень из суммы квадратов яркостей, полученных горизонтальным и вертикальным оператором.
 * \param src - полутоновое изображение
 * \return преобразованную копию изображения
 */
SMatrix SSobelOperator::bypass(SMatrix &src)
{
    int xn, yn, gx, gy;
    SMatrix ret(src.width(),src.height());
    for (int x=0; x<src.width(); ++x)
        for (int y=0; y<src.height(); ++y)
        {
            gx = gy = 0;
            for (int i=-1; i<=1; ++i)
                for (int j=-1; j<=1; ++j)
                {
                    xn = x+i; yn = y+j;
                    gx += src.get(xn,yn) * Gx[i+1][j+1];
                    gy += src.get(xn,yn) * Gy[i+1][j+1];
                }
            ret(x,y) = std::sqrt(gx*gx + gy*gy);
        }
    return ret;
}

/*!
 * \brief Обход изображения.
 * \details Применяет горизонтальный и вертикальный оператор Превитта на всем(включая граничные пиксели) выбранном полутоновом изображении.
 * Яркость пикселя получается как корень из суммы квадратов яркостей, полученных горизонтальным и вертикальным оператором.
 * \param src - полутоновое изображение
 * \return преобразованную копию изображения
 */
SMatrix SPrewittOperator::bypass(SMatrix &src)
{
    int xn, yn, gx, gy;
    SMatrix ret(src.width(),src.height());
    for (int x=0; x<src.width(); ++x)
        for (int y=0; y<src.height(); ++y)
        {
            gx = gy = 0;
            for (int i=-1; i<=1; ++i)
                for (int j=-1; j<=1; ++j)
                {
                    xn = x+i; yn = y+j;
                    gx += src.get(xn,yn) * Gx[i+1][j+1];
                    gy += src.get(xn,yn) * Gy[i+1][j+1];
                }
            ret(x,y) = std::sqrt(gx*gx + gy*gy);
        }
    return ret;
}

/*!
 * \brief Обход изображения.
 * \details Применяет горизонтальный и вертикальный оператор Робертса на всем(включая граничные пиксели) выбранном полутоновом изображении.
 * Яркость пикселя получается как корень из суммы квадратов яркостей, полученных горизонтальным и вертикальным оператором.
 * \param src - полутоновое изображение
 * \return преобразованную копию изображения
 */
SMatrix SRobertsOperator::bypass(SMatrix &src)
{
    int xn, yn, gx, gy;
    SMatrix ret(src.width(),src.height());
    for (int x=0; x<src.width(); ++x)
        for (int y=0; y<src.height(); ++y)
        {
            gx = gy = 0;
            for (int i=-1; i<=0; ++i)
                for (int j=-1; j<=0; ++j)
                {
                    xn = x+i; yn = y+j;
                    gx += src.get(xn,yn) * Gx[i+1][j+1];
                    gy += src.get(xn,yn) * Gy[i+1][j+1];
                }
            ret(x,y) = std::sqrt(gx*gx + gy*gy);
        }
    return ret;
}
