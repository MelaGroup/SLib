#include "sdataframe.h"
/*!
 * \brief Копирующий конструктор.
 * \param other - копируемая таблица
 */
SDataFrame::SDataFrame(const SDataFrame &other)
{
    this->operator =(other);
}

/*!
 * \brief Перемещающий конструктор.
 * \param other - временная таблица(rvalue)
 */
SDataFrame::SDataFrame(SDataFrame &&other)
{
    this->operator =(other);
}

/*!
 * \brief Установка имен признаков.
 * \details На каждое имя признака из header_list будет заведена колонка.
 *  В каждой колонке будут храниться значения соответствующих признаков объектов.
 * \param header_list - лист с именами признаков
 * \warning Повторные вызовы setHeader удаляют все текущее содержимое таблицы.
 */
void SDataFrame::setHeader(const std::list<std::string> &header_list)
{
    header.clear();
    table.clear();
    _rows=0;

    int id=0;
    for (auto col_name:header_list)
    {
        header[id]=col_name;
        ++id;
    }
}

/*!
 * \brief Занесение данных о новом объекте.
 * \details Создает новую строку в таблице для нового объекта, после чего заносит значения признаков последовательно
 * в указанном порядке. Строка нового объекта создается в конце таблицы.
 * \param name - имя нового объекта
 * \param values - вектор признаков для данного объекта
 * \warning Контроль порядка следования значений признаков осуществляет пользователь.
 * \throw std::invalid_argument - при несовпадении числа признаков и размерности вектора признаков
 */
void SDataFrame::newObject(const std::string &name, const std::vector<double> &values)
{
    if (header.size()!=values.size()) throw std::invalid_argument("SDataFrame: container.size() and headerSize() must be equal");
    names[_rows]=name;
    ++_rows;
    table.emplace_back(values);
    assert(table.size()==size_t(_rows));
}

/*!
 * \brief Занесение данных о новом объекте.
 * \details Альтернатива newObject(const std::string &name, const std::vector<double> &values)
 * \param name - имя нового объекта
 * \param values - вектор признаков для данного объекта
 * \warning Контроль порядка следования значений признаков осуществляет пользователь.
 * \throw std::invalid_argument - при несовпадении числа признаков и размерности вектора признаков
 */
void SDataFrame::newObject(const std::string &name, const std::list<double> &values)
{  
    newObject(name,std::vector<double>(values.begin(),values.end()));
}

/*!
 * \brief Горизонтальная стыковка таблиц.
 * \details Обеспечивает процедуру горизонтального стыкования. Процедура возможна если выполнятся условия:
 * - Текущая таблица - пустая (будет эквивалентно копированию)
 * - Число строк - совпадает (при этом соответствующие имена могут быть различными)
 * В результате в конце текущей таблицы появятся все колонки стыкуемой таблицы в том же порядке в котором они были.
 * \param other - стыкуемая таблица
 * \return результирующая таблица
 * \warning Метод не может автоматически соотносить имена объектов. Важен порядок их следования в таблицах.
 * \throw std::invalid_argument - при неравенстве числа строк в таблицах (кроме случая пустой текущей таблицы)
 */
SDataFrame &SDataFrame::operator+=(const SDataFrame &other)
{
    if (_rows==0 && header.empty() && names.empty())
    {
        *this=other;
        return *this;
    }
    if(_rows!=other._rows)
    {
        qDebug()<<"SDataFrame: other.rows() and rows() must be equal)";
        throw std::invalid_argument(nullptr);
    }
    size_t cols_start=cols();
    for (int i=0;i<other.cols();++i)
        header[cols_start+i]=other.header.at(i);

    for (int i=0;i<_rows;++i)
    {
        std::copy_n(other.table[i].cbegin(),other.cols(),std::back_inserter(table[i]));
    }
    return *this;
}

/*!
 * \brief Выртикальная стыковка таблиц.
 * \details Обеспечивает процедуру вертикального стыкования. Процедура возможна если выполнятся условия:
 * - Текущая таблица - пустая (будет эквивалентно копированию)
 * - Число столбцов - совпадает (при этом соответствующие названия признаков могут быть различными)
 * В результате в конце текущей таблицы появятся все объекты стыкуемой таблицы в том же порядке в котором они были.
 * \param other - стыкуемая таблица
 * \return результирующая таблица
 * \warning Метод не может автоматически соотносить названия признаков. Важен порядок их следования в таблицах.
 * \throw std::invalid_argument - при неравенстве числа столбцов в таблицах (кроме случая пустой текущей таблицы)
 */
SDataFrame &SDataFrame::vstack(const SDataFrame &other)
{
    if (_rows==0 && header.empty() && names.empty())
    {
        *this=other;
        return *this;
    }
    if(cols()!=other.cols())
    {
        qDebug()<<"SDataFrame: other.cols() and cols() must be equal)";
        throw std::invalid_argument(nullptr);
    }
    for (int i=0;i<other.rows();++i)
        newObject(other.names.at(i),other.table[i]);
    return *this;
}

/*!
 * \brief Перемещающее присваивание.
 * \param other - временная таблица (rvalue)
 * \return текущую таблицу
 */
SDataFrame& SDataFrame::operator=(SDataFrame &&other)
{
    std::swap(_rows,other._rows);
    std::swap(header,other.header);
    std::swap(names,other.names);
    std::swap(table,other.table);
    return *this;
}

/*!
 * \brief Копирующее присваивание.
 * \param other - копируемая таблица
 * \return текущую таблицу
 */
SDataFrame &SDataFrame::operator=(const SDataFrame &other)
{
    _rows=other._rows;
    header=other.header;
    names=other.names;
    table=other.table;
    return *this;
}

/*!
 * \brief Доступ к строке по номеру.
 * \details Возвращает константную ссылку на запрашиваемую строку. Обратите внимание что метод не возвращает
 * строковых переменных, т.е манипулирует лишь со значениями признаков. 0 строка - строка первого объекта.
 * \param r - номер строки
 * \return вектор значений признаков
 * \throw std::invalid_argument - если такой строки не существует.
 */
const std::vector<double> &SDataFrame::row(int r) const
{
    if (-1>r || r>=_rows)
        throw std::invalid_argument("SDataFrame: there is no such row");
    return table[r];
}

/*!
 * \brief Доступ к столбцу по номеру.
 * \details Возвращает копию столбца. Обратите внимание что метод не возвращает
 * строковых переменных, т.е манипулирует лишь со значениями признаков. 0 столбец - столбец первого признака.
 * \param с - номер столбца
 * \return столбец значений признака для объектов
 * \throw std::invalid_argument - если такого столбца не существует.
 */
std::vector<double> SDataFrame::col(int c) const
{
    std::vector<double> col(_rows);
    if (-1<c || c<cols())
        for (int r=0;r<_rows;++r)
            col[r]=table[r][c];
    else
        throw std::invalid_argument("SDataFrame: there is no such col");
    return col;
}

/*!
 * \brief Нормирование таблицы.
 * \details Производит нормировку всех признаков на их математическое ожидание и СКО по формуле:
 * \code
 *  table[r][c] = (table[r][c]-MX)/sqrt(DX);
 * \endcode
 * Вычисление MX и DX производится для каждого признака по отдельности.
 * Метод изменяет текущую таблицу.
 * \return ссылка на текщую таблицу (нормированную)
 */
SDataFrame &SDataFrame::scale()
{
    for(size_t c=0;c<header.size();++c)
    {
        double MX=0;
        for (int r=0;r<_rows;++r)
            MX+=table[r][c];
        MX/=double(_rows);

        double DX=0;
        for (int r=0;r<_rows;++r)
            DX+=(table[r][c]-MX)*(table[r][c]-MX);
        DX/=double(_rows);

        for (int r=0;r<_rows;++r)
            table[r][c] = (table[r][c]-MX)/sqrt(DX);
    }
    return *this;
}


/*!
 * \brief Удаление строки.
 * \details Удаляет строку по указанному номеру со сдвигом вверх (все строки ниже поднимуться на 1 вверх).
 * Процедура удаления несуществующей строки будет проигнорированно (Появится отладочное сообщение).
 * Обратите внимание: 0 столбец - столбец первого признака.
 * \param row - номер удаляемой строки
 */
void SDataFrame::removeObject(int row)
{
    if (-1<row || row<_rows)
    {
        while (row<_rows-1)
        {
            std::swap(table[row],table[row+1]);
            std::swap(names[row],names[row+1]);
            ++row;
        }
        table.erase(table.begin()+row);
        names.erase(_rows-1);
        --_rows;
    }
    else
        qDebug()<<"SDataFrame: there is no such row";

}

/*!
* \brief Удаление столбца.
* \details Удаляет столбец по указанному номеру со сдвигом влево(все строки справа сдвинуться на 1 влево).
* Процедура удаления несуществующего столбца будет проигнорированно (Появится отладочное сообщение) .
* Обратите внимание: 0 столбец - столбец первого признака.
* \param col - номер удаляемого столбца
*/
void SDataFrame::removeColumn(int col)
{
    int _cols=int(header.size());
    if (-1<col && col<_cols)
    {
        for(int c=col;c<_cols-1;++c)
        {
            std::swap(header[c],header[c+1]);
            for(int row=0;row<_rows;++row)
                std::swap(table[row][c],table[row][c+1]);
        }


        header.erase(_cols-1);
        for(int row=0;row<_rows;++row)
            table[row].pop_back();

    }
    else
        qDebug()<<"SDataFrame: there is no such col";

}

/*!
 * \brief Сохранение таблицы в csv.
 * \details Сохраняет данную таблицу в формат CSV (вместе с названиями объектов и признаков).
 * В качестве разделителей строк - ";", столбцов - перенос строки.
 * В зарезервированной ячейке записывается "Names". Формат файлового пути path описывается в QFile.
 * \param path - (полное) имя файла
 * \return true - если запись проведена успешно.
 * \warning Метод перезаписывает файл, если указано имя уже существующего.
 */
bool SDataFrame::toCSV(const QString &path)
{
    QFile csv(path);
    if (!csv.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&csv);
    out<<"Names";
    for(size_t i=0;i<header.size();++i)
    {
        out<<";"<<QString::fromStdString(header[i]);
    }
    out<<"\n";
    for(size_t r=0;r<size_t(_rows);++r)
    {
        out<<QString::fromStdString(names[r]);
        for(size_t c=0;c<header.size();++c)
        {
            out<<";"<<table[r][c];
        }
        out<<"\n";
    }
    csv.close();
    return true;
}

/*!
 * \brief Конструктор по csv-таблице.
 * \details Конструирует таблицу по файлу CSV - при этом содержимое таблицы должно быть организованно
 * в виде таблицы "объекты-признаки". В качестве разделителей строк - ";", столбцов - перенос строки.
 * Формат файлового пути path описывается в QFile. Сохраненные методом toCSV файлы могут быть использованны
 * беспрепятственно.
 * \param path - (полное) имя файла
 * \warning Метод не контролирует возможные отклонения от формата и не имеет контроля ошибок.
 */
SDataFrame::SDataFrame(const QString &path)
{
    QFile csv(path);
    csv.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&csv);
    QStringList rude_header = in.readLine().split(';');
    std::list<std::string> clear_header;
    for (QString s:rude_header)
        clear_header.push_back(s.toStdString());
    clear_header.pop_front();
    setHeader(clear_header);

    while(!in.atEnd())
    {
        QStringList row = in.readLine().split(';');
        std::string name = row.takeFirst().toStdString();
        std::list<double> values;
        for (QString s:row)
            values.push_back(s.toDouble());
        newObject(name,values);
    }

    csv.close();
}


