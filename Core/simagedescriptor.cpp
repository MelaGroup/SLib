#include "simagedescriptor.h"

/*!
 * \brief Проверка на готовность.
 * \details Проверяет, готов ли основной алгоритм run() к запуску:
 * \return false - если не заданы признаки или планы
 */
bool SImageDescriptor::isReady()
{ 
    const bool valid_comp = !components.empty();
    const bool valid_fs   = !all_features.empty();

    if (!valid_comp) qDebug()<<"SImageDescriptor: stop - no component ";   
    if (!valid_fs  ) qDebug()<<"SImageDescriptor: stop - no features";

    return valid_comp && valid_fs;
}

/*!
 * \brief Конструктор по цветному изображению и его карте сегментации
 * \details Задает цветное изображение и карту сегментации, необходимых для процедуры описания.
 *  Во избежание избыточного копирования используются константные ссылки на них.
 * \param img - константная ссылка на изображение
 * \param seg_map - константная ссылка на карту сегментации
 * \throw invalid_argument - если аргументы не валидны или не совместимы по размеру.
 * \warning Оригиналы передаваемых цветного изображения и карты сегментации должны существовать до вызова
 * деструктора SImageDescriptor.
 */
SImageDescriptor::SImageDescriptor(const QImage &img, const SSegmentationMap &seg_map):
    src(img),segments(seg_map)
{
    if (!seg_map.isCompatible(img))
        throw std::invalid_argument("SImageDescriptor: img and seg_map must be compatible");
    if (!seg_map.isValid())
        throw std::invalid_argument("SImageDescriptor: seg_map must be valid");
    if (img.isNull())
        throw std::invalid_argument("SImageDescriptor: img must be valid");
}

/*!
 * \brief Добавление плана.
 * \details Позволяет добавить новый план, по которому будут считаться признаки.
 * \param name - название плана
 * \param component - правило создания полутонового изображения
 * \return false - если такое имя уже существует
 */
bool SImageDescriptor::addComponent(const std::__cxx11::string &name, SFunctor component)
{
    if (components.find(name)==components.end())
    {
        components[name]=component;
        return true;
    }
    return false;
}

/*!
 * \brief Добавление признаков.
 * \details Позволяет добавить группу признаков, которые будут расчитываться для сегментов карты на различных планах.
 * Лучше всего передавать их динамечески через new для корректной работы деструктора.
 * \param features - указатель на класс расчет признаков
 * \return true - если указатель не nullptr
 * \warning Описатель и только он является владельцем динамически созданных признаков.
 */
bool SImageDescriptor::addFeatures(SAbstractFeatures *features)
{
    if (features!=nullptr)
    {
        all_features.push_back(features);
        return true;
    }
    return false;
}

/*!
 * \brief Запуск алгоритма описания изображения.
 * \details При условии что цветное изображение, его карта, планы и признаки были заданы, запускается основной алгоритм в
 * результате которого получается таблица с описаниями сегментов. Можно задать приставку к названиям сегментов (в этой
 * роли выступают идентификаторы сегментов) которая будет добавлена через "_". Названия признаков складываются из
 * название плана и его оригинального названия через "_".
 * \param img_prefix - приставка к названиям сегментов
 * \return таблицу "Объекты-признаки"
 */
SDataFrame SImageDescriptor::run(const std::__cxx11::string &img_prefix)
{
    using namespace std;
    SDataFrame X;
    if (isReady())
    {
        //emit status(".start:"+QString::fromStdString(img_predicat));
        qApp->processEvents();
        vector<int> ids=segments.IDs();       
        for (const pair<string,SFunctor>& c:components)
        {
            //emit status(".."+QString::fromStdString(c.first));
            qApp->processEvents();
            SMatrix plane(src,c.second);
            for (SAbstractFeatures* f:all_features)
            {                
                //emit status("..."+QString::fromStdString(typeid(*f).name()));
                qApp->processEvents();
                SDataFrame block;
                auto header=f->getHeader(c.first+"_");
                block.setHeader(header);
                for (int id:ids)
                {
                    //emit status("...."+id);
                    qApp->processEvents();
                    SMatrix segment = segments.getSegment(plane,id);
                    f->rebuild(segment,true);
                    block.newObject(img_prefix+to_string(id),f->getFeatures());
                }
                X+=block;
            }
        }
        //emit status(".end:"+QString::fromStdString(img_predicat));
    }   
    return X;
}

/*!
 * \brief Сброс настроек в исходное состояние.
 */
void SImageDescriptor::reset()
{
    components.clear();
    for(auto f:all_features) delete f;
    all_features.clear();
}
