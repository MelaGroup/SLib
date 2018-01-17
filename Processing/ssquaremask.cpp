#include "ssquaremask.h"
/*!
 * \brief Конструктор по размерам и пользовательской диагонали.
 * \details Создает 4 маски с разными ориентациями с соответствующей диагональю.
 * О структуре масок написано в описании класса SSquareMask. Радиус и ширина(она же - высота) маски
 * связаны соотношением: ШИРИНА = 2*РАДИУС+1.
 * \param a - радиус маски
 * \param profile - задаваемая строка(столбец)
 * \warning Если размер profile не совпадет с шириной то маска станет квадратной усредняющей.
 * \todo Переписать аргумент profile через initializer_list, параметр а - избыточен.
 */
SSquareMask::SSquareMask(int a, std::vector<int> profile):side(a)
{
    mask_v = new int* [side];
    for (int i=0;i<side;++i)
        mask_v[i] = new int [side];
    mask_h = new int* [side];
    for (int i=0;i<side;++i)
        mask_h[i] = new int [side];
    mask_r = new int* [side];
    for (int i=0;i<side;++i)
        mask_r[i] = new int [side];
    mask_l = new int* [side];
    for (int i=0;i<side;++i)
        mask_l[i] = new int [side];

    if (profile.size() == size_t(side))
    {
        for (int i=0;i<side;++i) for (int j=0;j<side;++j)
            mask_v[i][j] = profile.at(i);

        for (int i=0;i<side;++i) for (int j=0;j<side;++j)
            mask_h[i][j] = profile.at(j);

        std::rotate(profile.rbegin(), profile.rbegin()+radius, profile.rend());
        for (int i=0;i<side;++i)
        {
            for (int j=0;j<side;++j)
                mask_r[i][j] = profile.at(j);
            std::rotate(profile.begin(), profile.begin()+1, profile.end());
        }

        std::rotate(profile.begin(), profile.begin()+radius, profile.end());
        for (int i=0;i<side;++i)
        {
            for (int j=0;j<side;++j)
                mask_l[i][j] = profile.at(j);
            std::rotate(profile.rbegin(), profile.rbegin()+1, profile.rend());
        }
    }
    else
    {
        for (int i=0;i<side;++i) for (int j=0;j<side;++j)
        {
            mask_v[i][j] = 1;
            mask_h[i][j] = 1;
            mask_r[i][j] = 1;
            mask_l[i][j] = 1;
        }
    }
}

/*!
 * \brief Обход изображения.
 * \details Выполняет обход изображения 4-мя созданными масками по всему полутоновому изображению.
 * При этом результирующее значение пикселя есть максимальное из результатов наложений масок для данного пикселя.
 * Яркость выпадающих пикселей равна 0.
 * \param src - полутоновое изображение
 * \return обработанная копия изображения
 * \todo Сделать обработку выпадений более осмысленной.
 */
SMatrix SSquareMask::bypass(const SMatrix &src)
{
    int v,h,r,l;
    SMatrix ret(src);
    for (int x=0;x<src.width();++x)
        for (int y=0;y<src.height();++y)
        {
            v=0;h=0;r=0;l=0;
            for (int i=-radius;i<=radius;++i)
                for (int j=-radius;j<=radius;++j)
                {
                    int xn=x+i, yn=y+j;
                    v+=src.get(xn,yn)*mask_v[i+radius][j+radius];
                    h+=src.get(xn,yn)*mask_h[i+radius][j+radius];
                    r+=src.get(xn,yn)*mask_r[i+radius][j+radius];
                    l+=src.get(xn,yn)*mask_l[i+radius][j+radius];
                }
            ret(x,y) = std::max(std::max(v,h), std::max(r,l));
        }
    return ret;
}

/*!
 * \brief Деструктор, освобождающий динамическую память.
 * \details Освобождает всю динамическую память, выделенную на маски.
 */
SSquareMask::~SSquareMask()
{
    for (int i=0;i<side;++i)
    {
        delete[] mask_v[i];
        delete[] mask_h[i];
        delete[] mask_r[i];
        delete[] mask_l[i];
    }
    delete[] mask_v;
    delete[] mask_h;
    delete[] mask_r;
    delete[] mask_l;
}
