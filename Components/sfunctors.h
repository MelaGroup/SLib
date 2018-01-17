#ifndef SFUNCTORS_H
#define SFUNCTORS_H
#include <QImage>
#include <functional>
/*!
 * \ingroup Components
 * \brief Правило преобразования цветного изображения в полутоновое.
 * \details Данный класс необходим при создании экземпляра класса полутонового изображения SMatrix из QImage.
 * По сути, является функтором, преобразующим пиксели QColor в int по некоторой формуле. Может инстанцироваться
 * произвольными вызываемыми объектами - функциями, лямбда-выражениями и другими функциональнями объектами (см. functional в std)
 */
class SFunctor
{
protected:
    std::function<double(const QColor&)> func;
public:
    SFunctor();
    SFunctor(const std::function<double(const QColor&)>& rule);
    SFunctor(double r,double g,double b);
    SFunctor(const std::initializer_list<double>& args);
    SFunctor& operator=(const SFunctor& other);
    int operator()(const QColor& pixel) const;
};

namespace S
{
    SFunctor Red();
    SFunctor Green();
    SFunctor Blue();
    SFunctor Hue();
    SFunctor Saturation();
    SFunctor Value();
    SFunctor Lasy();
}

#endif // SFUNCTORS_H
