#include "sconveyor.h"

/*!
 * \brief Добавление обработки в конвеер.
 * \details Добавляет динамически созданную (через new) обработку, которую он сам освободит при вызове деструктора.
 * Например:
 * \code
 * SConveyor con();
 * con.addProcessing(new SThreshold<BINARY>()); //Добавление адаптивной прямой бинаризации
 * // не требуется дополнительного управления памятью!!!
 * \endcode
 * \param p - указатель на алгоритм предобработки
 * \warning Конвеер и только конвеер должен управлять динамически созданной памятью.
 */
void SConveyor::addProcessing(SProcessing *p)
{
    conveyor.push_back(p);
}

/*!
 * \brief Обход изображения.
 * \details Выполняет обход изображения заданными алгоритмами в порядке их добавления.
 * Пустой конвеер вернет исходное изображение.
 * \param src - полутоновое изображение
 * \return обработанную копию изображения
 */
SMatrix SConveyor::bypass(const SMatrix &src)
{
    SMatrix ret(src);
    for (SProcessing* p:conveyor)
        ret=p->bypass(ret);
    return ret;
}

/*!
 * \brief Деструктор конвеера
 * \details Последовательно вызывает оператор delete для всех занесенных алгоритмов.
 */
SConveyor::~SConveyor()
{
    for (SProcessing* p:conveyor)
        delete p;
}
