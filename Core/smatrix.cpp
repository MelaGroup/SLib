#include "smatrix.h"

/*!
 * \brief Выделение памяти.
 * \details Осуществляет выделение памяти для двумерного массива ptr укзанных размеров динамически,
 * посредством множественных вызовов оператора new[].
 * \param cols - ширина двумерного массива
 * \param rows - высота двумерного массива
 */
void SMatrix::seize(int cols, int rows)
{
    _height=rows,_width=cols;
    ptr=new int*[rows];
    for(int i=0;i<rows;++i)
        ptr[i]=new int[cols];
}

/*!
 * \brief Освобождение памяти.
 * \details Осуществляет освобождение памяти, выделенной для двумерного массива ptr (посредством seize),
 * посредством множественных вызовов оператора delete[].
 */
void SMatrix::release()
{
    for(int i=0;i<_height;++i)
        delete [] ptr[i];
    delete[] ptr;
}

/*!
 * \brief Копирование изображения.
 * \details Производит копирование содержимого двумерной матрицы и остальных полей из src.
 * \param src - копируемое полутоновое изображение
 * \todo Проверить на утечки памяти. Возможно нужен предварительный вызов release().
 */
void SMatrix::memcopy(const SMatrix &src)
{   
    for(int r=0;r<_height;++r)
        memcpy(ptr[r],src.ptr[r],sizeof(int)*_width);
    _max=src._max;
    _min=src._min;
}

/*!
 * \brief Обновление полей минимального и максимального значений ячеек двумерной матрицы.
 * \details Принято, что если _max<_min, то пределы являются невалидными и их необходимо найти заново, обойдя все изображение.
 * Обычно состояние _max<_min вызывается намеренно через ruin_limits().
 */
void SMatrix::refresh_limits()
{
    if (_max<_min)
    {
        _min=ptr[0][0];
        _max=ptr[0][0];
        for(int r=0;r<_height;++r)
            for(int c=0;c<_width;++c)
            {
                int cell=ptr[r][c];
                if (cell<_min) _min=cell;
                if (_max<cell) _max=cell;
            }
    }
}

/*!
 * \brief Обмен(рокировка) данными между переменными.
 * \details Осуществляет полный обмен содержимыми полей. Таким образом в *this оказываются данные из src, а в src - данные из *this.
 * \param src - полутоновое изображение для обмена
 */
void SMatrix::swap(SMatrix &src)
{
    std::swap(_height,src._height);
    std::swap(_width,src._width);
    std::swap(ptr,src.ptr);
    std::swap(_min,src._min);
    std::swap(_max,src._max);
}

/*!
 * \brief Конструктор по размерам.
 * \details Создает изображение указанных размеров.
 * \param cols - ширина полутонового изображения
 * \param rows - высота полутонового изображения
 * \warning Не контролирует cols и rows на допустимые значения (должны быть >=0). Начальное содержимое ячеек - мусор.
 */
SMatrix::SMatrix(int cols, int rows)
{
    seize(cols,rows);
}

/*!
 * \brief Копирующий конструктор.
 * \details Осуществляет создание копии указанного полутонового изображения.
 * \param src - копируемое полутоновое изображение
 */
SMatrix::SMatrix(const SMatrix &src)
{
    seize(src._width,src._height);
    memcopy(src);
}

/*!
 * \brief Перемещающий конструктор.
 * \details Производит обмен данными между текущим и временным полутоновым изображением. По сути, отбирает данные временного объекта.
 * \param src - временное полутоновое изображение
 */
SMatrix::SMatrix(SMatrix &&src)
{
    swap(src);
}

/*!
 * \brief Конструктор по цветному изображению.
 * \details Осуществляет создание полутонового изображения по цветному (RGB) с помощью указанного правила.
 * О правилах преобразования подробнее в Components. По умолчанию яркость пикселя считается по формуле (11*RED + 16*GREEN + 5*BLUE)/32
 *
 * Пример:
 * \code
 * SMatrix blue_plane(image,S::Blue()); // Взятие синего канала изображения.
 * \endcode
 * \param src - цветное изображение
 * \param formula - правило преобразования
 * \todo Проверить на разных форматах QImage.
 */
SMatrix::SMatrix(const QImage &src, const SFunctor &formula):SMatrix(src.width(),src.height())
{
    for(int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
            ptr[r][c]=formula(src.pixelColor(c,r));
    refresh_limits();
}

/*!
 * \brief Копирующее присваивание.
 * \details Осуществляет создание копии указанного полутонового изображения.
 * \param other - копируемое полутоновое изображение
 * \return ссылку на текущее полутоновое изображение
 */
SMatrix &SMatrix::operator=(const SMatrix &other)
{
    release();
    seize(other._width,other._height);
    memcopy(other);
    return *this;
}

/*!
 * \brief Поэлементое +=
 * \details Прибавляет к каждому пикселю оригинального полутонового изображения соответствующий пиксель из прибавляемого изображения.
 * \param other - прибавляемое полутоновое изображение
 * \return ссылку на текущее полутоновое изображение
 * \throw std::invalid_argument - при разных размерах изображений
 */
SMatrix &SMatrix::operator+=(const SMatrix &other)
{
    if(!isCompatible(other))
        throw std::invalid_argument("SMatrix: sizes must be equal");
    for(int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
            ptr[r][c]+=other.ptr[r][c];
    ruin_limits();
    return *this;
}

/*!
 * \brief Поэлементое -=
 * \details Вычитает из каждого пикселя оригинального полутонового изображения соответствующий пиксель из вычитаемого изображения.
 * \param other - вычитаемое полутоновое изображение
 * \return ссылку на текущее полутоновое изображение
 * \throw std::invalid_argument - при разных размерах изображений
 */
SMatrix &SMatrix::operator-=(const SMatrix &other)
{
    if(!isCompatible(other))
        throw std::invalid_argument("SMatrix: sizes must be equal");
    for(int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
            ptr[r][c]-=other.ptr[r][c];
    ruin_limits();
    return *this;
}

/*!
 * \brief Проверка на выпадение.
 * \details Проверяет на существование указанного пикселя в данном изображении.
 * Если (x,y) принадлежит (0,0,width-1,height-1), то пиксель существует.
 * \param col - номер столбца (х)
 * \param row - номер строки (у)
 * \return true - если пиксель существует.
 */
bool SMatrix::isValidPos(int col, int row) const
{
    return (-1<col && col<_width && -1<row && row<_height);
}

/*!
 * \brief Проверка на равенство размеров.
 * \details Проверяет на совместимость текущего изображения с другим полутоновым.
 * Если их ширины и высоты совпадают, то они совместимы.
 * \param src - сравниваемое полутоновое изображение
 * \return true - если пиксель существует.
 */
bool SMatrix::isCompatible(const SMatrix &src) const
{
    return src._width==_width && src.height()==_height;
}

/*!
 * \brief Проверка на равенство размеров.
 * \details Проверяет на совместимость текущего изображения с цветным.
 * Если их ширины и высоты совпадают, то они совместимы.
 * \param src - сравниваемое полутоновое изображение
 * \return true - если пиксель существует.
 */
bool SMatrix::isCompatible(const QImage &src) const
{
    return (src.width()==_width && src.height()==_height);
}

/*!
 * \brief Проверка на эквивалентность.
 * \details Если размеры и все соответствующие пиксели изображений равны, то они эквивалентны.
 * \param other - сравниваемое полутоновое изображение
 * \return true - если эквивалентны
 */
bool SMatrix::operator==(const SMatrix &other) const
{
    if (!isCompatible(other)) return false;
    for (int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
            if (ptr[r][c]!=other.ptr[r][c]) return false;
    return true;
}

/*!
 * \brief Прибавление числа ко всем пикселям.
 * \details Осуществляет прибавление указанного числа ко всем пикселям изображения.
 * Данная операция эквивалентна прибавлению полутонового изображения в котором все яркости пикселей = данному числу.
 * \param value - прибавляемое число
 * \return ссылку на текущее изображение
 */
SMatrix &SMatrix::operator+=(int value)
{
    _min+=value;_max+=value;
    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            ptr[y][x]+=value;
        }
    return *this;
}

/*!
 * \brief Минимальное значение пикселя по изображению
 * \return минимальное значение пикселя по изображению
 */
int SMatrix::min()
{
    refresh_limits();
    return _min;
}

/*!
 * \brief Максимальное значение пикселя по изображению
 * \return Максимальное значение пикселя по изображению
 */
int SMatrix::max()
{
    refresh_limits();
    return _max;
}

/*!
 * \brief Ширина изображения
 * \return ширину изображения
 */
int SMatrix::width() const {return _width;}

/*!
 * \brief Высота изображения
 * \return высоту изображения
 */
int SMatrix::height() const {return _height;}

/*!
 * \brief Оператор доступа к пикселю.
 * \details Осуществляет прямой доступ к пикселю. Разрушает вычисленные пределы (_min и _max) и,
 * следовательно, приводит к их пересчету в дальнейшем.
 * \param col - номер столбца (x)
 * \param row - номер строки (y)
 * \return изменяемую ссылку на пиксель
 * \warning Не контролирует выпадние пикселей за допустимый диапазон. Для защиты от выпадений используйте set.
 */
int& SMatrix::operator()(int col,int row)
{
    ruin_limits();
    return ptr[row][col];
}

/*!
 * \brief Оператор доступа к пикселю.
 * \details Извлекает значение указанного пикселя. Не разрушает вычисленные пределы (_min и _max).
 * \param col - номер столбца (x)
 * \param row - номер строки (y)
 * \return значение пикселя
 * \warning Не контролирует выпадние пикселей за допустимый диапазон. Для защиты от выпадений используйте get.
 */
int SMatrix::operator()(int col,int row)const{return ptr[row][col];}

/*!
 * \brief Считываение значения пикселя (защищенное).
 * \details Осуществляет защищенное от выпадений считывание значений пикселей. При выпадении за пределы изображения значение
 * пикселя считается равным out_value, что может быть полезно при обходах изображения масками.
 * Не разрушает вычисленные пределы (_min и _max).
 * \param col - номер столбца (x)
 * \param row - номер строки (y)
 * \param out_value - значение выпадающего пикселя
 * \return значение пикселя
 */
int SMatrix::get(int col, int row, const int out_value) const
{
    if (isValidPos(col,row))
        return ptr[row][col];
    return out_value;
}

/*!
 * \brief Запись значения пикселя (защищенная).
 * \details Осуществляет защищенную от выпадений перезапись значений пикселей,
 * что может быть полезно при обходах изображения масками.
 * Не разрушает вычисленные пределы (_min и _max), а обновляет их при каждом вызове:
 * \code
 * //Обновление пределов
 *      if (value<_min) _min=value;
 *      if (_max<value) _max=value;
 * \endcode
 * \param col - номер столбца (x)
 * \param row - номер строки (y)
 * \param value - устанавливаемое значение
 */
void SMatrix::set(int col, int row, int value)
{
    if (isValidPos(col,row))
    {
        ptr[row][col]=value;
        if (value<_min) _min=value;
        if (_max<value) _max=value;
    }

}

/*!
 * \brief Отладочная проверка содержания.
 * \details Для отображение содержимого двумерной матрицы удобно выводить в консоль не всю матрицу, а некоторую ее
 * прямоугольную область.
 * \param rect - отображаемая область изображения
 * \warning Выпадения приводят к игнорированию выполнения функции
 */
void SMatrix::view(const QRect &rect) const
{
    int x1=rect.x(),y1=rect.y();
    int x2=x1+rect.width()-1,y2=y1+rect.height()-1;
    if (isValidPos(x1,y1) && isValidPos(x2,y2))
    {
        for(int j=y1;j<=y2;++j)
        {
            QString row="{";
            for(int i=x1;i<=x2;++i)
            {
                row+=QString().number(ptr[j][i])+";";
            }
            row.chop(1);row+="}";
            qDebug()<<row;
        }
    }
}




/*!
 * \brief Нормировка значений пикселей.
 * \details Призводит линейное преобразование значений пикселей, при котором минимальное и максимальное значения пикселей
 * становятся равными указанным min и max.
 * \code
 * const int mat_min=_min,mat_max=_max; // Замена переменных
 * auto scaler = [min,max,mat_min,mat_max](int x)
        {
            return (max-min)*(x-mat_min)/(mat_max-mat_min)+min; // линейное преобразование
        };
 * \endcode
 * \param min - новое минимальное значение
 * \param max - новое максимальное значение
 * \return ссылку на текущее изображение
 */
SMatrix& SMatrix::scale(int min,int max)
{       
    if (ptr!=nullptr)
    {
        refresh_limits();
        if (_min==min && _max==max) return *this;

        const int mat_min=_min,mat_max=_max;
        auto scaler = [min,max,mat_min,mat_max](int x)
        {return (max-min)*(x-mat_min)/(mat_max-mat_min)+min;};

        for(int r=0;r<_height;++r)
            for(int c=0;c<_width;++c)
            {
                int cell=ptr[r][c];
                cell=scaler(cell);
                ptr[r][c]=cell;
            }
    }
    return *this;
}

/*!
 * \brief Копирование области.
 * \details Копирует указанную прямоугольную область изображения в новое изображение.
 * Значения выпадающих пикселей равны нулю.
 * \param x - x координата левого верхнего угла области
 * \param y - y координата левого верхнего угла области
 * \param w - ширина области
 * \param h - высота области
 * \return копию области изображения.
 * \throw std::invalid_argument - при отрицательных w и h или невалидной позиции (x,y)
 * \todo Сделать более осмысленной обработку выпадений.
 */
SMatrix SMatrix::copy(int x, int y, int w, int h) const
{
    if (w<=0 || h<=0) throw std::invalid_argument("SMatrix::copy - bad width or height");
    if (!isValidPos(x,y)) throw std::invalid_argument("SMatrix::copy - bad pos");

    SMatrix ret(w,h);
    for(int ry=0;ry<h;++ry)
        for(int rx=0;rx<w;++rx)
        {
            int nx=x+rx,ny=y+ry;
            ret.ptr[ry][rx]=isValidPos(nx,ny)?ptr[ny][nx]:0;
        }
    return ret;
}

/*!
 * \brief Копирование области.
 * \details Эквивалентно вызову copy(rect.x(),rect.y(),rect.width(),rect.height()).
 * \param rect - прямоугольная область
 * \return копию области изображения.
 */
SMatrix SMatrix::copy(const QRect &rect) const
{
    return copy(rect.x(),rect.y(),rect.width(),rect.height());
}

/*!
 * \brief Преобразование к серому изображению.
 * \details Перед преобразованием, полутоновое изображение нормируется на [0;255].
 * Если оно уже нормированно на [0;255], данный шаг будет опущен. После создается цветное изображение, в котором
 * R=G=B=значению пикселя нормированного изображения.
 * \return серое изображение
 * \warning Вызов функции приведет к изменению исходного изображения - оно будет нормированно на [0;255]!
 */
QImage SMatrix::toImage()
{
    QImage diagram(_width,_height,QImage::Format_RGB888);
    refresh_limits();
    const double mat_min=_min,mat_max=_max;
    auto scaler = [mat_min,mat_max](int x)
    {
        if (mat_max==255 && mat_min==0) return x;
        return int(255.*(double(x)-mat_min)/(mat_max-mat_min));
    };

    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int cell=scaler(ptr[y][x]);
            diagram.setPixel(x,y,qRgb(cell,cell,cell));
        }
    return diagram;
}
/*!
 * \brief Деструктор полутонового изображения.
 * \details Отвечает за конечное освобождение ресурсов ptr через вызов release().
 */
SMatrix::~SMatrix()
{
    release();
}

/*!
 * \ingroup Core
 * \brief Построение цветного изображения по каналам.
 * \details Если 3 полутоновых изображения имеют яркости в отрезке [0;255], то их можно собрать в цветное изображение.
 * Для этого нужно выбрать назначение каждого из изображений. Так цвет пикселя(x,y) будет определяться кодом #FFSSTT:
 * - FF - яркость пикселя(x,y) из первого изображения.
 * - SS - яркость пикселя(x,y) из второго изображения.
 * - TT - яркость пикселя(x,y) из третьего изображения.
 * \param r - полутоновое изображение для красного канала
 * \param g - полутоновое изображение для зеленого канала
 * \param b - полутоновое изображение для синего канала
 * \return цветное изображение
 * \throw std::invalid_argument - при несовместимости размеров r,g,b
 * \warning Не осуществляет проверку на [0;255]
 * \remark Данная функция служит скорее для отладочной визуализации, чем для практического использования.
 * Создание полутоновых изображений - довольно дорогостоящая операция, а здесь их требуется 3.
 */
QImage constructImage(const SMatrix &r, const SMatrix &g, const SMatrix &b)
{
    QImage img;
    if (r.isCompatible(g) && r.isCompatible(b))
    {
        img=QImage(r.width(),r.height(),QImage::Format_RGB888);
        for(int y=0;y<img.height();++y)
            for(int x=0;x<img.width();++x)
                img.setPixel(x,y,qRgb(r(x,y),g(x,y),b(x,y)));
    }
    else throw std::invalid_argument("collectImage: r,g, and b must be compatible");
    return img;
}


