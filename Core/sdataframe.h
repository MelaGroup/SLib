#ifndef SDATAFRAME_H
#define SDATAFRAME_H

#include <map>
#include <vector>
#include <deque>
#include <QFile>
#include <assert.h>
#include <qdebug.h>
//#include <SLib/sgenerators.h>

class SDataFrame
{
    int _rows=0;
    std::map<int,std::string> header;
    std::map<int,std::string> names;
    std::deque<std::vector<double>> table;

public:
    SDataFrame(){}
    SDataFrame(const SDataFrame& other);
    SDataFrame(SDataFrame&& other);
    SDataFrame(const QString& path);
    int cols() const {return int(header.size());}
    int rows() const {return _rows;}

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
