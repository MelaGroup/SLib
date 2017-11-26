#ifndef SDATAFRAME_H
#define SDATAFRAME_H

#include <map>
#include <vector>
#include <deque>
#include <QFile>
#include <assert.h>
#include <qdebug.h>
#include <SLib/sgenerators.h>

class SDataFrame
{
    int _rows=0;
    std::map<int,std::string> header;
    std::map<int,std::string> names;
    std::deque<std::vector<double>> table;

public:
    SDataFrame(){}
    int cols() const {return int(header.size());}
    int rows() const {return _rows;}

    void setHeader(const std::list<std::string>& header_list);
    void newObject(const std::string& name, const std::list<double> &values);

    /*Шаблоны не компилируются, на другом компиляторе норм
    template<typename It>
    void setHeader(const It& begin,const It& end);*/

    SDataFrame& operator+=(const SDataFrame& other);

    void removeObject(int row);
    virtual bool toCSV(const QString& path);
};
/*
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
