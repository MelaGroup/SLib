#include "simagestatistics.h"

SCovarienceMatrix::SCovarienceMatrix(const QImage &src)
{
    double Area=double(src.height()*src.width());

    double MR=0,MG=0,MB=0;
    for(int y=0;y<src.height();++y)
        for(int x=0;x<src.width();++x)
        {
            QRgb p=src.pixel(x,y);
            MR+=double(qRed(p));
            MG+=double(qGreen(p));
            MB+=double(qBlue(p));
        }
    MR/=Area;MG/=Area;MB/=Area;

    double DR=0,DG=0,DB=0;
    double covRG=0,covRB=0,covGB=0;
    for(int y=0;y<src.height();++y)
        for(int x=0;x<src.width();++x)
        {
            QRgb p=src.pixel(x,y);
            double deltaR=double(qRed(p))-MR;
            double deltaG=double(qGreen(p))-MG;
            double deltaB=double(qBlue(p))-MB;

            DR+=deltaR*deltaR;
            DG+=deltaG*deltaG;
            DB+=deltaB*deltaB;

            covRG+=deltaR*deltaG;
            covRB+=deltaR*deltaB;
            covGB+=deltaG*deltaB;
        }
    DR/=Area;DG/=Area;DB/=Area;
    covRG/=Area;covRB/=Area;covGB/=Area;
    a(1,1)=DR   ;a(1,2)=covRG;a(1,3)=covRB;
    a(2,1)=covRG;a(2,2)=DG   ;a(2,3)=covGB;
    a(3,1)=covRB;a(3,2)=covGB;a(3,3)=DB   ;
}
