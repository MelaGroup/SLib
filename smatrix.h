#ifndef SMATRIX_H
#define SMATRIX_H

#include <QImage>
#include <SLib/sfunctors.h>

class SMatrix
{
protected:
    int _height,_width;
    int** ptr=nullptr;
    inline void seize(int cols,int rows);
    inline void release();
    inline void memcopy(const SMatrix& src);

public:
    SMatrix(int width,int height);
    SMatrix(const SMatrix& src);
    SMatrix(SMatrix&& src);
    SMatrix(const QImage & src,const SFunctor& formula=SFunctor());
    SMatrix&operator=(const SMatrix& other);

    inline void swap(SMatrix& src);
    int width() const {return _width;}
    int height() const {return _height;}
    int& operator()(int col,int row);
    int operator()(int col,int row)const;
    bool isValidPos(int col,int row)const;
    SMatrix& scale(int min,int max);
    SMatrix copy(int x,int y,int w,int h) const;
    SMatrix copy(QRect rect) const;


    int average()const;
    virtual QImage toImage() const;

    ~SMatrix();
};


#endif // SMATRIX_H
