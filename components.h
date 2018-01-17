#ifndef COMPONENTS_H
#define COMPONENTS_H

/*!
 *\defgroup Components Модуль компонент "Components"
 *\brief Данный модуль, содержит правила преобразования цветного изображение в полутоновое.
 *\details Принципы и парадигмы:
 * 1. Правило преобразования цветного изображение в полутоновое - это правило преобразования пикселя из QColor в int
 * 2. Преобразование в полутоновое должно происходить по любому осмысленному правилу.
 * 3. Создание правил может делегироваться другим классам.
 */

#include <SLib\Components\sfunctors.h>
#include <SLib\Components\slinearalgebra.h>
#include <SLib\Components\smaincomponents.h>

#endif // COMPONENTS_H
