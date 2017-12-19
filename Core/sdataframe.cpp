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

SDataFrame::SDataFrame(const SDataFrame &other)
{
    this->operator =(other);
}

SDataFrame::SDataFrame(SDataFrame &&other)
{
    this->operator =(other);
}

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

SDataFrame& SDataFrame::operator=(SDataFrame &&other)
{
    std::swap(_rows,other._rows);
    std::swap(header,other.header);
    std::swap(names,other.names);
    std::swap(table,other.table);
    return *this;
}

SDataFrame &SDataFrame::operator=(const SDataFrame &other)
{
    _rows=other._rows;
    header=other.header;
    names=other.names;
    table=other.table;
    return *this;
}

const std::vector<double> &SDataFrame::row(int r) const
{
    if (-1>r || r>=_rows)
        throw std::invalid_argument("SDataFrame: there is no such row");
    return table[r];
}


std::vector<double> SDataFrame::col(int c) const
{
    std::vector<double> col(_rows);
    if (-1<c || c<cols())
        for (int r=0;r<_rows;++r)
            col[r]=table[r][c];
    else
        throw std::invalid_argument("SDataFrame: there is no such row");
    return col;
}

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


