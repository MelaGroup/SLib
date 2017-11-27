#include "shistogram.h"

void SHistogram::ignoreZero()
{
    sum-=bars[0];
    bars.erase(0);
    if (min_key==0)
        min_key=bars.begin()->first;
    if (max_key==0)
        max_key=bars.rbegin()->first;
}

SHistogram::SHistogram(const SMatrix &src, bool ignore_zero)
{
    rebuild(src,ignore_zero);
}

int SHistogram::MX()
{
    double av=0;
    for(int i=min_key;i<=max_key;++i)
        av+=double(i*bars[i]);
    av/=double(sum);
    return av;
}

int SHistogram::DX(int av)
{
    if (av==std::numeric_limits<int>::max())
        av=MX();
    double dv=0;
    for(int i=min_key;i<=max_key;++i)
        dv+=double(bars[i]*(i-av)*(i-av));
    dv/=double(sum);
    return dv;
}

void SHistogram::rebuild(const SMatrix &src, bool ignore_zero)
{
    min_key=max_key=src(0,0);
    bars.clear();
    for(int r=0;r<src.height();++r)
        for(int c=0;c<src.width();++c)
        {
            int key=src(c,r);
            if (key<min_key) min_key=key;
            if (max_key<key) max_key=key;
            ++bars[key];
        }    
    sum=src.width()*src.height();
    if (ignore_zero) ignoreZero();
}

std::list<std::__cxx11::string> SHistogram::getHeader(const std::string& predicat)
{
    std::list<std::string> header;
    header.push_back(predicat+"MIN");
    header.push_back(predicat+"MAX");
    header.push_back(predicat+"SPAN");
    header.push_back(predicat+"MX");
    header.push_back(predicat+"DX");
    return header;
}

std::list<double> SHistogram::getFeatures()
{
    std::list<double> features;
    features.push_back(min());
    features.push_back(max());
    features.push_back(span());
    int average = MX();
    features.push_back(average);
    features.push_back(DX(average));
    return features;
}

QImage SHistogram::toImage()
{
    int max_bar=0;
    for(int i=min_key;i<=max_key;++i)
        if (max_bar<bars[i]) max_bar=bars[i];

    QImage img(span(),max_bar+1,QImage::Format_RGB888);

    for(int x=min_key;x<=max_key;++x)
        for(int y=0;y<=max_bar;++y)
        {
            int pix = (y<=bars[x])?0:255;
            img.setPixel(x,y,qRgb(pix,pix,pix));
        }
    return img.mirrored();
}
