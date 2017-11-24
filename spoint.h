#ifndef SPOINT_H
#define SPOINT_H

#include <QPoint>

class SPoint:public QPoint
{
public:
    SPoint():QPoint(){}
    SPoint(int x,int y):QPoint(x,y){}
    bool operator<(const SPoint& other) const
    {return (y()==other.y())?(x()<other.x()):(y()<other.y());}
};

#endif // SPOINT_H
