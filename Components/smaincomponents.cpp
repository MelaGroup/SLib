#include "smaincomponents.h"
/*!
 * \brief Конструктор по цветному изображению.
 * \details Внутри, осуществляется вычисление ковариационной матрицы, собственных значений и собственных векторов.
 * \param src - Цветное изображение
 */
SMainComponents::SMainComponents(const QImage &src)
{
    SMatrix_3x3 cov_mat(0);
    double Area=double(src.height()*src.width());

    double MR=0,MG=0,MB=0;
    for(int y=0;y<src.height();++y)
        for(int x=0;x<src.width();++x)
        {
            QRgb p=src.pixel(x,y);
            MR+=double(qRed(p));
            MG+=double(qGreen(p));
            MB+=double(qBlue(p));
        }
    MR/=Area;MG/=Area;MB/=Area;

    double DR=0,DG=0,DB=0;
    double covRG=0,covRB=0,covGB=0;
    for(int y=0;y<src.height();++y)
        for(int x=0;x<src.width();++x)
        {
            QRgb p=src.pixel(x,y);
            double deltaR=double(qRed(p))-MR;
            double deltaG=double(qGreen(p))-MG;
            double deltaB=double(qBlue(p))-MB;

            DR+=deltaR*deltaR;
            DG+=deltaG*deltaG;
            DB+=deltaB*deltaB;

            covRG+=deltaR*deltaG;
            covRB+=deltaR*deltaB;
            covGB+=deltaG*deltaB;
        }
    DR/=Area;DG/=Area;DB/=Area;
    covRG/=Area;covRB/=Area;covGB/=Area;
    cov_mat.a(1,1)=DR   ;cov_mat.a(1,2)=covRG;cov_mat.a(1,3)=covRB;
    cov_mat.a(2,1)=covRG;cov_mat.a(2,2)=DG   ;cov_mat.a(2,3)=covGB;
    cov_mat.a(3,1)=covRB;cov_mat.a(3,2)=covGB;cov_mat.a(3,3)=DB   ;

    eigenvalues = cov_mat.eigenvalues();
    eigenvectors = cov_mat.eigenvectors(eigenvalues);
}

/*!
 * \brief Собственные значения ковариационной матрицы.
 * \details Собственные значения представляются в виде координат 3D-вектора. Собственные значения в векторе расположены в порядке убывания.
 * \return собственные значения ковариационной матрицы
 */
SVector_3D SMainComponents::Eigenvalues() const {return eigenvalues;}

/*!
 * \brief Собственные вектора ковариационной матрицы.
 * \details Главные значения представляются в виде столбцов матрицы 3 на 3.
 * Главные вектора в матрице расположены в порядке убывания их собственных значений.
 * Длины векторов нормированы на 1.
 * \return собственные вектора ковариационной матрицы
 */
SMatrix_3x3 SMainComponents::Eigenvectors() const {return eigenvectors;}

/*!
 * \brief Функтор выделения по первой главной компоненте.
 * \details Согласно методу главных компонент является самой информативной среди всех возможных линейных комбинаций по RGB.
 * Формула преобразования: значение проекции на первый собственный вектор (с max собственным значением) p = ({RED; GREEN; BLUE};first_eigenvector)
 * \return Правило преобразования пикселей
 */
SFunctor SMainComponents::first() const
{
    SVector_3D vec = eigenvectors.col(1);
    return {vec.x,vec.y,vec.z};
}

/*!
 * \brief Функтор выделения по второй главной компоненте.
 * \details Согласно методу главных компонент является самой информативной среди всех компонент, ортогональных первой.
 * Формула преобразования: значение проекции на второй собственный вектор p = ({RED; GREEN; BLUE};second_eigenvector)
 * \return Правило преобразования пикселей
 */
SFunctor SMainComponents::second() const
{
    SVector_3D vec = eigenvectors.col(2);
    return {vec.x,vec.y,vec.z};
}

/*!
 * \brief Функтор выделения по третьей главной компоненте.
 * \details Самая неинформативная из главных компонент. Она ортогональна первой и второй.
 * Формула преобразования: значение проекции на третий собственный вектор p = ({RED; GREEN; BLUE};third_eigenvector)
 * \return Правило преобразования пикселей
 */
SFunctor SMainComponents::third() const
{
    SVector_3D vec = eigenvectors.col(3);
    return {vec.x,vec.y,vec.z};
}

/*!
 * \brief Оператор сравнения.
 * \param other - объект для сравнения
 * \return true, при равенстве собственных значений и векторов
 */
bool SMainComponents::operator==(const SMainComponents &other)
{
    return (eigenvalues==other.eigenvalues) && (eigenvectors==other.eigenvectors);
}
