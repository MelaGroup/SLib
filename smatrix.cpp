#include "smatrix.h"


void SMatrix::seize(int cols, int rows)
{
    _height=rows,_width=cols;
    ptr=new int*[rows];
    for(int i=0;i<rows;++i)
        ptr[i]=new int[cols];
}

void SMatrix::release()
{
    for(int i=0;i<_height;++i)
        delete [] ptr[i];
    delete[] ptr;
}

void SMatrix::memcopy(const SMatrix &src)
{   
    for(int r=0;r<_height;++r)
        memcpy(ptr[r],src.ptr[r],sizeof(int)*_width);
    _max=src._max;
    _min=src._min;
}

void SMatrix::refresh_limits()
{
    if (_max<_min)
    {
        _min=ptr[0][0];
        _max=ptr[0][0];
        for(int r=0;r<_height;++r)
            for(int c=0;c<_width;++c)
            {
                int cell=ptr[r][c];
                if (cell<_min) _min=cell;
                if (_max<cell) _max=cell;
            }
    }
}

void SMatrix::swap(SMatrix &src)
{
    std::swap(_height,src._height);
    std::swap(_width,src._width);
    std::swap(ptr,src.ptr);
    std::swap(_min,src._min);
    std::swap(_max,src._max);
}

SMatrix::SMatrix(int cols, int rows)
{
    seize(cols,rows);
}

SMatrix::SMatrix(const SMatrix &src)
{
    seize(src._width,src._height);
    memcopy(src);
}

SMatrix::SMatrix(SMatrix &&src)
{
    swap(src);
}

SMatrix::SMatrix(const QImage &src, const SFunctor &formula):SMatrix(src.width(),src.height())
{
    for(int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
            ptr[r][c]=formula(src.pixelColor(c,r));
    refresh_limits();
}

SMatrix &SMatrix::operator=(const SMatrix &other)
{
    release();
    seize(other._width,other._height);
    memcopy(other);
    return *this;
}

SMatrix &SMatrix::operator+=(const SMatrix &other)
{
    if(!isCompatible(other))
        throw std::invalid_argument("SMatrix: sizes must be equal");
    for(int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
            ptr[r][c]+=other.ptr[r][c];
    return *this;
}

bool SMatrix::isValidPos(int col, int row) const
{
    return (-1<col && col<_width && -1<row && row<_height);
}

bool SMatrix::isCompatible(const SMatrix &src) const
{
    return src._width==_width && src.height()==_height;
}

bool SMatrix::isCompatible(const QImage &src) const
{
    return (src.width()==_width && src.height()==_height);
}

bool SMatrix::operator==(const SMatrix &other) const
{
    for (int r=0;r<_height;++r)
        for(int c=0;c<_width;++c)
            if (ptr[r][c]!=other.ptr[r][c]) return false;
    return true;
}

SMatrix &SMatrix::operator+=(int value)
{
    _min+=value;_max+=value;
    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            ptr[y][x]+=value;
        }
    return *this;
}

int SMatrix::min()
{
    refresh_limits();
    return _min;
}

int SMatrix::max()
{
    refresh_limits();
    return _max;
}

int& SMatrix::operator()(int col,int row)
{
    ruin_limits();
    return ptr[row][col];
}

int SMatrix::get(int col, int row, const int out_value) const
{
    if (isValidPos(col,row))
        return ptr[row][col];
    return out_value;
}

void SMatrix::set(int col, int row, int value)
{
    if (isValidPos(col,row))
    {
        ptr[row][col]=value;
        if (value<_min) _min=value;
        if (_max<value) _max=value;
    }

}

void SMatrix::view(const QRect &rect) const
{
    int x1=rect.x(),y1=rect.y();
    int x2=x1+rect.width()-1,y2=y1+rect.height()-1;
    if (isValidPos(x1,y1) && isValidPos(x2,y2))
    {
        for(int j=y1;j<=y2;++j)
        {
            QString row="{";
            for(int i=x1;i<=x2;++i)
            {
                row+=QString().number(ptr[j][i])+";";
            }
            row.chop(1);row+="}";
            qDebug()<<row;
        }
    }
}
int SMatrix::operator()(int col,int row)const{return ptr[row][col];}


SMatrix& SMatrix::scale(int min,int max)
{       
    if (ptr!=nullptr)
    {
        refresh_limits();
        if (_min==min && _max==max) return *this;

        const int mat_min=_min,mat_max=_max;
        auto scaler = [min,max,mat_min,mat_max](int x)
        {return (max-min)*(x-mat_min)/(mat_max-mat_min)+min;};

        for(int r=0;r<_height;++r)
            for(int c=0;c<_width;++c)
            {
                int cell=ptr[r][c];
                cell=scaler(cell);
                ptr[r][c]=cell;
            }
    }
    return *this;
}

SMatrix SMatrix::copy(int x, int y, int w, int h) const
{
    if (w<=0 || h<=0) throw std::invalid_argument("SMatrix::copy - bad width or height");
    if (!isValidPos(x,y)) throw std::invalid_argument("SMatrix::copy - bad pos");

    SMatrix ret(w,h);
    for(int ry=0;ry<h;++ry)
        for(int rx=0;rx<w;++rx)
        {
            int nx=x+rx,ny=y+ry;
            ret.ptr[ry][rx]=isValidPos(nx,ny)?ptr[ny][nx]:0;
        }
    return ret;
}

SMatrix SMatrix::copy(const QRect &rect) const
{
    return copy(rect.x(),rect.y(),rect.width(),rect.height());
}

QImage SMatrix::toImage()
{
    QImage diagram(_width,_height,QImage::Format_RGB888);
    refresh_limits();
    const int mat_min=_min,mat_max=_max;
    auto scaler = [mat_min,mat_max](int x)
    {
        if (mat_max==255 && mat_min==0) return x;
        return 255*(x-mat_min)/(mat_max-mat_min);
    };

    for(int y=0;y<_height;++y)
        for(int x=0;x<_width;++x)
        {
            int cell=scaler(ptr[y][x]);
            diagram.setPixel(x,y,qRgb(cell,cell,cell));
        }
    return diagram;
}
QImage constructImage(const SMatrix &r, const SMatrix &g, const SMatrix &b)
{
    QImage img;
    if (r.isCompatible(g) && r.isCompatible(b))
    {
        img=QImage(r.width(),r.height(),QImage::Format_RGB888);
        for(int y=0;y<img.height();++y)
            for(int x=0;x<img.width();++x)
                img.setPixel(x,y,qRgb(r(x,y),g(x,y),b(x,y)));
    }
    else throw std::invalid_argument("collectImage: r,g, and b must be compatible");
    return img;
}

SMatrix::~SMatrix()
{
    release();
}
