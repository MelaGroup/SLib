#ifndef STEACHER_H
#define STEACHER_H
#include <SLib/Core/smatrix.h>
#include <SLib/Features/sabstractfeatures.h>

/*!
 * \ingroup Features
 * \brief Класс-интерпретатор разметки.
 * \details Для постановки задачи классификации по прецедентам требуется составление столбца ответов,
 * в котором для выделенных сегментов изображения находятся идентификаторы их классов.
 * Для автоматизированной разметки сегментов прменяется следующий алгоритм:
 * - Создается оригинальная копия изображения в bmp.
 * - На этой копии производится графическая разметка специальными цветами.
 * - Используя карту сегментации оригинального изображания вместе с его размеченной копией можно разметить сегменты:
 * Если сегмент содержит некоторую долю пикселей специального цвета (порог необходимости) - отметить его классом этого цвета.
 * Такой подход позволяет быстро производить переразмечивание сегментов при разных алгоритмах сегментации.
 * Лучше всего использовать план прямой передачи S::Lasy().
 * Порог необходимости - минимальная доля пикселей специального цвета от общего количества пикселей изображения (включая фон!),
 * позволяющая отметить изображение кодом соответствующего цвета.
 *
 * Пример:
 * \code
 * SDataFrame result; //Таблица (пустая)
 * for(size_t i=0;i<folder.size();++i) //Обход папки с размеченными изображениями
 * {
 *     QImage markup(folder[i]); //Загрузка размеченного изображения
 *     SImageDescriptor master(markup,map); //Инициализация описателя по размеченному изображению и карте сегментации оригинала
 *     master.addComponent("",S::Lasy()); //Добавление плана прямой передачи
 *     master.addFeatures(new STeacher()); //Добавление класса-интерпретатора разметки
 *     result.vstack(master.run()); //Создание таблиц с классами сегментов и их вертикальная стыковка
 * }
 * result.toCSV("answers.csv");//Сохранение результатов
 * \endcode
 */
class STeacher: public SAbstractFeatures
{
    std::map<int,int> marks;
    int segment_class=0;
    double necessity_thrs=0.10;
public:
    STeacher ();
    STeacher(const std::map<int, int> &marks, double necessity_threshold=0.10);
    void rebuild(const SMatrix& img, bool ignore_zero=true);
    std::list<std::string> getHeader(const std::string& prefix="");
    std::list<double> getFeatures();
};

#endif // STEACHER_H
