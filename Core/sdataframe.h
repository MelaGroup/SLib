#ifndef SDATAFRAME_H
#define SDATAFRAME_H

#include <map>
#include <vector>
#include <deque>
#include <QFile>
#include <assert.h>
#include <qdebug.h>

/*!
 * \ingroup Core
 * \brief Таблица "Объекты-признаки".
 * \details Таблица "Объекты-признаки" устроена следующим образом:
 * - первая ячейка таблицы зарезервированна.
 * - в первой строке написаны названия признаков (название на колонку).
 * - в первом столбце написаны имена объектов (имя на строку).
 * - На пересечении строки, соответствующей объекту, и столбца, соответствующего признаку,
 * записывается значение признака для данного объекта.
 * - индексация ведется начиная с ячейкой первого признака первого объекта
 *
 * Пример работы:
\code
SDataFrame table;
std::list<std::string> header{"first","second","third"};
std::list<std::string> names={"Vasya","Petya","Misha"};
std::list<double> VMarks{1.,2.,1.},PMarks{3.,2.,3.},MMarks{5.,4.,5.};
auto it=names.begin();

table.setHeader(header);
table.newObject(*it++,VMarks);
table.newObject(*it++,PMarks);
table.newObject(*it++,MMarks);
table.removeObject(1);
table.toCSV("C://test.csv");
\endcode
 */
class SDataFrame
{
    int _rows=0;                            //!< число объектов
    std::map<int,std::string> header;       //!< словарь с названиями признаков
    std::map<int,std::string> names;        //!< словарь с именами объектов
    std::deque<std::vector<double>> table;  //!< матрица со значениями признаков объектов

public:
    SDataFrame(){}                          //!< Пустой конструктор.
    SDataFrame(const SDataFrame& other);
    SDataFrame(SDataFrame&& other);
    SDataFrame(const QString& path);
    int cols() const {return int(header.size());}   //!< Число признаков.
    int rows() const {return _rows;}                //!< Число объектов.

    void setHeader(const std::list<std::string>& header_list);
    void newObject(const std::string &name, const std::vector<double> &values);
    void newObject(const std::string& name, const std::list<double> &values);

    SDataFrame& operator+=(const SDataFrame& other);
    SDataFrame& vstack(const SDataFrame& other);
    SDataFrame& operator=(const SDataFrame& other);
    SDataFrame& operator=(SDataFrame&& other);

    const std::vector<double> &row(int r) const;

    std::vector<double> col(int c) const;

    SDataFrame& scale();

    void removeObject(int row);
    void removeColumn(int col);
    virtual bool toCSV(const QString& path);

};

/*Шаблоны не компилируются, на другом компиляторе норм
    template<typename It>
    void setHeader(const It& begin,const It& end);
template <typename It>
void SDataFrame::setHeader(const It& begin,const It& end)
{
    header.clear();
    table.clear();
    _rows=0;

    SRange<int> id;
    for (It i=begin;i!=end;++i)
        header[id()]=*i;
}*/

#endif // SDATAFRAME_H
