#ifndef SSQUAREMASK_H
#define SSQUAREMASK_H

#include <SLib/Processing/sprocessing.h>
#include <SLib/Core/smatrix.h>

/*!
 * \ingroup Processing
 * \brief Класс специальных квадратных масок.
 * \details Позволяет выделять продолговатые объекты. Является группой из 4-х квадратных симметричных масок:
 *  - Вертикальная: столбцы одинаковые и задаются пользователем.
 *  - Горизонтальная: строки одинаковые и задаются пользователем.
 *  - 45-градусная диагональ: центральная строка задается пользователем,
 * остальные получаются в результате циклического сдвига так, что образуются северовосточные диагонали из одинаковых значений.
 *  - 135-градусная диагональ: аналогично, но образуются северозападные диагонали из одинаковых значений.
 */
class SSquareMask : public SProcessing
{
private:
    int side = 0;
    int radius = side/2;
    int square = side*side;
    int **mask_v = nullptr;
    int **mask_h = nullptr;
    int **mask_r = nullptr;
    int **mask_l = nullptr;
public:
    SSquareMask(int a, std::vector<int> profile);
    SMatrix bypass(const SMatrix& src);
    ~SSquareMask();
};

#endif // SSQUAREMASK_H
