#ifndef PROCESSING_H
#define PROCESSING_H

/*!
 * \defgroup Processing Модуль предобработки "Processing"
 * \brief Данный модуль содержит ряд алгоритмов предобработки на полутоновых изображениях.
 * \details Все классы модуля строятся по следующей схеме:
 * - Отнаследованны от sprocessing.
 * - Имеют метод bypass для обхода(обработки) изображения. Метод не меняет оригинальное изображение,
 * т.е возвращает его преобразованную копию.
 * - Несколько методов могут собираться в конвеер SConveyor.
 */
#include <SLib\Processing\sprocessing.h>
#include <SLib\Processing\sconveyor.h>
#include <SLib\Processing\sbilateralfilter.h>
#include <SLib\Processing\sedgedetection.h>
#include <SLib\Processing\sgaussfilter.h>
#include <SLib\Processing\slaplaceoperator.h>
#include <SLib\Processing\ssquaremask.h>
#include <SLib\Processing\sthreshold.h>

#endif // PROCESSING_H
