#ifndef SIMAGEDESCRIPTOR_H
#define SIMAGEDESCRIPTOR_H

#include <typeinfo>
#include <QApplication>
#include <SLib/Core/ssegmentationmap.h>
#include <SLib/Features/sabstractfeatures.h>
#include <SLib/Core/sdataframe.h>

/*!
 * \ingroup Core
 * \brief Описатель изображений.
 * \details Данный класс прдназначается для обеспечения процедуры описания сегментов цветного изображения,
 * по заданным пользователем планам и признакам. В качестве входных данных требуется цветное изображение и
 * его карта сегментции. В итоге получается таблица "Объекты-признаки". Несколько таких таблиц нетрудно
 * соединить методами SDataFrame.
 */
class SImageDescriptor
{
    const QImage& src;                          //!<Константная ссылка на цветное изображение
    const SSegmentationMap& segments;           //!<Константная ссылка на карту сегментации
    std::map<std::string,SFunctor> components;  //!<Используемые планы
    std::list<SAbstractFeatures*> all_features; //!<Считаемые признаки
protected:
    bool isReady();
public:
    SImageDescriptor(const QImage& img,const SSegmentationMap& seg_map);
    bool addComponent(const std::string& name,SFunctor component);
    bool addFeatures(SAbstractFeatures* features);    
    virtual void reset();
    virtual SDataFrame run(const std::string& img_prefix="");
    ~SImageDescriptor(){for(auto f:all_features) delete f;} //!Деструктор, освобождающий память
};

#endif // SIMAGEDESCRIPTOR_H
