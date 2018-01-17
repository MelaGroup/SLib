#ifndef SCONVEYOR_H
#define SCONVEYOR_H
#include <list>
#include <SLib/Processing/sprocessing.h>

/*!
 * \ingroup Processing
 * \brief Класс-конвеер.
 * \details Обеспечивает выполнение различных алгоритмов предобработок по цепочке, в которой результат предыдущей операции
 * обрабатывается последующей. Поскольку SConveyor является наследником SProcessing, конвееры можно вставлять в другие конвееры.
 * \warning Вставлять обработки следует динамически (через new). Деструктор освободит выделенную память.
 */
class SConveyor: public SProcessing
{
    std::list<SProcessing*> conveyor;
public:
    ///\brief Пустой конструктор.
    SConveyor(){}
    void addProcessing(SProcessing* p);
    SMatrix bypass(const SMatrix& src);
    ~SConveyor();
};

#endif // SCONVEYOR_H
