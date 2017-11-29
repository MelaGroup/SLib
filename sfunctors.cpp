#include "sfunctors.h"

SFunctor::SFunctor(const std::initializer_list<double> &args)
{
    auto i=args.begin();
    r=*i++;g=*i++;b=*i++;
}

SFunctor &SFunctor::operator=(const SFunctor &other)
{
    r=other.r;
    g=other.g;
    b=other.b;
    return *this;
}

int SFunctor::operator()(const QColor &pixel) const
{
    return int(r*double(pixel.red())+g*double(pixel.green())+b*double(pixel.blue()));
}

SMainComponents::SMainComponents(const QImage &src)
{
    SCovarienceMatrix cov_mat(src);
    eigenvalues = cov_mat.eigenvalues();
    eigenvectors = cov_mat.eigenvectors(eigenvalues);
}
