#ifndef SPIE_H
#define SPIE_H
#include <smatrix.h>
#include <QVector>


class SPie: public QVector<SMatrix>
{
    int _width;
    int _height;
    int thickness;
public:
    SPie():_width(0),_height(0),thickness(0){}
    SPie(int width,int height,int thickness)
        :QVector(thickness,SMatrix(width,height))
        ,_width(width),_height(height),thickness(thickness){}
    SPie& scaleSlices(int min=0,int max=(_width+_height)/2);
    double distance(int x1,int y1,int x2,int y2) const;
    int height()const{return _height;}
    int width()const{return _width;}

    SMatrix getDataTable() const;
};

#endif // SPIE_H
