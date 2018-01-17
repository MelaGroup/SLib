#ifndef SMAINCOMPONENTS_H
#define SMAINCOMPONENTS_H

#include <QImage>
#include <SLib/Components/sfunctors.h>
#include <SLib/Components/slinearalgebra.h>
/*!
 * \ingroup Components
 * \brief Класс, служащий для перехода из пространства RGB в пространство собственных векторов методом главных компонент (PCA).
 * \details Данный переход осуществляется следующим образом:
 * 1. Составляется ковариационная матрица для пикселей по координатам RGB.
 * 2. Для данной матрицы вычисляются собственные значения и собственные вектора.
 * 3. Осуществляется переход из RGB в пространство собственных векторов.
 *
 * Пример использования:
 * \code
 * QImage image(path);
   SMainComponents mc(image);
   SMatrix main_plane(image,mc.first());
 * \endcode
 */
class SMainComponents
{
    SVector_3D eigenvalues;
    SMatrix_3x3 eigenvectors;
public:
    SMainComponents(const QImage& src);
    SVector_3D Eigenvalues() const;
    SMatrix_3x3 Eigenvectors()const;
    SFunctor first() const;
    SFunctor second() const;
    SFunctor third() const;
    bool operator==(const SMainComponents& other);
};

#endif // SMAINCOMPONENTS_H
