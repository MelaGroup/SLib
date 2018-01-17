#include "sfunctors.h"
/*!
 * \brief Конструктор по умолчанию.
 * \details Создает правило преобразования пикселей по формуле (11*RED + 16*GREEN + 5*BLUE)/32, наподобие функции gray() в Qt
 */
SFunctor::SFunctor()
{
    func = [](const QColor& p)
    {
        return (11.*p.red()+16.*p.green()+5.*p.blue())/32.;
    };
}
/*!
 * \brief Конструктор по функции(лямбде).
 * \details Позволяет задавать пользовательские правила с помощью обобщенного указателя на вызываемый объект (Подробнее читайте про functional в std)
 * \code
 * //пример использования
 * auto myfunc = [](const QColor& p)
    {
        return pow(p.red()*p.green()*p.blue(),0.33); // среднее геометрическое
    };
   SMatrix gray(img,SFunctor(myfunc));
 * \endcode
 * \param rule - обобщенный указатель на вызываемый объект.
 */
SFunctor::SFunctor(const std::function<double (const QColor &)> &rule):func(rule){}

/*!
 * \brief Конструктор - линейная комбинация R,G,B.
 * \details Создает правило преобразования пикселей по формуле p = r*RED + g*GREEN + b*BLUE. Нормирование суммы r,g,b не обязательно.
 * \param r - коэффициент при красной компаненте пикселя.
 * \param g - коэффициент при зеленой компаненте пикселя.
 * \param b - коэффициент при синей компаненте пикселя.
 */
SFunctor::SFunctor(double r, double g, double b)
{
    func =[r,g,b](const QColor& p)
    {
        return (r*p.red()+g*p.green()+b*p.blue());
    };
}

/*!
 * \brief Конструктор списком инициализации (линейная комбинация R,G,B).
 * \details В целом эквивалентен SFunctor(double r, double g, double b): cоздает правило преобразования пикселей по формуле p = r*RED + g*GREEN + b*BLUE.
 * Однако позволяет инициализировать списком инициализации.
 * \code
 * SMatrix gray(img,{0.33,0.33,0.33});// среднее арифметическое
 * \endcode
 * \param args - список инициализации {r,g,b}
 */
SFunctor::SFunctor(const std::initializer_list<double> &args)
{
    auto i=args.begin();
    double r,g,b;
    r=*i++,g=*i++,b=*i++;
    func =[r,g,b](const QColor& p)
    {
        return (r*p.red()+g*p.green()+b*p.blue());
    };
}

/*!
 * \brief Копирующий оператор присваивания
 * \param other - Копируемый функтор
 * \return ссылку на текущий функтор
 */
SFunctor &SFunctor::operator=(const SFunctor &other)
{
    func=other.func;
    return *this;
}

/*!
 * \brief Функторный оператор
 * \param pixel - пиксель QColor из исходного QImage изображения
 * \return значение пикселя после преобразования к серому
 */
int SFunctor::operator()(const QColor &pixel) const
{
    return func(pixel);
}

/*!
 * \ingroup Components
 * \brief Функтор выделения по R-каналу (красная компонента)
 * \details Основана на соответствующей стандартной формуле Qt::QColor.
 * \return Правило преобразования пикселей по формуле p = RED
 */
SFunctor S::Red()
{return SFunctor([](const QColor& p){return p.red();});}

/*!
 * \ingroup Components
 * \brief Функтор выделения по G-каналу (зеленая компонента)
 * \details Основана на соответствующей стандартной формуле Qt::QColor.
 * \return Правило преобразования пикселей по формуле p = GREEN
 */
SFunctor S::Green()
{return SFunctor([](const QColor& p){return p.green();});}

/*!
 * \ingroup Components
 * \brief Функтор выделения по B-каналу (синяя компонента)
 * \details Основана на соответствующей стандартной формуле Qt::QColor.
 * \return Правило преобразования пикселей по формуле p = BLUE
 */
SFunctor S::Blue()
{return SFunctor([](const QColor& p){return p.blue();});}

/*!
 * \ingroup Components
 * \brief Функтор выделения по H-каналу (компанента тона)
 * \details Основана на соответствующей стандартной формуле Qt::QColor.
 * \return  Правило преобразования пикселей по формуле p = HUE
 */
SFunctor S::Hue()
{return SFunctor([](const QColor& p){return p.hue();});}

/*!
 * \ingroup Components
 * \brief Функтор выделения по S-каналу (компанента тона)
 * \details Основана на соответствующей стандартной формуле Qt::QColor.
 * \return  Правило преобразования пикселей по формуле p = SATURATION
 */
SFunctor S::Saturation()
{return SFunctor([](const QColor& p){return p.saturation();});}


/*!
 * \ingroup Components
 * \brief Функтор выделения по V-каналу (компанента значения)
 * \details Основана на соответствующей стандартной формуле Qt::QColor.
 * \return  Правило преобразования пикселей по формуле p = VALUE
 */
SFunctor S::Value()
{return SFunctor([](const QColor& p){return p.value();});}


/*!
 * \ingroup Components
 * \brief Ленивое преобразование.
 * \details В качестве значения серого пикселя выбирается код #RRGGBB соответствующего пикселя.
 * Такое преобразование полезно при прямой передаче цветного изображения последующим алгоритмам.
 * Основана на соответствующей стандартной формуле Qt::QColor.
 * \return Правило преобразования пикселей по формуле p = #RRGGBB
 */
SFunctor S::Lasy()
{return SFunctor([](const QColor& p){return (int)(p.rgb());});}
