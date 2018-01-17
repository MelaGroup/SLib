#ifndef SSEGMENTATIONMAP_H
#define SSEGMENTATIONMAP_H

#include <random>
#include <set>
#include <deque>
#include <tuple>
#include <typeinfo>
#include <algorithm>
#include <SLib/Core/smatrix.h>
#include <assert.h>


/*!
 * \ingroup Core
 * \brief Класс сегмента.
 * \details Несмотря на название, содержит исключительно первычную информацию о сегменте,
 * а именно его площадь (в пикселях), а так же параметры описывающей его прямоугольной области.
 */
struct SSegment
{
    int x;      //!< Верхняя левая x координата описывающей прямоугольной области
    int y;      //!< Верхняя левая y координата описывающей прямоугольной области
    int w;      //!< Ширина описывающей прямоугольной области
    int h;      //!< Высота описывающей прямоугольной области
    int power;  //!< Площадь сегмента в пикселях

    SSegment(int x=0,int y=0,int w=0,int h=0,int power = -1);
    bool operator<(const SSegment& seg) const;
    void operator+=(const SSegment& seg);
    QRect toRect();
};


/*!
 * \ingroup Core
 * \brief Карта сегментации.
 * \details Под картой сегментации понимается полутоновое изображение в пикселях которого находятся
 * идентификаторы сегментов. Таким образом, чтобы узнать к каком сегменту отностися пиксель(x,y) нужно
 * посмотреть значение пикселя(x,y) на карте сегментации.
 * Для построения такой карты используется заливка всех связных областей на приготовленном изображении.
 * Область на приготовленном изображении будет выделенна как сегмент если она а) одноцветная б) связная.
 * Так же содержит информацию о выделенных сегментах.
 */
class SSegmentationMap:protected SMatrix
{
protected:
    std::map<int,SSegment> segments; //!< словарь из выделенных сегментов
    std::default_random_engine& global_urng() const;

    int pick(int l,int r) const;    
    SSegment floodFill(int value,int x,int y);
public:
    SSegmentationMap(const SMatrix& src);
    SSegmentationMap(SMatrix&& src);
    SSegmentationMap(const SSegmentationMap& src);
    SSegmentationMap(int width=0,int height=0);
    SSegmentationMap &operator=(const SSegmentationMap &other);


    void join(int id1,int id2);
    int joinToEnviroment(int id);
    void combine(int power_threshold);

    void connectedAreas();

    bool isExist(int id) const;
    bool isValid() const;
    bool isCompatible(const SMatrix & src) const;
    bool isCompatible(const QImage& src) const;

    std::vector<int> IDs()const;
    int IDsmallest()const;
    int IDlargest()const;
    SMatrix getSegment(const SMatrix& original,int id) const;
    void forgetSegment(int id);


    SSegment& operator[](int id);
    SSegment operator[](int id) const;
    QImage toImage() const;
    virtual void onImage(QImage& img);
    virtual SMatrix toMatrix() const;
};

#endif // SSEGMENTATIONMAP_H
