#ifndef SGRID_H
#define SGRID_H

/*Класс-генератор задает сетку из rect-областей (x,y,w,h) для изображения*/

template<typename rect>
class SGrid
{
private:
    int dx,dy;
public:
    SGrid(int deltaX,int deltaY):dx(deltaX),dy(deltaY){}
    rect operator()(int col,int row)const
    {
        return rect({col*dx,row*dy,dx,dy});
    }

};

#endif // SGRID_H
