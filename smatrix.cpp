#include "smatrix.h"


SMatrix::SMatrix(int cols, int rows):_height(rows),_width(cols)
{
    ptr=new int*[rows];
    for(int i=0;i<rows;++i)
        ptr[i]=new int[cols];
}

SMatrix::SMatrix(const SMatrix &src):SMatrix(src._width,src._height)
{
    for(int r=0;r<_height;++r)
        memcpy(ptr[r],src.ptr[r],sizeof(int)*_width);
}

SMatrix::SMatrix(SMatrix &&src)
{
    _height=src._height;_width=src._width;
    ptr=src.ptr; src.ptr=nullptr;
}

SMatrix::SMatrix(const QImage &src, const SFunctor &formula):SMatrix(src.width(),src.height())
{
    for(int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
            ptr[r][c]=formula(src.pixelColor(c,r));
}

int& SMatrix::operator()(int col,int row){return ptr[row][col];}
int SMatrix::operator()(int col,int row)const{return ptr[row][col];}

bool SMatrix::isValidPos(int col, int row) const
{
    return (-1<col && col<_width && -1<row && row<_height);
}


SMatrix& SMatrix::scale(int min,int max)
{
    int mat_min=ptr[0][0];
    int mat_max=ptr[0][0];
    for(int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
        {
            int cell=ptr[r][c];
            mat_min=(mat_min<cell)?mat_min:cell;
            mat_max=(mat_max<cell)?cell:mat_max;
        }
    for(int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
        {
            int cell=ptr[r][c];
            cell=(max-min)*(cell-mat_min)/(mat_max-mat_min)+min;
            ptr[r][c]=cell;
        }
    return *this;
}

SMatrix SMatrix::copy(int x, int y, int w, int h) const
{
    if (w<=0 || h<=0) throw std::invalid_argument("SMatrix::copy - bad width or height");

    SMatrix ret(w,h);
    for(int ry=0;ry<h;++ry)
        for(int rx=0;rx<w;++rx)
        {
            int nx=x+rx,ny=y+ry;
            ret.ptr[ry][rx]=isValidPos(nx,ny)?ptr[ny][nx]:0;
        }
    return ret;

    /*if (isValidPos(x,y) && isValidPos(x+w-1,y+h-1))
    {
        SMatrix ret(w,h);
        for(int ry=0;ry<h;++ry)
            for(int rx=0;rx<w;++rx)
                ret.ptr[ry][rx]=ptr[y+ry][x+rx];
        return ret;
    }
    else qDebug()<<"SMatrix: Rect(x,y,w,z) does not fit";
    return SMatrix(0,0);*/
}


int SMatrix::average() const
{
    int sum=0;
    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            sum+=ptr[y][x];
        }
    return sum/_width*_height;
}

QImage SMatrix::toImage() const
{
    QImage diagram(_width,_height,QImage::Format_RGB888);
    SMatrix copy_mat(*this);
    copy_mat.scale(0,255);
    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int cell=copy_mat.ptr[y][x];
            diagram.setPixel(x,y,qRgb(cell,cell,cell));
        }
    return diagram;
}

SMatrix::~SMatrix()
{
    for(int i=0;i<_height;++i)
        delete [] ptr[i];
    delete[] ptr;
}
