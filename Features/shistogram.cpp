#include "shistogram.h"

/*!
 * \brief Игнорирование черного фона.
 * \details Удаляет нулевую колонку гистограммы. При этом пересчитывается минимальное и максимальное значения яркости.
 */
void SHistogram::ignoreZero()
{
    sum-=bars[0];
    bars.erase(0);
    if (min_key==0)
        min_key=bars.begin()->first;
    if (max_key==0)
        max_key=bars.rbegin()->first;
}

/// \brief Пустой конструктор.
SHistogram::SHistogram(){}

/*!
 * \brief Конструктор по изображению.
 * \details Строит гистограмму по полутоновому изображению. По сути является вызовом метода rebuild.
 * \param src - полутоновое изображение
 * \param ignore_zero - игнорирование черного фона
 */
SHistogram::SHistogram(const SMatrix &src, bool ignore_zero)
{
    rebuild(src,ignore_zero);
}

/*!
 * \brief Поиск минимальной яркости.
 * \return минимальная яркость
 */
double SHistogram::min() const {return min_key;}


/*!
 * \brief Поиск максимальной яркости.
 * \return максимальной яркости
 */
double SHistogram::max() const {return max_key;}


/*!
 * \brief Вычисление размаха по яркости (max-min).
 * \return размах по яркости
 */
double SHistogram::span() const {return max_key - min_key;}


/*!
 * \brief Вычисление средней яркости.
 * \details Происходит по формуле: сумма (i*bars[i])/число пикселей с учетом флага ignore_zero.
 * \return средняя яркость
 */
double SHistogram::MX()
{
    double av=0;
    for(int i=min_key;i<=max_key;++i)
        av+=double(i*bars[i]);
    av/=double(sum);
    return av;
}

/*!
 * \brief Вычисление среднеквадратического отклонения.
 * \details В целях оптимизации вычислений в метод передается значение средней яркости в качестве аргумента.
 * Если оставить значение аргумента по умолчанию (std::numeric_limits<int>::max() - максимальное значение int), то
 * средняя яркость будет вычислена. Рассчет производится по формуле: сумма (bars[i]*(i-MX)^2)/число пикселей с учетом флага
 * ignore_zero.
 * \param av - средняя яркость(опционально)
 * \return СКО
 */
double SHistogram::SD(double av)
{
    if (av==std::numeric_limits<int>::max())
        av=MX();
    double dv=0;
    for(int i=min_key;i<=max_key;++i)
        dv+=double(bars[i]*(i-av)*(i-av));
    dv/=double(sum);
    return sqrt(dv);
}

/*!
 * \brief Перевычисление гистограммы.
 * \details Осуществляет пересчет гистограммы по новому полутоновому изображению и указанному флагу фона.
 * \param src - полутоновое изображение
 * \param ignore_zero - игнорирование черного фона
 */
void SHistogram::rebuild(const SMatrix &src, bool ignore_zero)
{
    min_key=max_key=src(0,0);
    bars.clear();
    for(int r=0;r<src.height();++r)
        for(int c=0;c<src.width();++c)
        {
            int key=src(c,r);
            if (key<min_key) min_key=key;
            if (max_key<key) max_key=key;
            ++bars[key];
        }    
    sum=src.width()*src.height();
    if (ignore_zero) ignoreZero();
}

/*!
 * \brief Создание листа с названиями признаков.
 * \details К названиям признаков можно добавлять некоторую приставку.
 * Итоговое название признака будет складываться из приставки и оригинального названия.
 * Например "Blue_MIN" - минимальная яркость по синему каналу. "Blue" в данном примере является приставкой.
 * \param prefix - приставка
 * \return лист с названиями признаков
 */
std::list<std::__cxx11::string> SHistogram::getHeader(const std::string& predicat)
{
    std::list<std::string> header;
    header.push_back(predicat+"MIN");
    header.push_back(predicat+"MAX");
    header.push_back(predicat+"SPAN");
    header.push_back(predicat+"MX");
    header.push_back(predicat+"SD");
    return header;
}

/*!
 * \brief Создание листа со значениями признаков.
 * \details Признаки распологаются в том же порядке, что и в getHeader.
 * \return лист со значениями признаков
 */
std::list<double> SHistogram::getFeatures()
{
    std::list<double> features;
    features.push_back(min());
    features.push_back(max());
    features.push_back(span());
    double average = MX();
    features.push_back(average);
    features.push_back(SD(average));
    return features;
}

/*!
 * \brief Отрисовка гистограммы.
 * \details Создает черно-белое изображение гистограммы с шириной = SPAN и высотой максимального столбца.
 * Столбцы - черные, фон - белый. Очевидно, такое изображение нужно будет отмасштабировать (QImage::scaled) по высоте и ширине перед выводом.
 * \return Изображение гистограммы.
 * \warning Данная функция не тестировалась.
 */
QImage SHistogram::toImage()
{
    int max_bar=0;
    for(int i=min_key;i<=max_key;++i)
        if (max_bar<bars[i]) max_bar=bars[i];

    QImage img(span(),max_bar+1,QImage::Format_RGB888);

    for(int x=min_key;x<=max_key;++x)
        for(int y=0;y<=max_bar;++y)
        {
            int pix = (y<=bars[x])?0:255;
            img.setPixel(x,y,qRgb(pix,pix,pix));
        }
    return img.mirrored();
}
