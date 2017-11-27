#include "sdataframe.h"

/* Пример работы:
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
table.toCSV("C://test.csv");*/

void SDataFrame::setHeader(const std::list<std::string> &header_list)
{
    header.clear();
    table.clear();
    _rows=0;

    SRange<int> id;
    for (auto col_name:header_list)
        header[id()]=col_name;
}

void SDataFrame::newObject(const std::string &name, const std::vector<double> &values)
{
    if (header.size()!=values.size()) throw std::invalid_argument("SDataFrame: container.size() and headerSize() must be equal");
    names[_rows]=name;
    ++_rows;
    table.emplace_back(values);
    assert(table.size()==size_t(_rows));
}

void SDataFrame::newObject(const std::string &name, const std::list<double> &values)
{  
    newObject(name,std::vector<double>(values.begin(),values.end()));
}

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

void SDataFrame::removeObject(int row)
{
    if (row<_rows)
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

bool SDataFrame::toCSV(const QString &path)
{
    QFile csv(path);
    if (!csv.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&csv);
    out<<"Names;";
    for(size_t i=0;i<header.size();++i)
    {
        out<<QString::fromStdString(header[i])<<";";
    }
    out<<"\n";
    for(size_t r=0;r<size_t(_rows);++r)
    {
        out<<QString::fromStdString(names[r])<<";";
        for(size_t c=0;c<header.size();++c)
        {
            out<<table[r][c]<<";";
        }
        out<<"\n";
    }
    csv.close();
    return true;
}


