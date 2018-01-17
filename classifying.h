#ifndef CLASSIFYING_H
#define CLASSIFYING_H
/*!
 *\defgroup Classifying Модуль классификации "Classifying"
 *\brief Данный модуль, содержит алгоритмы классификации.
 *\details Все классификаторы модуля строятся по следующей схеме:
 * 1. Все классификаторы - наследники SAbstractClassifier.
 * 2. Метод fit - обучает классификатор.
 *  В качестве входных данных используется таблица "Объекты-признаки"(SDataFrame) и столбец ответов(vector<int>).
 * 3. Метод predict - дает результаты классификации.
 *  В качестве входных данных используется таблица "Объекты-признаки"(SDataFrame)
 *  В качестве выходных данных используется столбец предсказаний(vector<int>)
 */
#include <SLib\Classifying\sabstractclassifier.h>
#include <SLib\Classifying\skneighborsclassifier.h>

#endif // CLASSIFYING_H
