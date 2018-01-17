#include "ssegmentationmap.h"

/*!
 * \brief Конструктор
 * \details Задает все поля настоящей структуры: x,y, ширину и высоту описывающей прямоугольной области,
 * а так же площадь сегмента в пикселях.
 * \param x - верхняя левая x координата описывающей прямоугольной области
 * \param y - верхняя левая y координата описывающей прямоугольной области
 * \param w - ширина описывающей прямоугольной области
 * \param h - высота описывающей прямоугольной области
 * \param power - площадь сегмента в пикселях
 * \warning Отсутствует проверка на допустимость указанных значений.
 */
SSegment::SSegment(int x, int y, int w, int h, int power):x(x),y(y),w(w),h(h),power(power){}

/*!
 * \brief Оператор <
 * \details Обеспечивает сравнение сегментов по площади. Если площадь I сегмента < площади II сегмента,
 * то I сегмент < II сегмента.
 * \param seg - второй сравниваемый сегмент
 * \return true - если меньше чем seg
 */
bool SSegment::operator<(const SSegment &seg) const
{
    return power<seg.power;
}

/*!
 * \brief Геометрическое объединение.
 * \details Объединение сегментов = получение прямоугольной области, описывающей оба сегмента + суммирование их площадей.
 * Является необходимой операцией для обеспечения процедуры объединения сегментов на карте сегментации
 * SSegmentationMap через join.
 * \param seg - второй объединяемый сегмент
 */
void SSegment::operator+=(const SSegment &seg)
{
    int x1,y1,x2,y2;
    x1=(x<seg.x)?x:seg.x;
    y1=(y<seg.y)?y:seg.y;
    x2=(x+w<seg.x+seg.w)?(seg.x+seg.w-1):(x+w-1);
    y2=(y+h<seg.y+seg.h)?(seg.y+seg.h-1):(y+h-1);

    x=x1,y=y1,w=x2-x1+1,h=y2-y1+1;
    power+=seg.power;
}

/*!
 * \brief Описывающий прямоугольник.
 * \details Описывающая прямоуголная область содержит в себе все пиксели сегмента, при этом имея минимальные размеры.
 * Зазор между сегментом и границей прямоуголной области отсутствует.
 * \return описывающую прямоугольник.
 */
QRect SSegment::toRect(){return {x,y,w,h};}

/*!
 * \brief Заливка одноцветной связной области.
 * \details Заливка происходит алгоритмом строковой заливки с занесением в очередь.
 * Суть метода заключается в следующем:
 * - Занесение начальной точки в очередь если для нее имеет смысл заливка.
 *
 * Далее в цикле:
 *  - Извлекается точка из очереди
 *  - Для нее определяется левый и правый предел, формируя одноцветную связную линию.
 *  - От левого до правого предела линия заливается
 *  - На тех же пределах, но на верхней и нижней линиях:
 *      * Если точка левого предела - одноцветная, то поместить в очередь.
 *      * Если точка - одноцветная и ее левый сосед - неодноцветный, то поместить в очередь.
 *
 * Пока очередь не станет пустой.
 * \param value - цвет заливки (идентификатор сегмента)
 * \param x - х координата стартового пикселя
 * \param y - y координата стартового пикселя
 * \return Информацию о полученном сегменте
 * \throw std::invalid_argument - если (x,y) выпадает или цвет заливки (value) совпадает с заливаемым цветом.
 */
SSegment SSegmentationMap::floodFill(int value, int x, int y)
{
    using namespace std;
    if (!isValidPos(x,y)) throw invalid_argument("SSegmentationMap::floodFill - Invalid position");
    int orig=ptr[y][x];
    if (orig==value) throw invalid_argument("SSegmentationMap::floodFill - fill and original values must be different");
    int x1=x,y1=y,x2=x,y2=y;
    int power=0;

    deque<QPoint> points({QPoint(x,y)});
    do
    {
        QPoint p = points.front();
        points.pop_front();
        int fy = p.y(),llim=p.x(),rlim=p.x();
        while (isValidPos(llim,fy) && ptr[fy][llim]==orig) --llim;
        while (isValidPos(rlim,fy) && ptr[fy][rlim]==orig) ++rlim;
        ++llim;--rlim;

        for (int nx=llim;nx<=rlim;++nx)
            ptr[fy][nx]=value;

        power+=rlim-llim+1;
        if (llim<x1) x1=llim;
        if (fy<y1) y1=fy;
        if (x2<rlim) x2=rlim;
        if (y2<fy) y2=fy;

        if (fy-1>=0)
        {
            if (ptr[fy-1][llim]==orig)
                points.push_back({llim,fy-1});
            for (int nx = llim+1;nx<=rlim;++nx)
                if (ptr[fy-1][nx]==orig && ptr[fy-1][nx-1]!=orig)
                    points.push_back({nx,fy-1});
        }
        if (fy+1<height())
        {
            if (ptr[fy+1][llim]==orig)
                points.push_back({llim,fy+1});
            for (int nx = llim+1;nx<=rlim;++nx)
                if (ptr[fy+1][nx]==orig && ptr[fy+1][nx-1]!=orig)
                    points.push_back({nx,fy+1});
        }
    }
    while(!points.empty());
    return SSegment(x1,y1,x2-x1+1,y2-y1+1,power);
}

/*!
 * \brief Конструктор(копирующий) по приготовленному полутоновому изображению.
 * \details Обеспечивает создание карты сегментации, на которой сегментами будут являться одноцветные
 * связные области приготовленного полутонового изображения.
 * \param src - приготовленное полутоновое изображение
 */
SSegmentationMap::SSegmentationMap(const SMatrix &src):SMatrix(src){connectedAreas();}

/*!
 * \brief Конструктор(перемещающий) по приготовленному полутоновому изображению.
 * \details Обеспечивает создание карты сегментации, на которой сегментами будут являться одноцветные
 * связные области приготовленного полутонового изображения.
 * \param src - приготовленное полутоновое изображение
 */
SSegmentationMap::SSegmentationMap(SMatrix &&src):SMatrix(src){connectedAreas();}

/*!
 * \brief Копирующий конструктор
 * \details Создает эквивалентную копию существующей карты сегментации.
 * \param src - карта сегментации
 */
SSegmentationMap::SSegmentationMap(const SSegmentationMap &src):SMatrix(src),segments(src.segments){}

/*!
 * \brief Конструктор по размерам.
 * \deprecated Будет удален в послующих версиях.
 * \details Конструирует карту сегментации по заданным ширине и высоте. В целом не имеет практической пользы.
 * \param width - ширина карты
 * \param height - высота карты
 */
SSegmentationMap::SSegmentationMap(int width, int height):SMatrix(width,height){}

/*!
 * \brief Инициализация генератора случайных чисел.
 * \details Применяется для методов визуализации сегментов, например выбора цвета сегмента.
 * \return генератор случайных чисел
 */
std::default_random_engine &SSegmentationMap::global_urng() const
{
    static std::default_random_engine u{};
    return u;
}

/*!
 * \brief Генерация случайного числа в заданном отрезке.
 * \details Применяется для методов визуализации сегментов, например выбора цвета сегмента.
 * \param l - минимальное генерируемое значение
 * \param r - максимальное генерируемое значение
 * \return случайное число
 */
int SSegmentationMap::pick(int l, int r) const
{
    static std::uniform_int_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d(global_urng(),parm_t{l,r});
}


/*!
 * \brief Копирующее присваивание.
 * \details Создает эквивалентную копию существующей карты сегментации.
 * \param other - копируемая карта сегментации.
 * \return ссылка на текущую карту сегментации
 */
SSegmentationMap &SSegmentationMap::operator=(const SSegmentationMap &other)
{
    SMatrix::operator=(other);
    segments=other.segments;
    return *this;
}

/*!
 * \brief Оператор доступа к информации о сегменте.
 * \details Предоставляет информацию о размерах описывающего прямоугольника и площади в пикселях для сегмента
 * с указанным идентификатором.
 * \param id - идентификатор сегмента.
 * \return Изменяемую ссылку на информацию о сегменте
 * \warning Не используйте несуществующие идентификаторы. Список существующих можно получить через IDs()
 */
SSegment &SSegmentationMap::operator[](int id)
{
    return segments.at(id);
}

/*!
 * \brief Оператор доступа к информации о сегменте.
 * \details Предоставляет информацию о размерах описывающего прямоугольника и площади в пикселях для сегмента
 * с указанным идентификатором.
 * \param id - идентификатор сегмента.
 * \return Информацию о сегменте
 * \warning Не используйте несуществующие идентификаторы. Список существующих можно получить через IDs()
 */
SSegment SSegmentationMap::operator[](int id) const
{
    return segments.at(id);
}

/*!
 * \brief Объединение сегментов.
 * \details Осуществляет объединение сегментов на карте сегментации: второй сегмент будет залит цветом(идентификатором)
 * первого сегмента, после чего он будет удален. Первый сегмент будет результирующим. Несвязность данных областей не
 * является помехой (такая операция абсолютно валидна)
 * \param id1 - идентификатор первого сегмента
 * \param id2 - идентификатор второго сегмента
 * \warning Не используйте несуществующие идентификаторы. Список существующих можно получить через IDs()
 */
void SSegmentationMap::join(int id1, int id2)
{
    segments.at(id1)+=segments.at(id2);
    SSegment& seg = segments.at(id2);
    for (int y=seg.y;y<(seg.y+seg.h);++y)
        for(int x=seg.x;x<(seg.x+seg.w);++x)
            if (ptr[y][x]==id2) ptr[y][x]=id1;
    segments.erase(id2);
}

/*!
 * \brief Присоединение к окружению.
 * \details В расширенной (т.е (x-1;y-1;w+2;h+2)) описывающей прямоугольной области для данного сегмента
 *  составляется рейтинг его соседей по количеству пикселей каждого сегмента в указанной области(кроме сливаемого).
 * К соседу с наивысшим рейтингом присоединяется данный сегмент.
 * \param id - идентификатор сливаемого сегмента.
 * \return идентификатор сегмента окружения.
 * \remark Данный метод обладает существенным минусом, а именно возможной потери связности.
 * В дальнейшем он будет доработан.
 */
int SSegmentationMap::joinToEnviroment(int id)
{
    SSegment& seg = segments.at(id);
    std::map<int,int> counters;
    for (int y=seg.y-1;y<(seg.y+seg.h+2);++y)
        for(int x=seg.x-1;x<(seg.x+seg.w+2);++x)
        {        
            if (isValidPos(x,y))
                ++counters[ptr[y][x]];
        }
    counters.erase(id);
    
    int eid=(counters.begin()->first);
    for(auto c: counters)
        if (counters[eid]<c.second) eid=c.first;

    join(eid,id);
    return eid;
}

/*!
 * \brief Список идентификаторов сегментов.
 * \details Возвращает список идентификаторов всех выделенных сегментов
 * (кроме намеренно забытых через forgetSegment), в порядке возрастания.
 * \return список идентификаторов сегментов
 */
std::vector<int> SSegmentationMap::IDs() const
{
    std::vector<int> id_vec;
    id_vec.reserve(segments.size());
    for (auto s:segments)
        id_vec.push_back(s.first);
    return id_vec;
}

/*!
 * \brief Возвращает идентификатор сегмента с минимальной площадью.
 * \return идентификатор сегмента с минимальной площадью
 */
int SSegmentationMap::IDsmallest() const
{
    int min_id=segments.begin()->first;
    for (auto pair:segments)
        if (pair.second < segments.at(min_id)) min_id=pair.first;
    return min_id;
}

/*!
 * \brief Возвращает идентификатор сегмента с максимальной площадью.
 * \return дентификатор сегмента с максимальной площадью
 */
int SSegmentationMap::IDlargest() const
{

    int max_id=segments.begin()->first;
    for (auto pair:segments)
        if (segments.at(max_id)< pair.second) max_id=pair.first;
    return max_id;
}

/*!
 * \brief Получение полутонового изображения сегмента.
 * \details Используя карту сегментации и совместимое полутоновое изображение можно получить изображение сегмента на
 * черном фоне. Для этого нужно указать используемое полутоновое изображение и идентификатор сегмента. Размеры
 * получаемого изображения совпадают с размерами соответствующей описывающей прямоугольной области.
 * \param original - полутоновое изображение
 * \param id - идентификатор сегмента
 * \return полутоновое изображение сегмента на черном фоне
 * \warning Не используйте несуществующие идентификаторы. Список существующих можно получить через IDs().
 * Используйте только совместимые изображения.
 */
SMatrix SSegmentationMap::getSegment(const SMatrix &original, int id) const
{  
    const SSegment& seg=segments.at(id);
    SMatrix ret = original.copy(seg.x,seg.y,seg.w,seg.h);
    for(int y=0;y<ret.height();++y)
        for(int x=0;x<ret.width();++x)
            if (ptr[seg.y+y][seg.x+x]!=id)
                ret(x,y)=0;
    return ret;
}

/*!
 * \brief Забывание сегмента.
 * \details Применяется чтобы исключить сегмент из рассмотрения всеми(!) методами на данной карте.
 * При сегмент останется на карте, но вернуть информацию по нему или провести слияние не получится.
 * Используется при оптимизации (исключение мусорных или фоновых сегментов)
 * \param original - полутоновое изображение
 * \param id - идентификатор забываемого сегмента.
 */
void SSegmentationMap::forgetSegment(int id){segments.erase(id);}


/*!
 * \brief Комбинирование мелких с крупными.
 * \details Осуществляет слияние всех мелких сегментов (площадь < задаваемого порога) с окружающими их
 * более крупными.
 * \param power_threshold - порог минимального значения площади
 */
void SSegmentationMap::combine(int power_threshold)
{
    int min_id=segments.begin()->first;
    int max_id=segments.rbegin()->first;
    for(int id=min_id;id<=max_id;++id)
        if (isExist(id) && segments[id].power<power_threshold)
            joinToEnviroment(id);
}

/*!
 * \brief Выделение всех связных одноцветных областей.
 * \details Вспомогательный метод, обеспечивающий преобразование из полутонового изображения
 * в карту сегментации. Алгаритм работы следующий:
 *  - Из изображения вычиается максимальное значение, после чего все оригинальные пиксели имеют
 * отрицательные значения.
 *  - Происходит обход изображения:
 *      * если значение пикселя < 0, инициировать заливку новым идентификатором.
 *      * связать новый идентификатор с полученной информацией о сегменте и занести в словарь.
 */
void SSegmentationMap::connectedAreas()
{
    segments.clear();
    if (max()>=0)(*this)+=(-max()-1);//все оригинальные элементы теперь <0
    int id=0;
    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int pix = ptr[y][x];
            if (pix<0)
            {
                auto seg=floodFill(id,x,y);
                segments.insert({id,seg});
                ++id;
            }
        }
}

/*!
 * \brief Проверка идентификатора.
 * \param id - идентификатор сегмента.
 * \return true - если переданный идентификатор существует (не забыт).
 */
bool SSegmentationMap::isExist(int id) const
{
    return segments.find(id)!=segments.end();
}

/*!
 * \brief Проверка на валидность карты сегментации.
 * \return true - если списко сегментов не пустой.
 */
bool SSegmentationMap::isValid() const
{
    return !(segments.empty());
}

/*!
 * \brief Проверка на совпадение размеров.
 * \param src - сравниваемое полутоновое изображение
 * \return true - если совместимы
 */
bool SSegmentationMap::isCompatible(const SMatrix &src) const
{
    return SMatrix::isCompatible(src);
}

/*!
 * \brief Проверка на совпадение размеров.
 * \param src - цветное изображение
 * \return true - если совместимы
 */
bool SSegmentationMap::isCompatible(const QImage &src) const
{
    return SMatrix::isCompatible(src);
}


/*!
 * \brief Отображение карты.
 * \details Данный метод служит для визуализации содержимого карты сегментации.
 * Результат представляет собой цветное изображение в котором одному идентификатору соответствует случайно
 * выбранный уникальный цвет. Метод игнорирует забывание через forgetSegment.
 * \return визуализацию карты сегментации
 */
QImage SSegmentationMap::toImage() const
{
    QImage img(_width,_height,QImage::Format_RGB888);

    using namespace std;
    map<int,QColor> colors;

    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int pix=ptr[y][x];
            if (colors.find(pix)==colors.end())
                colors[pix]=QColor(pick(0,255),pick(0,255),pick(0,255));
            img.setPixelColor(x,y,colors.at(pix));
        }
    return img;
}

/*!
 * \brief Отображение карты на цветном изображении.
 * \details Данный метод служит для визуализации содержимого карты сегментации.
 * Результат представляет собой оригинальное цветное изображение с нанесенным на него цветными границами сегментов.
 * При этом границы сегментов могут быть двойными - по границе от каждого сегмента. Цвет границы - уникален для
 * каждого сегмента. Метод игнорирует забывание через forgetSegment.
 * \param img - оригинальное цветное изображение
 * \throw invalid_argument - если изображение не совместимо с картой по размерам
 */
void SSegmentationMap::onImage(QImage &img)
{
    using namespace std;
    if (!isCompatible(img))
        throw invalid_argument("SSegmentationMap::onImage - img and segmentation map must be compatible");
    map<int,QColor> colors;

    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int pix=ptr[y][x];
            if (colors.find(pix)==colors.end())
                colors[pix]=QColor(pick(0,255),pick(0,255),pick(0,255));

            for (int ny=-3;ny<=3;++ny)
                for (int nx=-3;nx<=3;++nx)
                    if (pix!=get(x+nx,y+ny,!pix))
                    {
                        img.setPixelColor(x,y,colors.at(pix));
                        break;
                    }
        }
}

/*!
 * \brief Конвертирование к полутоновому изображению.
 * \details Позволяет вернуть карту сегментации в виде полутонового изображения с идентификаторами сегментов в пикселях.
 * \return полутоновое изображение (карта сегментации)
 */
SMatrix SSegmentationMap::toMatrix() const
{
    return SMatrix(*this);
}

