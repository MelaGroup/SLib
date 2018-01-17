#ifndef FEATURES_H
#define FEATURES_H

/*!
 *\defgroup Features Модуль признаков "Features"
 *\brief Данный модуль содержит специальные классы для вычисления признаков на полутоновом изображении.
 *\details Все классы модуля строятся по следующей схеме:
 * 1. Отнаследованны от SAbstractFeatures.
 * 2. Названия признаков дает метод getHeader. Можно указать приставку, которая будет добавлена ко всем названиям признаков.
 * 3. Значения признаков дает метод getFeatures.
 * 4. Методом rebuild можно перевычислить признаки на новом изображении.
 * 5. Игнорирование черного фона при вычислении признаков с помощью флага ignore_zero. Значение флага при игнорировании - true.
 */
#include <SLib\Features\sabstractfeatures.h>
#include <SLib\Features\sadjacencymatrix.h>
#include <SLib\Features\shistogram.h>
#include <SLib\Features\steacher.h>

#endif // FEATURES_H
