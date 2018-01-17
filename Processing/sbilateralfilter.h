#ifndef SBILATERALFILTER_H
#define SBILATERALFILTER_H

#include <cmath>
#include <SLib/Processing/sprocessing.h>
#include <SLib/Core/smatrix.h>

/*!
 * \ingroup Processing
 * \brief Класс билатерального фильтра.
 * \details Билатеральный фильтр - вид усредняющего фильтра, в котором весовые коэффициенты соседей зависят как от отдаленности,
 * так и от цветовой разности. Таким образом он сглаживает внутренние области объектов не размывая их границ.
 * Подробнее читайте https://en.wikipedia.org/wiki/Bilateral_filter. Настоящий фильтр имеет форму матрицы 3х3, коэффициенты
 * расчитываются согласно гауссовой функции с максимумом в обрабатываемом пикселе (пространство OxyI, где I - яркость пикселя).
 * Имеет 2 параметра - пространственный разброс sd и яркостной разброс si. Чем больше разброс по признаку, тем слабее его вклад.
 */

class SBilateralFilter : public SProcessing
{
private:
    double sigmaD;
    double sigmaI;
public:
    SBilateralFilter(double sd, double si);
    SMatrix bypass (const SMatrix &src);
};

#endif // BILATERALFILTER_H
