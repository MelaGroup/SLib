#include "sbilateralfilter.h"

/*!
 * \brief Конструктор по параметрам функции Гаусса.
 * \details Конструктор имеет 2 параметра - пространственный разброс sd и яркостной разброс si.
 * Чем больше разброс по признаку, тем слабее его вклад.
 * \param sd - пространственный разброс
 * \param si - яркостной разброс
 */
SBilateralFilter::SBilateralFilter(double sd, double si):sigmaD(sd),sigmaI(si){}


/*!
 * \brief Обход изображения.
 * \details Осуществляет обход изображения билатеральным фильтром по указанному изображению с заданными параметрами пространственного и яркостного разброса.
 * Обращаем внимание что обработка не затрагивает граничные пиксели.
 * \param src - полутоновое изображение
 * \return обработанную копию изображения
 */
SMatrix SBilateralFilter::bypass(const SMatrix &src)
{
    SMatrix ret(src.width(), src.height());
    for (int x=1;x<src.width()-1;++x)
        for (int y=1;y<src.height()-1;++y)
        {
            double I = src(x,y);
            double In = 0, w=0;
            for(int i=-1;i<=1;++i) for(int j=-1;j<=1;++j)
            {
                int xn=x+i; int yn=y+j;
                double tw = exp(-((i*i+j*j)/(2*pow(sigmaD,2)))-(pow((src(xn,yn)-I),2)/(2*pow(sigmaI,2))));
                w+=tw;
                In += src(xn,yn)*tw;
            }
            ret(x,y) = In/w;
        }
    return ret;
}

