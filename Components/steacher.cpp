#include "steacher.h"



STeacher::STeacher()
{
    marks = {{0xFFFF0000,1}, //red
             {0xFFFFFF00,2}, //yellow
             {0xFF00FF00,3}, //green
             {0xFF00FFFF,4}, //cyan
             {0xFF0000FF,5}, //blue
             {0xFFFF00FF,6}}; //magenta
}

STeacher::STeacher(std::map<int, int> &&marks, double necessity_threshold)
    :marks(marks),necessity_thrs(necessity_threshold)
{
    if (necessity_threshold>1 || necessity_threshold<0)
        throw std::invalid_argument("STeacher: necessity_threshold must be within [0,1]");
}

void STeacher::rebuild(const SMatrix &img, bool ignore_zero)
{
    using namespace std;
    map<int,int> counters;
    for (int y=0;y<img.height();++y)
        for (int x=0;x<img.width();++x)
        {
            int pix = img(x,y);
            if (!ignore_zero || pix != 0)
                ++counters[pix];
        }

    segment_class=0;
    if (counters.size()!=0)
    {
        for (pair<int,int> c:counters)
            if (marks.find(c.first)!=marks.end())
            {
                double ratio = double(c.second)/
                        double(img.width()*img.height());
                if  (ratio>necessity_thrs)
                {
                    if (segment_class!=0)
                    {
                        qDebug("STeacher: the markup used permits duality");
                        qDebug("    help: try to improve threshold of necessity");
                    }
                    segment_class = marks[c.first];
                }
            }
    }
}

std::list<std::__cxx11::string> STeacher::getHeader(const std::__cxx11::string &predicat)
{return {predicat+"Y"};}

std::list<double> STeacher::getFeatures()
{
    return {double(segment_class)};
}
