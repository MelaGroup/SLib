#ifndef SMATRIX_H
#define SMATRIX_H

#include <QImage>
#include <QDebug>

#include <SLib/Components/sfunctors.h>

/*!
 * \ingroup Core
 * \brief Класс полутонового изображения.
 * \details По сути является двумерной динамической матрицей из int. Содержимое ячеек - любые числа, входящие в int.
 * Обычно генерируется по некоторому цветному изображению(QImage) и указанному правилу преобразования к полутоновому(SFunctor)
 */
class SMatrix
{
protected:
    int _height;        //!< Высота изображения
    int _width;         //!< Ширина изображения
    int** ptr=nullptr;  //!< Указатель на двумерный динамический массив
    int _min=1;         //!< Минимальное значение по элементам ptr
    int _max=0;         //!< Максимальное значение по элементам ptr

    inline void seize(int cols,int rows);
    inline void release();
    inline void memcopy(const SMatrix& src);
    inline void refresh_limits();
    ///\brief Потеря актуальности пределов min и max.
    inline void ruin_limits(){_min=1;_max=0;}

public:
    SMatrix(int width,int height);
    SMatrix(const SMatrix& src);
    SMatrix(SMatrix&& src);
    SMatrix(const QImage & src,const SFunctor& formula=SFunctor());
    SMatrix copy(int x,int y,int w,int h) const;
    SMatrix copy(const QRect& rect) const;
    SMatrix& scale(int min,int max);
    SMatrix& operator=(const SMatrix& other);
    SMatrix& operator+=(int value);
    SMatrix& operator+=(const SMatrix& other);
    SMatrix& operator-=(const SMatrix& other);

    void swap(SMatrix& src);
    bool isValidPos(int col,int row)const;
    bool isCompatible(const SMatrix & src) const;
    bool isCompatible(const QImage& src) const;
    bool operator==(const SMatrix& other) const;

    int min();
    int max();
    int width() const;
    int height() const;
    int operator()(int col,int row)const;
    int& operator()(int col,int row);
    int get(int col,int row,const int out_value=0) const;
    void set(int col,int row,int value);

    void view(const QRect& rect) const;

    virtual QImage toImage();
    ~SMatrix();
};

QImage constructImage(const SMatrix &r, const SMatrix &g, const SMatrix &b);


#endif // SMATRIX_H
