#ifndef SEDGEDETECTION_H
#define SEDGEDETECTION_H

#include "SLib/Core/smatrix.h"
#include "SLib/Processing/sprocessing.h"

/*!
 * \ingroup Processing
 * \brief Операторы Собеля
 * \details Позволяет вычертить границы объектов используя операторы Собеля:
 * \code
 * //Горизонтальный
 * int Gx[3][3] = {  {1,0,-1},
 *                   {2,0,-2},
 *                   {1,0,-1} };
 * //Вертикальный
 * int Gy[3][3] = {  {1,2,1},
 *                   {0,0,0},
 *                   {-1,-2,-1} };
 * \endcode
 */
class SSobelOperator : public SProcessing
{
private:
    int Gx[3][3] = { {1,0,-1},
                     {2,0,-2},
                     {1,0,-1} };
    int Gy[3][3] = { {1,2,1},
                     {0,0,0},
                     {-1,-2,-1} };
public:
    ///\brief Пустой конструктор.
    SSobelOperator(){}
    SMatrix bypass(SMatrix &src);
};

/*!
 * \ingroup Processing
 * \brief Операторы
 * \details Позволяет вычертить границы объектов  используя операторы Превитта:
 * \code
 * //Горизонтальный
 * int Gx[3][3] = { {-1,0,1},
 *                   {-1,0,1},
 *                   {-1,0,1} };
 * //Вертикальный
 * int Gy[3][3] ={ {-1,-1,-1},
 *                   {0,0,0},
 *                   {1,1,1} };
 * \endcode
 */
class SPrewittOperator : public SProcessing
{
private:
    int Gx[3][3] = { {-1,0,1},
                     {-1,0,1},
                     {-1,0,1} };
    int Gy[3][3] = { {-1,-1,-1},
                     {0,0,0},
                     {1,1,1} };
public:
    ///\brief Пустой конструктор.
    SPrewittOperator(){}
    SMatrix bypass(SMatrix &src);
};

/*!
 * \ingroup Processing
 * \brief Операторы
 * \details Позволяет вычертить границы объектов используя операторы Робертса:
 * \code
 * //Горизонтальный
 * int Gx[3][3] = { {-1,0,1},
 *                   {-1,0,1},
 *                   {-1,0,1} };
 * //Вертикальный
 * int Gy[3][3] ={ {-1,-1,-1},
 *                   {0,0,0},
 *                   {1,1,1} };
 * \endcode
 */
class SRobertsOperator : public SProcessing
{
private:
    int Gx[2][2] = { {1,0},
                     {0,-1} };
    int Gy[2][2] = { {0,1},
                     {-1,0} };
public:
    ///\brief Пустой конструктор.
    SRobertsOperator(){}
    SMatrix bypass(SMatrix &src);
};

#endif // EDGEDETECTION_H
