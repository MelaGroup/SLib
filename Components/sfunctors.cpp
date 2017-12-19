#include "sfunctors.h"

SFunctor::SFunctor()
{
    func = [](const QColor& p)
    {
        return (11.*p.red()+16.*p.green()+5.*p.blue())/32.;
    };
}

SFunctor::SFunctor(double r, double g, double b)
{
    func =[r,g,b](const QColor& p)
    {
        return (r*p.red()+g*p.green()+b*p.blue());
    };
}

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

SFunctor &SFunctor::operator=(const SFunctor &other)
{
    func=other.func;
    return *this;
}

int SFunctor::operator()(const QColor &pixel) const
{
    return func(pixel);
}



SFunctor S::Red()
{return SFunctor([](const QColor& p){return p.red();});}

SFunctor S::Green()
{return SFunctor([](const QColor& p){return p.green();});}

SFunctor S::Blue()
{return SFunctor([](const QColor& p){return p.blue();});}

SFunctor S::Hue()
{return SFunctor([](const QColor& p){return p.hue();});}

SFunctor S::Saturation()
{return SFunctor([](const QColor& p){return p.saturation();});}

SFunctor S::Value()
{return SFunctor([](const QColor& p){return p.value();});}

SFunctor S::Lasy()
{return SFunctor([](const QColor& p){return (int)(p.rgb());});}
