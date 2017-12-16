#ifndef SFUNCTORS_H
#define SFUNCTORS_H
#include <QImage>
#include <functional>

class SFunctor
{
protected:
    std::function<double(const QColor&)> func;
public:
    SFunctor();
    SFunctor(const std::function<double(const QColor&)>& rule):func(rule){}
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
