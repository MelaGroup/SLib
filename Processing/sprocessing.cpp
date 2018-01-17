#include "sprocessing.h"

/*!
 * \ingroup Processing
 * \brief Заливка одноцветной области.
 * \details Данная функция заливает одноцветную область цветом value, начиная с точки (x,y), на изображении src.
 * Реализованн алгоритм медленной заливки через очередь:
 * - выяснение оригинального цвета и его проверка на не равенство value.
 * - занесение начального пикселя в очередь и его окрашивание.
 * - Далее цикл:
 *  * извлекается пиксель из очереди.
 *  * его соседи оригинального цвета заносятся в очередь.
 *  * текущий пиксель окрашивается.
 * - До тех пор, пока очередь не станет пустой.
 * \param src - полутоновое изображение.
 * \param value - новый цвет области
 * \param x - x координата пикселя изображения
 * \param y - y координата пикселя изображения
 * \return число залитых пикселей
 * \todo вынести функцию в класс
 */
int floodFill(SMatrix &src, int value, int x, int y)
{
    if (src.isValidPos(x,y))
    {
        std::deque<QPoint> deque{{x,y}};
        int original_val=src(x,y);
        if (original_val==value) return -1;
        src(x,y)=value;
        int counter=0;

        while(!deque.empty())
        {
            int fx=deque.front().x(),fy=deque.front().y();
            for (int dy=-1;dy<=1;++dy)
                for (int dx=-1;dx<=1;++dx)
                {
                    int nx=fx+dx,ny=fy+dy;
                    if (src.isValidPos(nx,ny)&& src(nx,ny)==original_val)
                    {
                        src(nx,ny)=value;
                        deque.push_back({nx,ny});
                    }
                }
            deque.pop_front();
            ++counter;
        }
        return counter;
    }
    else qDebug()<<"bad position("<<x<<","<<y<<") in SMatrix";
    return -1;
}


