#include "sthreshold.h"
/*!
 * \ingroup Processing
 * \brief Подбор порога бинаризации методом Отсу.
 * \details Позволяет выбрать адаптивное значение порога на полутоновом изображении методом Отсу (https://habrahabr.ru/post/112079/).
 * Суть метода - разделить 2 пика гистограммы оптимальным способом
 * (2 разделенные гистограммы имеют минимальную внутриклассовую дисперсию). Значение яркостей может быть не нормированно на [0;256).
 * \param src - полутоновое изображение
 * \return адаптивное значение порога
 */
int Otsu(const SMatrix &src)
{
    std::map<int,int> histogram;
    int min_key=std::numeric_limits<int>::max();
    int max_key=std::numeric_limits<int>::min();

    for(int r=0;r<src.height();++r)
        for(int c=0;c<src.width();++c)
        {
            int p=src(c,r);
            if (p<min_key) min_key=p;
            if (max_key<p) max_key=p;
            ++histogram[p];
        }

    int n=0,m=0;
    for(int i=min_key;i<=max_key;++i)
    {
        n+=histogram[i];
        m+=i*histogram[i];
    }

    int max_sigma=0,threshold=0;
    int a1=0,b1=0;

    for(int t=min_key;t<=max_key;++t)
    {
        a1+=t*histogram[t];
        b1+=histogram[t];

        float w1=(float)b1/n;
        float a=(float)a1/b1 - (float)(m - a1)/(n - b1);
        float sigma=w1*(1-w1)*a*a;
        if (sigma>max_sigma)
        {
            max_sigma=sigma;
            threshold=t;
        }
    }
    return threshold;
}


