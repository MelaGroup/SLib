#ifndef CORE_H
#define CORE_H

#include <SLib\Core\sdataframe.h>
#include <SLib\Core\simagedescriptor.h>
#include <SLib\Core\smatrix.h>
#include <SLib\Core\ssegmentationmap.h>
/*!
 * \defgroup Core Модуль базовых классов "Core"
 * \brief Данный модуль содержит все базовые классы, связанные с таблицами.
 * \details Классы модуля не имеют единого принципа построения. Однако, в совокупности
 * они обеспечивают процедуру описания сегментов от оригинального изображения до таблицы "Объекты признаки".
 * Содержит 5 классов:
 * - SMatrix - полутоновое изображение
 * - SSegment - первичная информация о сегменте
 * - SSegmentationMap - карта сегментации
 * - SDataFrame - таблица "объекты-признаки"
 * - SImageDescriptor - генератор таблиц "объекты-признаки"
 */
#endif // CORE_H
