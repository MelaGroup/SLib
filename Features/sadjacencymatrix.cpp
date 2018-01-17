#include "sadjacencymatrix.h"
/*!
 * \brief Вычисление МПС для изображения.
 * \details Учитывает краевые пиксели изображения.
 * \param img - полутоновое изображение
 */
inline void SAdjacencyMatrix::calculate(const SMatrix &img)
{    
    for(int y=0;y<img.height();++y)for(int x=0;x<img.width();++x)
        {
            int center=img(x,y);
            for(int i=-radius;i<=radius;++i)for(int j=-radius;j<=radius;++j)
            {
                int xn=x+i,yn=y+j;                
                if (img.isValidPos(xn,yn))
                {
                    int neighbor=img(xn,yn);
                    ++matrix[center][neighbor];
                }                
            }
            --matrix[center][center];
        }
}

/*!
 * \brief Игнорирование черного фона.
 * \details Зануляет 0 строку и стобец матрицы смежности.
 */
void SAdjacencyMatrix::ignoreZero()
{
    for( auto& m: matrix)
    {
        matrix[0][m.first]=0;
        matrix[m.first][0]=0;
    }
}

/*!
 * \brief Конструктор по радиусу смежности.
 * \details Устнавливает радиус смежности, который нельзя впоследствии изменить.
 * \param radius - радиус смежности
 * \warning Отсутствует контроль redius<1.
 */
SAdjacencyMatrix::SAdjacencyMatrix(int radius):radius(radius)
{}

/*!
 * \brief Конструктор по изображению.
 * \details Строит МПС с указанным радиусом смежности и флагом фона для полутонового изображения.
 * \param img - полутоновое изображение.
 * \param radius - радиус смежности
 * \param ignore_zero - игнорирование черного фона
 * \warning Отсутствует контроль redius<1 и валидности img.
 */
SAdjacencyMatrix::SAdjacencyMatrix(const SMatrix&img, int radius, bool ignore_zero):radius(radius)
{rebuild(img,ignore_zero);}

/*!
 * \brief Вычисление Энергии.
 * \details Энергия = отношение суммы квадратов элементов матрицы к числу пикселей.
 * \return энергия
 */
double SAdjacencyMatrix::energy()
{
    using namespace std;
    double energy=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            energy+=(matrix[x][y]*matrix[x][y]);
        }
    }
    energy/=double(elements);
    assert(energy>=0);
    return energy;
}

/*!
 * \brief Вычисление Энтропии.
 * \details Энтропия равна сумме (N*log(N) если (0<N), иначе 0), нормированной на число пикселей, где N - элемент матрицы смежности.
 * \return энтропия
 */
double SAdjacencyMatrix::entropy()
{
    double entropy=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            int N=matrix[x][y];
            if (0<N) entropy+=N*log(N);
        }
    }
    entropy/=double(elements);
    return entropy;
}

/*!
 * \brief Вычисление Локальной однородности.
 * \details Локальная однородность равна сумме (элемент_матрицы[x][y]/(1+(x-y)^2), нормированной на число пикселей.
 * \return локальная однородность
 */
double SAdjacencyMatrix::localHomogenity()
{
    double homogenity=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            homogenity+=matrix[x][y]/(1+(x-y)*(x-y));
        }
    }
    homogenity/=double(elements);
    return homogenity;
}

/*!
 * \brief Вычисление Максимальной вероятности.
 * \details Максимальная вероятность равна максимальному из значений ячеек матрицы смежности, нормированному на число пикселей.
 * \return максимальная вероятность
 */
double SAdjacencyMatrix::maxProbability()
{
    double max_p=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            if (max_p<matrix[x][y]) max_p=matrix.at(x).at(y);
        }
    }
    max_p/=double(elements);
    return max_p;
}

/*!
 * \brief Вычисление Момента инерции.
 * \details Момент инерции равен сумме (элемент_матрицы[x][y]*(x-y)^2), нормированной на число пикселей.
 * \return момент инерции
 */
double SAdjacencyMatrix::inertiaMoment()
{
    double iner=0;
    for(auto& r: matrix)
    {
        int y=r.first;
        for(auto& c: matrix)
        {
            int x=c.first;
            iner+=(x-y)*(x-y)*matrix[x][y];
        }
    }
    iner/=double(elements);
    return iner;
}

/*!
 * \brief Вычисление Следа МПС.
 * \details След МПС равен сумме диагональных элементов, нормированной на число пикселей.
 * \return след МПС
 */
double SAdjacencyMatrix::trail()
{
    double tr=0;
    for(auto& r: matrix)
    {
        int i=r.first;
        tr+=matrix[i][i];
    }
    tr/=double(elements);
    assert(tr>=0);
    return tr;
}

/*!
 * \brief Вычисление Средней яркости.
 * \details Средняя яркость равна сумме по y (y * сумма по х(matrix[x][y])), нормированной на число пикселей.
 * \return средняя яркость
 */
double SAdjacencyMatrix::averageBrightness()
{
    double av=0,buffer;
    for(auto& r: matrix)
    {
        int y=r.first;
        buffer=0;
        for(auto& c: matrix)
        {
            int x=c.first;
            buffer+=matrix[x][y];
        }
        av+=y*buffer;
    }
    av/=double(elements);
    assert(av>=0);
    return av;
}


/*!
 * \brief Оператор сравнения.
 * \details Две матрицы считаются равными если соответствующие элементы матрицы МПС равны между собой.
 * \param other - сравниваемая МПС
 * \return true - если равны
 */
bool SAdjacencyMatrix::operator==(const SAdjacencyMatrix &other) const
{
    return (matrix==other.matrix);
}

/*!
 * \brief Перевычисляет МПС.
 * \details Осуществляет перевычисление матрицы смежности по указанному полутоновому изображению и значению флага фона.
 * Не меняет значение радиуса смежности.
 * \param img - полутоновое изображение
 * \param ignore_zero - игнорирование черного фона
 */
void SAdjacencyMatrix::rebuild(const SMatrix &img, bool ignore_zero)
{
    elements=img.width()*img.height();
    calculate(img);
    if (ignore_zero)
    {
        for(int y=0;y<img.height();++y)for(int x=0;x<img.width();++x)
            if (img(x,y)==0)
                --elements;
        assert(elements>=0);
        ignoreZero();
    }
}

/*!
 * \brief Создание листа с названиями признаков.
 * \details К названиям признаков можно добавлять некоторую приставку.
 * Итоговое название признака будет складываться из приставки и оригинального названия.
 * Например "Blue_1_Energy" - Энергия по МПС с шагом(радиусом)=1 по синему каналу. "Blue" в данном примере является приставкой.
 * \param prefix - приставка
 * \return лист с названиями признаков
 */
std::list<std::__cxx11::string> SAdjacencyMatrix::getHeader(const std::string& prefix)
{
    using namespace std;
    list<string> header;
    string str=prefix+to_string(radius);
    header.push_back(str+"_Energy");
    header.push_back(str+"_ENT");
    header.push_back(str+"_LUN");
    header.push_back(str+"_MPR");
    header.push_back(str+"_CON");
    header.push_back(str+"_TR");
    header.push_back(str+"_AV");
    return header;
}

/*!
 * \brief Создание листа со значениями признаков.
 * \details Признаки распологаются в том же порядке, что и в getHeader.
 * \return лист со значениями признаков
 */
std::list<double> SAdjacencyMatrix::getFeatures()
{
    std::list<double> features;
    features.push_back(energy());
    features.push_back(entropy());
    features.push_back(localHomogenity());
    features.push_back(maxProbability());
    features.push_back(inertiaMoment());
    features.push_back(trail());
    features.push_back(averageBrightness());
    return features;
}
/* работает странно
double SAdjacencyMatrix::correlationBrightness(double av) const
{
    if (av<0) av=averageBrightness();
    double nsum,dsum,numerator=0,denominator=0;
    for(int i=0;i<256;++i)
    {
        nsum=0,dsum=0;
        for(int j=0;j<256;++j)
        {
            nsum+=(j-av)*matrix[i][j];
            dsum+=matrix[i][j];
        }
        numerator+=(i-av)*nsum;
        denominator+=(i-av)*(i-av)*dsum;
    }
    double corr=numerator/denominator;
    return corr;
}
*/


