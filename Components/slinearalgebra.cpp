#include "slinearalgebra.h"
/*!
 * \brief Конструктор по координатам
 * \details Координаты - действительные (double) числа.
 * \param x - X координата вектора
 * \param y - Y координата вектора
 * \param z - Z координата вектора
 */
SVector_3D::SVector_3D(double x, double y, double z):x(x),y(y),z(z){}

/*!
 * \brief Конструктор списком инициализации.
 * \details Координаты - действительные (double) числа в списке инициализации. Класс init = std::initializer_list<double>;
 * \param xyz - список инициализации {x,y,z}
 * \warning Не пытайтесь инициализировать списком инициализации с размером !=3.
 */
SVector_3D::SVector_3D(const init &xyz)
{
    auto i=xyz.begin();
    x=*i++; y=*i++; z=*i++;
}

/*!
 * \brief Оператор суммы векторов.
 * \details Осуществляет покоординатную сумму: {x1+x2;y1+y2;z1+z2}
 * \param other - второе слагаемое
 * \return Вектор суммы
 */
SVector_3D SVector_3D::operator+(const SVector_3D &other) const
{return SVector_3D(x+other.x,y+other.y,z+other.z);}

/*!
 * \brief Оператор разности векторов.
 * \details Осуществляет покоординатную разность: {x1-x2;y1-y2;z1-z2}
 * \param other - вычитаемое
 * \return Вектор разности
 */
SVector_3D SVector_3D::operator-(const SVector_3D &other) const
{return SVector_3D(x-other.x,y-other.y,z-other.z);}

/*!
 * \brief Оператор скаляроного произведения векторов.
 * \details Осуществляет скалярное произведение по формуле: ret = x1*x2 + y1*y2 + z1*z2
 * \param other - второе слагаемое
 * \return Вектор суммы
 */
double SVector_3D::operator*(const SVector_3D &other) const
{return x*other.x+y*other.y+z*other.z;}

/*!
 * \brief Оператор нормирования вектора.
 * \details Делит все координаты вектора на заданное число.
 * \param norma - делитель (не 0)
 * \return Нормированный вектор
 * \warning Не нормируйте вектор на 0.
 */
SVector_3D &SVector_3D::operator/=(double norma)
{
    x/=norma;y/=norma;z/=norma;
    return *this;
}

/*!
 * \brief Сортировка координат по убыванию.
 * \details Не имеет смысла в рамках линейной алгебры. Предназначается для сортировки набора из 3-х чисел по убыванию.
 * Алгоритм сортировки - bubble sort. После сортировки в координате x - наибольшее, а в z - наименьшее из них.
 * \return Упорядоченный по убыванию набор из 3-х чисел
 */
SVector_3D& SVector_3D::sort()
{
    double nx=x,ny=y,nz=z,buf;
    do{
        if (nx<ny)
        {buf=nx,nx=ny,ny=buf;}
        if (ny<nz)
        {buf=ny,ny=nz,nz=buf;}
    }while(!(nx>=ny && ny>=nz));
    x=nx;y=ny;z=nz;
    return *this;
}

/*!
 * \brief Оператор индексации.
 * \details Между координатами и индексами установленно соответствие: 1 - x; 2 - y; 3 - z; иначе - исключение.
 * \throw std::invalid_argument - при индексе <1 или >3
 * \param num - Индекс
 * \return значение координаты
 * \warning Индексация производится с единицы.
 */
double SVector_3D::operator[](int num)
{
    switch (num) {
    case 1:
        return x;
        break;
    case 2:
        return y;
        break;
    case 3:
        return z;
        break;
    default:
        throw std::invalid_argument("SVector_3D::[] - Bad index in SVector_3D");
        return -1;
        break;
    }
}

/*!
 * \brief Оператор сравнения
 * \details Вектора считаются равными если сумма модулей разностей их координат < 10^-7, т.е
 * |x1-x2|+|y1-y2|+|z1-z2|<1E-7
 * \param other - сравниваемый вектор
 * \return true - если равны
 */
bool SVector_3D::operator==(const SVector_3D &other)
{
    return fabs(x-other.x)+fabs(y-other.y)+fabs(z-other.z)<1E-7;
}

/*!
 * \brief Вывод вектора в отладочную консоль
 */
void SVector_3D::print() const
{
    qDebug()<<"{"<<x<<","<<y<<","<<z<<"};";
}

/*!
 * \brief Решение кубического уравнения.
 * \details В ходе задачи нахождения собственных значений матрицы требуется решить характеристическое уравнение,
 * представленное в виде кубического уравнения X^3+aX^2+bX+c=0. Для кубических уравнений существует строгое алгебраическое решение -
 * метод Виете-Кардано, суть которого изложена на http://program.rin.ru/razdel/html/757.html.
 * \param a - коэффициент при X^2
 * \param b - коэффициент при X
 * \param c - свободный член
 * \return Набор из 3-х корней.
 * \warning Реализация метода не подразумевает наличие комплексных корней.
 */
SVector_3D SMatrix_3x3::VieteCardanoMethod(double a, double b, double c) const
{
    SVector_3D roots;
    double q,r,r2,q3;
    double M_2PI=2.*M_PI;
    q=(a*a-3.*b)/9.; r=(a*(2.*a*a-9.*b)+27.*c)/54.;
    r2=r*r; q3=q*q*q;
    if(r2<q3)
    {
        double t=acos(r/sqrt(q3));
        a/=3.; q=-2.*sqrt(q);
        roots.x=q*cos(t/3.)-a;
        roots.y=q*cos((t+M_2PI)/3.)-a;
        roots.z=q*cos((t-M_2PI)/3.)-a;

    }
    else
    {
        double aa,bb;
        if(r<=0.) r=-r;
        aa=-pow(r+sqrt(r2-q3),1./3.);
        if(aa!=0.) bb=q/aa;
        else bb=0.;
        a/=3.; q=aa+bb; r=aa-bb;
        roots.x=q-a;
        roots.y=(-0.5)*q-a;
        roots.z=(sqrt(3.)*0.5)*fabs(r);
    }
    return roots;
}

/*!
 * \brief Конструктор диагональной матрицы.
 * \details Конструирует матрицу по правилу: если элемент диагональный (№ столбца = № строки), то = diag_value, иначе = 0;
 * \param diag_value - значение диагональных элементов, по умолчанию = 1
 */
SMatrix_3x3::SMatrix_3x3(double diag_value):mat{{diag_value,0,0},{0,diag_value,0},{0,0,diag_value}}{}

/*!
 * \brief Копирующий конструктор столбцами.
 * \details Констрирует матрицу 3-мя столбцами в виде трехмерных векторов. Координаты x будут расположены в первой строке, а y и z - во 2 и 3.
 * \param col1 - первая колонка
 * \param col2 - вторая колонка
 * \param col3 - третья колонка
 */
SMatrix_3x3::SMatrix_3x3(const SVector_3D &col1, const SVector_3D &col2, const SVector_3D &col3)
{
    mat[0][0]=col1.x,mat[1][0]=col1.y,mat[2][0]=col1.z;
    mat[0][1]=col2.x,mat[1][1]=col2.y,mat[2][1]=col2.z;
    mat[0][2]=col3.x,mat[1][2]=col3.y,mat[2][2]=col3.z;
}

/*!
 * \brief Дает строку по индексу (копия).
 * \details Возвращает строку матрицы по указанному индексу в виде вектора.
 * \throw std::invalid_argument - при индексе <1 или >3
 * \param i - индекс
 * \return строка матрицы
 * \warning Индексация производится с единицы.
 */
SVector_3D SMatrix_3x3::row(int i) const
{
    if (i<1 || i>3) throw std::invalid_argument("SMatrix_3x3::row - Row doesn't exist");
    return SVector_3D(mat[i-1][0],mat[i-1][1],mat[i-1][2]);
}

/*!
 * \brief Дает столбец по индексу (копия).
 * \details Возвращает столбец матрицы по указанному индексу в виде вектора (x,y,z - 1,2,3 строка соответственно).
 * \throw std::invalid_argument - при индексе <1 или >3
 * \param i - индекс
 * \return столбец матрицы
 * \warning Индексация производится с единицы.
 */
SVector_3D SMatrix_3x3::col(int j) const
{
    if (j<1 || j>3) throw std::invalid_argument("SMatrix_3x3::col - Column doesn't exist");
    return SVector_3D(mat[0][j-1],mat[1][j-1],mat[2][j-1]);
}

/*!
 * \brief Оператор разности матриц.
 * \details Осуществляет поэлементную разность.
 * \param other - вычитаемая матрица
 * \return Матрица разности
 */
SMatrix_3x3 SMatrix_3x3::operator-(const SMatrix_3x3 &other) const
{
    SMatrix_3x3 sub;
    for (int i=0;i<3;++i)
        for (int j=0;j<3;++j)
            sub.mat[i][j]=mat[i][j]-other.mat[i][j];
    return sub;
}

/*!
 * \brief Оператор произведения матриц.
 * \details Осуществляет обчное произведение матриц, в котором значение элемента на i-ой строке в j-ом столбце есть
 * скаларное произведение вектора i-ой строки I матрицы и вектора j-го столбца II матрицы.
 * \param m - II матрица
 * \return Матрица произведения
 */
SMatrix_3x3 SMatrix_3x3::operator*(const SMatrix_3x3& m)const
{
    SMatrix_3x3 dst;
    for(int i=1;i<4;++i)
        for(int j=1;j<4;++j)
            dst.mat[i-1][j-1]=row(i)*m.col(j);
    return dst;
}

/*!
 * \brief Оператор сравнения
 * \details Матрицы считаются равными, если сумма модулей разностей соответствующих элементов меньше 10^-7, т.е
 * |a11-b11|+|a12-b12|+...+|a33-b33|<1E-7
 * \param other - сравниваемая матрица
 * \return true - если равны
 */
bool SMatrix_3x3::operator==(const SMatrix_3x3 &other) const
{
    double a00,a10,a20;
    double a01,a11,a21;
    double a02,a12,a22;

    a00=fabs(mat[0][0]-other.mat[0][0]);
    a10=fabs(mat[1][0]-other.mat[1][0]);
    a20=fabs(mat[2][0]-other.mat[2][0]);

    a01=fabs(mat[0][1]-other.mat[0][1]);
    a11=fabs(mat[1][1]-other.mat[1][1]);
    a21=fabs(mat[2][1]-other.mat[2][1]);

    a02=fabs(mat[0][2]-other.mat[0][2]);
    a12=fabs(mat[1][2]-other.mat[1][2]);
    a22=fabs(mat[2][2]-other.mat[2][2]);

    return (a00+a10+a20+a01+a11+a21+a02+a12+a22)<1E-7;
}

/*!
 * \brief Ссылка на элемент матрицы.
 * \details Возвращает изменяемую ссылку на элемент, лежащий на пересечении x-ой строки и y-ого столбца.
 * \throw std::invalid_argument - при индексе <1 или >3
 * \param x - индекс строки
 * \param y - индекс столбца
 * \return Изменяемую ссылку на элемент.
 * \warning Индексация производится с единицы.
 */
double &SMatrix_3x3::a(int x, int y)
{
    if (!isExist(x,y))
        throw  std::invalid_argument("SMatrix_3x3::a - Element with selected indexes does not exist");
    return mat[y-1][x-1];
}

/*!
 * \brief Вычисление собственных значений матрицы.
 * \details Собственные значения - корни характеристического уравнения det|A-xE|=0.
 * После алгебраических преобразований данное уравнение становится кубическим.
 * Для решения кубического уравнения используется метод Виете-Кардано (см. VieteCardanoMethod)
 * \return Упорядоченный по убыванию набор собственных значений.
 * \warning Реализация метода Виете-Кардано не подразумевает наличие комплексных корней.
 */
SVector_3D SMatrix_3x3::eigenvalues() const
{
    double A=-mat[0][0]-mat[1][1]-mat[2][2];
    double Buf0=(mat[1][2]*mat[2][1]);
    double Buf1=(mat[0][2]*mat[2][0]);
    double Buf2=(mat[0][1]*mat[1][0]);
    double B=(mat[0][0]*mat[1][1])+(mat[0][0]*mat[2][2])+(mat[1][1]*mat[2][2])
            -Buf2-Buf1-Buf0;

    double C=(mat[0][0]*Buf0)+(mat[1][1]*Buf1)+(mat[2][2]*Buf2)
            -(mat[0][0]*mat[1][1]*mat[2][2])
            -(mat[0][1]*mat[1][2]*mat[2][0])
            -(mat[0][2]*mat[1][0]*mat[2][1]);

    return VieteCardanoMethod(A,B,C).sort();
}

/*!
 * \brief Вычисление собственных векторов матрицы.
 * \details Ненулевой вектор b называется собственным для матрицы A если A*b=l*b, где l - собственное значение вектора.
 * Собственные значения можно найти с помощью eigenvalues(). Результат - матрица, в столбцах которой находятся собственные вектора,
 * расположенные в порядке убывания их собственных значений и нормированные на единицу.
 * \param eigenValues - упорядоченный по убыванию набор собственных значений.
 * \return Матрицу с собственными значениями в столбцах.
 */
SMatrix_3x3 SMatrix_3x3::eigenvectors(const SVector_3D &eigenValues) const
{
    SMatrix_3x3 fir=(*this)-SMatrix_3x3(eigenValues.x);
    SMatrix_3x3 sec=(*this)-SMatrix_3x3(eigenValues.y);
    SMatrix_3x3 thd=(*this)-SMatrix_3x3(eigenValues.z);

    SMatrix_3x3 fs=fir*sec;
    SMatrix_3x3 ft=fir*thd;
    SMatrix_3x3 st=sec*thd;

    SVector_3D F=st.row(1)+st.row(2)+st.row(3);
    SVector_3D S=ft.row(1)+ft.row(2)+ft.row(3);
    SVector_3D T=fs.row(1)+fs.row(2)+fs.row(3);

    F/=fabs(F.x)+fabs(F.y)+fabs(F.z);
    S/=fabs(S.x)+fabs(S.y)+fabs(S.z);
    T/=fabs(T.x)+fabs(T.y)+fabs(T.z);

    return SMatrix_3x3(F,S,T);
}

/*!
 * \brief Вывод матрицы в отладочную консоль.
 */
void SMatrix_3x3::print()
{
    qDebug()<<"{"<<mat[0][0]<<"|"<<mat[0][1]<<"|"<<mat[0][2]<<"}";
    qDebug()<<"{"<<mat[1][0]<<"|"<<mat[1][1]<<"|"<<mat[1][2]<<"}";
    qDebug()<<"{"<<mat[2][0]<<"|"<<mat[2][1]<<"|"<<mat[2][2]<<"}";
}

/*SVector_3D SMatrix_3x3::eigenvector(double eigenvalue) const
{
    SMatrix_3x3 m=(*this)-SMatrix_3x3(eigenvalue);
    SVector_3D b;
    b.x=(m.mat[0][0]*m.mat[1][2] - m.mat[0][2]*m.mat[1][0])
            *(m.mat[1][0]*m.mat[2][1] - m.mat[2][0]*m.mat[1][1]);

    b.y=(m.mat[1][1]*m.mat[2][2] - m.mat[1][2]*m.mat[2][1])
            *(m.mat[0][1]*m.mat[1][0] - m.mat[1][1]*m.mat[0][0]);

    b.z=(m.mat[0][1]*m.mat[1][0] - m.mat[1][1]*m.mat[0][0])
            *(m.mat[1][0]*m.mat[2][1] - m.mat[2][0]*m.mat[1][1]);
    double sum=b.x+b.y+b.z;
    b.x/=sum;b.y/=sum;b.z/=sum;
    return b;
}*/
