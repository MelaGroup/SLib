#ifndef SMATRIX_H
#define SMATRIX_H

#include <QImage>
#include <SLib/sfunctors.h>


class SMatrix
{
protected:

    int _height,_width;
    int** ptr=nullptr;
    int _min=1;
    int _max=0;

    inline void seize(int cols,int rows);
    inline void release();
    inline void memcopy(const SMatrix& src);
    inline void refresh_limits();
    inline void ruin_limits(){_min=1;_max=0;}

public:
    SMatrix(int width,int height);
    SMatrix(const SMatrix& src);
    SMatrix(SMatrix&& src);
    SMatrix(const QImage & src,const SFunctor& formula=SFunctor());    
    SMatrix copy(int x,int y,int w,int h) const;
    SMatrix copy(QRect rect) const;
    SMatrix& scale(int min,int max);
    SMatrix& operator=(const SMatrix& other);
    SMatrix& operator+=(int value);
    inline void swap(SMatrix& src);
    bool isValidPos(int col,int row)const;

    int min();
    int max();
    int width() const {return _width;}
    int height() const {return _height;}
    int operator()(int col,int row)const;
    int& operator()(int col,int row);    

    virtual QImage toImage() const;
    ~SMatrix();
};


#endif // SMATRIX_H
