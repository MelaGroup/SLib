#include "steacher.h"

/*!
 * \brief Конструктор по умолчанию.
 * \details Задает правило интерпретации цветов:
 * - красный   (255,0,0) - код 1
 * - зеленый   (0,255,0 )- код 2
 * - синий     (0,0,255) - код 3
 * - желтый    (255,255,0) - код 4
 * - голубой   (0,255,255) - код 5
 * - пурпурный (255,0,255) - код 6
 * Иначе - код 0
 * По умолчанию порог необходимости = 0.1
 * \warning Т.к интерпретируются не яркости, а коды пикселей, используте план прямой передачи S::Lasy().
 */
STeacher::STeacher()
{
    marks = {{0xFFFF0000,1}, //red
             {0xFFFFFF00,2}, //yellow
             {0xFF00FF00,3}, //green
             {0xFF00FFFF,4}, //cyan
             {0xFF0000FF,5}, //blue
             {0xFFFF00FF,6}}; //magenta
}

/*!
 * \brief Конструктор по правилу интерпретации.
 * \details Позволяет задать собственное правило интерпретации цветов в виде словаря и доли пикселей.
 * Ключ словаря - цвет, значение - код класса. Например:
 * \code
 *  marks = {{0xFFFF0000,1}, //red
             {0xFFFFFF00,2}, //yellow
             {0xFF00FF00,3}, //green
             {0xFF00FFFF,4}, //cyan
             {0xFF0000FF,5}, //blue
             {0xFFFF00FF,6}}; //magenta
 * \endcode
 * \param marks - ссылка на словарь
 * \param necessity_threshold - порог необходимости
 * \throw std::invalid_argument - при пороге <0 или >1
 */
STeacher::STeacher(const std::map<int, int> &marks, double necessity_threshold)
    :marks(marks),necessity_thrs(necessity_threshold)
{
    if (necessity_threshold>1 || necessity_threshold<0)
        throw std::invalid_argument("STeacher: necessity_threshold must be within [0,1]");
}

/*!
 * \brief Перестройка класса.
 * \details По сути дает изображению метку класса. Игнорирование фона при этом не играет существенной роли.
 * \param img - полутоновое изображение
 * \param ignore_zero - игнорирование фона
 * \remark Лучше всего используте план прямой передачи S::Lasy().
 * \warning В случае конфликта разметок изображение относится к классу с наибольшим ключем в словаре.
 */
void STeacher::rebuild(const SMatrix &img, bool ignore_zero)
{
    using namespace std;
    map<int,int> counters;
    for (int y=0;y<img.height();++y)
        for (int x=0;x<img.width();++x)
        {
            int pix = img(x,y);
            if (!ignore_zero || pix != 0)
                ++counters[pix];
        }

    segment_class=0;
    if (counters.size()!=0)
    {
        for (pair<int,int> c:counters)
            if (marks.find(c.first)!=marks.end())
            {
                double ratio = double(c.second)/
                        double(img.width()*img.height());
                if  (ratio>necessity_thrs)
                {
                    if (segment_class!=0)
                    {
                        qDebug("STeacher: the markup used permits duality");
                        qDebug("    help: try to improve threshold of necessity");
                    }
                    segment_class = marks[c.first];
                }
            }
    }
}

/*!
 * \brief Создание листа с названием признака.
 * \details К названиям признаков можно добавлять некоторую приставку.
 * Итоговое название признака будет складываться из приставки и оригинального названия = "Y".
 * Признак всего один - идентификатор класса.
 * \param prefix - приставка
 * \return лист с одним названием
 */
std::list<std::__cxx11::string> STeacher::getHeader(const std::__cxx11::string &prefix)
{return {prefix+"Y"};}

/*!
 * \brief Создание листа со значением идентификатора класса.
 * \details Возвращает лист с единственным признаком - идентификатором класса.
 * \return лист с одним признаком
 */
std::list<double> STeacher::getFeatures()
{
    return {double(segment_class)};
}
